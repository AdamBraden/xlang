#include <assert.h>
#include <string.h>

#include "xmeta_idl_reader.h"
#include "ast_to_st_listener.h"
#include "XlangLexer.h"
#include "XlangParser.h"
#include "xlang_model_walker.h"

namespace xlang::xmeta
{
    void xmeta_idl_reader::read(std::istream& idl_contents, bool disable_error_reporting)
    {
        ast_to_st_listener listener{ m_xlang_model, m_error_manager };
        read(idl_contents, listener, disable_error_reporting);
    }

    void xmeta_idl_reader::read(std::istream& idl_contents, XlangParserBaseListener& listener, bool disable_error_reporting)
    {
        antlr4::ANTLRInputStream input{ idl_contents };
        XlangLexer lexer{ &input };
        antlr4::CommonTokenStream tokens{ &lexer };

        XlangParser parser{ &tokens };
        //parser.setBuildParseTree(true);

        if (disable_error_reporting) {
            lexer.removeErrorListeners();
            parser.removeErrorListeners();
        }

        antlr4::tree::ParseTree *tree = parser.xlang();
        antlr4::tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);
        m_error_manager.set_num_syntax_error(parser.getNumberOfSyntaxErrors());
        pass1();
        pass2();
    }

    void xmeta_idl_reader::pass1()
    {
        xlang_model_pass_1 pass1_listener(m_xlang_model.symbols, m_error_manager);
        xlang_model_walker walker(m_xlang_model.namespaces, pass1_listener);
        walker.walk();
    }

    // We can probably combine pass2 and the last pass in xmeta_emit together. 
    void xmeta_idl_reader::pass2()
    {
        xlang_model_pass_2 pass2_listener(m_xlang_model.symbols, m_error_manager);
        xlang_model_walker walker(m_xlang_model.namespaces, pass2_listener);
        walker.walk();
    }

    void xlang_model_pass_1::listen_interface_model(std::shared_ptr<interface_model> const& model)
    {
        model->resolve(m_symbols, m_error_manager);
    }

    void xlang_model_pass_1::listen_struct_model(std::shared_ptr<struct_model> const& model)
    {
        model->resolve(m_symbols, m_error_manager);
    }

    void xlang_model_pass_1::listen_delegate_model(std::shared_ptr<delegate_model> const& model)
    {
        model->resolve(m_symbols, m_error_manager);
    }

    void xlang_model_pass_2::listen_interface_model(std::shared_ptr<interface_model> const& model)
    {
        model->has_circular_inheritance(m_symbols, m_error_manager);
    }

    void xlang_model_pass_2::listen_struct_model(std::shared_ptr<struct_model> const& model)
    {
        model->has_circular_struct_declarations(m_symbols, m_error_manager);
    }
}
