// WARNING: Please don't edit this file. It was generated by Python/WinRT

#pragma once

// These two headers are part of the Python C Extension API
#include <Python.h>
#include <structmember.h>

#include <winrt/base.h>

namespace winrt::impl
{
    // Bug 19167653: C++/WinRT missing category for AsyncStatus. 
    // The following line can be removed after 19167653 is fixed
    template <> struct category<winrt::Windows::Foundation::AsyncStatus> { using type = enum_category; };
}

namespace py
{
    template <typename Category>
    struct pinterface_checker
    {
        static constexpr bool value = false;
    };

    template <typename... Args>
    struct pinterface_checker<winrt::impl::pinterface_category<Args...>>
    {
        static constexpr bool value = true;
    };

    template <typename Category>
    struct struct_checker
    {
        static constexpr bool value = false;
    };

    template <typename... Args>
    struct struct_checker<winrt::impl::struct_category<Args...>>
    {
        static constexpr bool value = true;
    };

    template <typename T>
    struct pinterface_python_type
    {
        using abstract = void;
        using concrete = void;
    };

    template <typename T>
    struct delegate_python_type
    {
        using type = void;
    };

    template<typename T>
    constexpr bool is_basic_category_v = std::is_same_v<winrt::impl::category_t<T>, winrt::impl::basic_category>;

    template<typename T>
    constexpr bool is_class_category_v = std::is_same_v<winrt::impl::category_t<T>, winrt::impl::class_category>;

    template<typename T>
    constexpr bool is_delegate_category_v = std::is_same_v<winrt::impl::category_t<T>, winrt::impl::delegate_category>;

    template<typename T>
    constexpr bool is_enum_category_v = std::is_same_v<winrt::impl::category_t<T>, winrt::impl::enum_category>;

    template<typename T>
    constexpr bool is_interface_category_v = std::is_same_v<winrt::impl::category_t<T>, winrt::impl::interface_category>;

    template<typename T>
    constexpr bool is_pinterface_category_v = pinterface_checker<typename winrt::impl::category<T>::type>::value;

    template<typename T>
    constexpr bool is_struct_category_v = struct_checker<typename winrt::impl::category<T>::type>::value;

    struct winrt_wrapper_base
    {
        PyObject_HEAD;

        // PyObject_New doesn't call type's constructor, so manually manage the "virtual" get_unknown function
        winrt::Windows::Foundation::IUnknown const&(*get_unknown)(winrt_wrapper_base* self);
    };

    template<typename T>
    struct winrt_struct_wrapper
    {
        PyObject_HEAD;
        T obj{ };
    };

    template<typename T>
    struct winrt_wrapper : winrt_wrapper_base
    {
        T obj{ nullptr };

        static winrt::Windows::Foundation::IUnknown const& fetch_unknown(winrt_wrapper_base* self)
        {
            return reinterpret_cast<winrt_wrapper<T>*>(self)->obj;
        }
    };

    template<typename T>
    struct winrt_pinterface_wrapper : winrt_wrapper_base
    {
        std::unique_ptr<T> obj{ nullptr };

        static winrt::Windows::Foundation::IUnknown const& fetch_unknown(winrt_wrapper_base* self)
        {
            return reinterpret_cast<winrt_pinterface_wrapper<T>*>(self)->obj->get_unknown();
        }
    };

    template<typename To>
    To as(winrt_wrapper_base* wrapper)
    {
        return wrapper->get_unknown(wrapper).as<To>();
    }

    struct winrt_base;

    template<typename T>
    struct winrt_type { static PyTypeObject* python_type; };

