// This is a generated file, do not edit.  Generated from ../../gnome/glibmm/glib/glibmm/../src/value_basictypes.h.m4

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#ifndef _GLIBMM_VALUE_H_INCLUDE_VALUE_BASICTYPES_H
#error "glibmm/value_basictypes.h cannot be included directly"
#endif
#endif

/* Suppress warnings about `long long' when GCC is in -pedantic mode.
 */
#if (__GNUC__ >= 3 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 96))
#pragma GCC system_header
#endif

namespace Glib
{

/**
 * @ingroup glibmmValue
 */
template <>
class GLIBMM_API Value<bool> : public ValueBase
{
public:
  using CppType = bool;

  static GType value_type() G_GNUC_CONST;

  void set(bool data);
  bool get() const;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  GParamSpec* create_param_spec(const Glib::ustring& name, const Glib::ustring& nick,
                                const Glib::ustring& blurb, Glib::ParamFlags flags) const;
#endif
};

/// @newin{2,44}

/**
 * @ingroup glibmmValue
 */
template <>
class GLIBMM_API Value<signed char> : public ValueBase
{
public:
  using CppType = signed char;

  static GType value_type() G_GNUC_CONST;

  void set(signed char data);
  signed char get() const;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  GParamSpec* create_param_spec(const Glib::ustring& name, const Glib::ustring& nick,
                                const Glib::ustring& blurb, Glib::ParamFlags flags) const;
#endif
};


/**
 * @ingroup glibmmValue
 */
template <>
class GLIBMM_API Value<unsigned char> : public ValueBase
{
public:
  using CppType = unsigned char;

  static GType value_type() G_GNUC_CONST;

  void set(unsigned char data);
  unsigned char get() const;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  GParamSpec* create_param_spec(const Glib::ustring& name, const Glib::ustring& nick,
                                const Glib::ustring& blurb, Glib::ParamFlags flags) const;
#endif
};


/**
 * @ingroup glibmmValue
 */
template <>
class GLIBMM_API Value<int> : public ValueBase
{
public:
  using CppType = int;

  static GType value_type() G_GNUC_CONST;

  void set(int data);
  int get() const;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  GParamSpec* create_param_spec(const Glib::ustring& name, const Glib::ustring& nick,
                                const Glib::ustring& blurb, Glib::ParamFlags flags) const;
#endif
};


/**
 * @ingroup glibmmValue
 */
template <>
class GLIBMM_API Value<unsigned int> : public ValueBase
{
public:
  using CppType = unsigned int;

  static GType value_type() G_GNUC_CONST;

  void set(unsigned int data);
  unsigned int get() const;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  GParamSpec* create_param_spec(const Glib::ustring& name, const Glib::ustring& nick,
                                const Glib::ustring& blurb, Glib::ParamFlags flags) const;
#endif
};


/**
 * @ingroup glibmmValue
 */
template <>
class GLIBMM_API Value<long> : public ValueBase
{
public:
  using CppType = long;

  static GType value_type() G_GNUC_CONST;

  void set(long data);
  long get() const;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  GParamSpec* create_param_spec(const Glib::ustring& name, const Glib::ustring& nick,
                                const Glib::ustring& blurb, Glib::ParamFlags flags) const;
#endif
};


/**
 * @ingroup glibmmValue
 */
template <>
class GLIBMM_API Value<unsigned long> : public ValueBase
{
public:
  using CppType = unsigned long;

  static GType value_type() G_GNUC_CONST;

  void set(unsigned long data);
  unsigned long get() const;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  GParamSpec* create_param_spec(const Glib::ustring& name, const Glib::ustring& nick,
                                const Glib::ustring& blurb, Glib::ParamFlags flags) const;
#endif
};


/**
 * @ingroup glibmmValue
 */
template <>
class GLIBMM_API Value<long long> : public ValueBase
{
public:
  using CppType = long long;

  static GType value_type() G_GNUC_CONST;

  void set(long long data);
  long long get() const;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  GParamSpec* create_param_spec(const Glib::ustring& name, const Glib::ustring& nick,
                                const Glib::ustring& blurb, Glib::ParamFlags flags) const;
#endif
};


/**
 * @ingroup glibmmValue
 */
template <>
class GLIBMM_API Value<unsigned long long> : public ValueBase
{
public:
  using CppType = unsigned long long;

  static GType value_type() G_GNUC_CONST;

  void set(unsigned long long data);
  unsigned long long get() const;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  GParamSpec* create_param_spec(const Glib::ustring& name, const Glib::ustring& nick,
                                const Glib::ustring& blurb, Glib::ParamFlags flags) const;
#endif
};


/**
 * @ingroup glibmmValue
 */
template <>
class GLIBMM_API Value<float> : public ValueBase
{
public:
  using CppType = float;

  static GType value_type() G_GNUC_CONST;

  void set(float data);
  float get() const;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  GParamSpec* create_param_spec(const Glib::ustring& name, const Glib::ustring& nick,
                                const Glib::ustring& blurb, Glib::ParamFlags flags) const;
#endif
};


/**
 * @ingroup glibmmValue
 */
template <>
class GLIBMM_API Value<double> : public ValueBase
{
public:
  using CppType = double;

  static GType value_type() G_GNUC_CONST;

  void set(double data);
  double get() const;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  GParamSpec* create_param_spec(const Glib::ustring& name, const Glib::ustring& nick,
                                const Glib::ustring& blurb, Glib::ParamFlags flags) const;
#endif
};


/**
 * @ingroup glibmmValue
 */
template <>
class GLIBMM_API Value<void*> : public ValueBase
{
public:
  using CppType = void*;

  static GType value_type() G_GNUC_CONST;

  void set(void* data);
  void* get() const;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  GParamSpec* create_param_spec(const Glib::ustring& name, const Glib::ustring& nick,
                                const Glib::ustring& blurb, Glib::ParamFlags flags) const;
#endif
};

} // namespace Glib