    inline __declspec(noinline) PyObject* to_PyErr() noexcept
    {
        try
        {
            throw;
        }
        catch (winrt::hresult_error const& e)
        {
            PyErr_SetString(PyExc_RuntimeError, winrt::to_string(e.message()).c_str());
        }
        catch (std::bad_alloc const&)
        {
            return PyErr_NoMemory();
        }
        catch (std::out_of_range const& e)
        {
            PyErr_SetString(PyExc_IndexError, e.what());
        }
        catch (std::invalid_argument const& e)
        {
            PyErr_SetString(PyExc_TypeError, e.what());
        }
        catch (std::exception const& e)
        {
            PyErr_SetString(PyExc_RuntimeError, e.what());
        }

        return nullptr;
    }

    void wrapped_instance(std::size_t key, PyObject* obj);
    PyObject* wrapped_instance(std::size_t key);

    template<typename T>
    inline auto get_instance_hash(T const& instance)
    {
        return std::hash<winrt::Windows::Foundation::IUnknown>{}(instance);
    }

    template<typename T>
    PyObject* wrap_struct(T instance, PyTypeObject* type_object)
    {
        if (type_object == nullptr)
        {
            PyErr_SetNone(PyExc_NotImplementedError);
            return nullptr;
        }

        auto py_instance = PyObject_New(py::winrt_struct_wrapper<T>, type_object);

        if (!py_instance)
        {
            return nullptr;
        }

        // PyObject_New doesn't call type's constructor, so manually initialize the wrapper's fields
        std::memset(&(py_instance->obj), 0, sizeof(py_instance->obj));
        py_instance->obj = instance;

        return reinterpret_cast<PyObject*>(py_instance);
    }

    template<typename T>
    PyObject* wrap(T instance, PyTypeObject* type_object)
    {
        if (type_object == nullptr)
        {
            PyErr_SetNone(PyExc_NotImplementedError);
            return nullptr;
        }

        auto py_instance = PyObject_New(py::winrt_wrapper<T>, type_object);

        if (!py_instance)
        {
            return nullptr;
        }

        // PyObject_New doesn't call type's constructor, so manually initialize the wrapper's fields
        py_instance->get_unknown = &winrt_wrapper<T>::fetch_unknown;
        std::memset(&(py_instance->obj), 0, sizeof(py_instance->obj));
        py_instance->obj = instance;

        wrapped_instance(get_instance_hash(instance), reinterpret_cast<PyObject*>(py_instance));

        return reinterpret_cast<PyObject*>(py_instance);
    }

    template<typename T>
    PyObject* wrap_pinterface(T instance)
    {
        using ptype = pinterface_python_type<T>;

        PyTypeObject* type_object = winrt_type<ptype::abstract>::python_type;

        if (type_object == nullptr)
        {
            PyErr_SetNone(PyExc_NotImplementedError);
            return nullptr;
        }

        auto py_instance = PyObject_New(py::winrt_pinterface_wrapper<ptype::abstract>, type_object);

        if (!py_instance)
        {
            return nullptr;
        }

        // PyObject_New doesn't call type's constructor, so manually initialize the wrapper's fields
        py_instance->get_unknown = &winrt_pinterface_wrapper<ptype::abstract>::fetch_unknown;
        std::memset(&(py_instance->obj), 0, sizeof(py_instance->obj));
        py_instance->obj = std::make_unique<ptype::concrete>(instance);

        wrapped_instance(get_instance_hash(instance), reinterpret_cast<PyObject*>(py_instance));

        return reinterpret_cast<PyObject*>(py_instance);
    }

    template<typename T>
    PyObject* wrap(T instance)
    {
        if constexpr (is_class_category_v<T> || is_interface_category_v<T>)
        {
            return wrap<T>(instance, winrt_type<T>::python_type);
        }
        else
        {
            if constexpr (std::is_same_v<pinterface_python_type<T>::abstract, void>)
            {
                PyErr_SetNone(PyExc_NotImplementedError);
                return nullptr;
            }
            else
            {
                return wrap_pinterface<T>(instance);
            }
        }
    }

    template <typename T>
    struct converter
    {
        static PyObject* convert(T value) noexcept
        {
            PyErr_SetNone(PyExc_NotImplementedError);
            return nullptr;
        }

        static T convert_to(PyObject* obj)
        {
            throw winrt::hresult_not_implemented();
        }
    };

    template <>
    struct converter<bool>
    {
        static PyObject* convert(bool value) noexcept
        {
            return PyBool_FromLong(value ? 1 : 0);
        }

        static bool convert_to(PyObject* obj)
        {
            auto result = PyObject_IsTrue(obj);

            if (result == -1)
            {
                throw winrt::hresult_invalid_argument();
            }

            return result > 0;
        }
    };

    template <>
    struct converter<int32_t>
    {
        static PyObject* convert(int32_t value) noexcept
        {
            return PyLong_FromLong(value);
        }

        static int32_t convert_to(PyObject* obj)
        {
            auto result = PyLong_AsLong(obj);

            if (result == -1 && PyErr_Occurred())
            {
                throw winrt::hresult_invalid_argument();
            }

            return result;
        }
    };

    template <>
    struct converter<uint32_t>
    {
        static PyObject* convert(uint32_t value) noexcept
        {
            return PyLong_FromUnsignedLong(value);
        }

        static uint32_t convert_to(PyObject* obj)
        {
            auto result = PyLong_AsUnsignedLong(obj);

            if (result == -1 && PyErr_Occurred())
            {
                throw winrt::hresult_invalid_argument();
            }

            return result;
        }
    };

    template <>
    struct converter<int64_t>
    {
        static PyObject* convert(int64_t value) noexcept
        {
            return PyLong_FromLongLong(value);
        }

        static int64_t convert_to(PyObject* obj)
        {
            auto result = PyLong_AsLongLong(obj);

            if (result == -1 && PyErr_Occurred())
            {
                throw winrt::hresult_invalid_argument();
            }

            return result;
        }
    };

    template <>
    struct converter<uint64_t>
    {
        static PyObject* convert(uint64_t value) noexcept
        {
            return PyLong_FromUnsignedLongLong(value);
        }

        static uint64_t convert_to(PyObject* obj)
        {
            auto result = PyLong_AsUnsignedLongLong(obj);

            if (result == -1 && PyErr_Occurred())
            {
                throw winrt::hresult_invalid_argument();
            }

            return result;
        }
    };

    template <>
    struct converter<float>
    {
        static PyObject* convert(float value) noexcept
        {
            return PyFloat_FromDouble(value);
        }

        static float convert_to(PyObject* obj)
        {
            auto result = PyFloat_AsDouble(obj);

            if (result == -1 && PyErr_Occurred())
            {
                throw winrt::hresult_invalid_argument();
            }

            return static_cast<float>(result);
        }
    };

    template <>
    struct converter<double>
    {
        static PyObject* convert(double value) noexcept
        {
            return PyFloat_FromDouble(value);
        }

        static double convert_to(PyObject* obj)
        {
            auto result = PyFloat_AsDouble(obj);

            if (result == -1 && PyErr_Occurred())
            {
                throw winrt::hresult_invalid_argument();
            }

            return result;
        }
    };

    struct pystring
    {
        wchar_t* buffer{ nullptr };
        std::wstring_view::size_type size{};

        explicit pystring(PyObject* obj)
        {
            Py_ssize_t py_size;
            buffer = PyUnicode_AsWideCharString(obj, &py_size);
            if (buffer != nullptr)
            {
                size = static_cast<std::wstring_view::size_type>(py_size);
            }
        }

        pystring(pystring& other) = delete;
        pystring& operator=(pystring const&) = delete;

        pystring(pystring&& other) noexcept : buffer(other.buffer), size(other.size)
        {
            other.buffer = nullptr;
        }

        pystring& operator=(pystring&& rhs)
        {
            std::swap(buffer, rhs.buffer);
            size = rhs.size;
        }

        operator bool() const noexcept
        {
            return buffer != nullptr;
        }

        ~pystring()
        {
            PyMem_Free(buffer);
        }
    };

    struct pystringview : public pystring, public std::wstring_view
    {
        explicit pystringview(PyObject* obj) : pystring(obj), std::wstring_view(pystring::buffer, pystring::size)
        {
        }
    };

    template <>
    struct converter<winrt::hstring>
    {
        static PyObject* convert(winrt::hstring value) noexcept
        {
            return PyUnicode_FromWideChar(value.c_str(), value.size());
        }

        static pystringview convert_to(PyObject* obj)
        {
            pystringview str{ obj };

            if (!str)
            {
                throw winrt::hresult_invalid_argument();
            }

            return str;
        }
    };

    template<typename T>
    PyObject* convert_enum(T instance) noexcept
    {
        using enum_type = std::underlying_type_t<T>;
        return converter<enum_type>::convert(static_cast<enum_type>(instance));
    }

    template<typename T>
    PyObject* convert(T instance) noexcept
    {
        if constexpr (is_class_category_v<T> || is_interface_category_v<T> || is_pinterface_category_v<T>)
        {
            return wrap(instance);
        }
        else if constexpr (is_enum_category_v<T>)
        {
            return convert_enum(instance);
        }
        else if constexpr (is_struct_category_v<T>)
        {
            return wrap_struct<T>(instance, winrt_type<T>::python_type);
        }
        else if constexpr (is_basic_category_v<T>)
        {
            return converter<T>::convert(instance);
        }
        else
        {
            PyErr_SetNone(PyExc_NotImplementedError);
            return nullptr;
        }
    }

    template<typename T>
    auto convert_to(PyObject* arg)
    {
        if (!arg)
        {
            throw winrt::hresult_invalid_argument();
        }

        if constexpr (is_class_category_v<T>)
        {
            if (Py_TYPE(arg) != winrt_type<T>::python_type)
            {
                throw winrt::hresult_invalid_argument();
            }

            return reinterpret_cast<winrt_wrapper<T>*>(arg)->obj;
        }
        else if constexpr (is_interface_category_v<T>)
        {
            if (Py_TYPE(arg) == winrt_type<T>::python_type)
            {
                return reinterpret_cast<winrt_wrapper<T>*>(arg)->obj;
            }

            if (PyObject_IsInstance(arg, reinterpret_cast<PyObject*>(winrt_type<winrt_base>::python_type)) == 0)
            {
                throw winrt::hresult_invalid_argument();
            }

            auto wrapper = reinterpret_cast<winrt_wrapper_base*>(arg);
            return as<T>(wrapper);
        }
        else if constexpr (is_pinterface_category_v<T>)
        {
            if constexpr (std::is_base_of_v<winrt::Windows::Foundation::IInspectable, T>)
            {
                // pinterface
                auto wrapper = reinterpret_cast<winrt_wrapper_base*>(arg);
                return as<T>(wrapper);
            }
            else
            {
                // pdelegate
                return delegate_python_type<T>::type::get(arg);
            }
        }
        else if constexpr (is_delegate_category_v<T>)
        {
            return delegate_python_type<T>::type::get(arg);
        }
        else if constexpr (is_enum_category_v<T>)
        {
            return static_cast<T>(convert_to<std::underlying_type_t<T>>(arg));
        }
        else if constexpr (is_struct_category_v<T>)
        {
            if (Py_TYPE(arg) == winrt_type<T>::python_type)
            {
                return reinterpret_cast<winrt_struct_wrapper<T>*>(arg)->obj;
            }

            if (PyDict_Check(arg))
            { 
                return converter<T>::convert_to(arg);
            }

            throw winrt::hresult_invalid_argument();
        }
        else if constexpr (is_basic_category_v<T>)
        {
            return converter<T>::convert_to(arg);
        }
        else
        {
            throw winrt::hresult_not_implemented();
        }
    }

    template<typename T>
    auto convert_to(PyObject* args, int index)
    {
        return convert_to<T>(PyTuple_GetItem(args, index));
    }
}