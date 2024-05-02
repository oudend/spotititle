// This is a generated file. Do not edit it.  Generated from ../../gnome/glibmm/glib/glibmm/../src/variant_basictypes.h.m4

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#ifndef _GLIBMM_VARIANT_H_INCLUDE_VARIANT_BASICTYPES_H
#error "glibmm/variant_basictypes.h cannot be included directly"
#endif
#else
#include <glibmmconfig.h>
#endif

namespace Glib
{

/** Specialization of Glib::Variant containing a bool type.
 *
 * @ingroup Variant
 */
template <>
class GLIBMM_API Variant<bool> : public VariantBase
{
public:
  using CType = gboolean;

  /// Default constructor.
  Variant()
  : VariantBase()
  {}

  /** GVariant constructor.
   * @param castitem The GVariant to wrap.
   * @param take_a_reference Whether to take an extra reference of the
   * GVariant or not (not taking one could destroy the GVariant with the
   * wrapper).
   */
  explicit Variant(GVariant* castitem, bool take_a_reference = false)
  : VariantBase(castitem, take_a_reference)
  {}

  /** Gets the Glib::VariantType.
   * @return The Glib::VariantType.
   */
  static const VariantType& variant_type() G_GNUC_CONST;

  /** Creates a new Glib::Variant<bool>.
   * @param data The value of the new Glib::Variant<bool>.
   * @return The new Glib::Variant<bool>.
   */
  static Variant<bool> create(bool data);

  /** Gets the value of the Glib::Variant<bool>.
   * @return The bool value of the Glib::Variant<bool>.
   */
  bool get() const;
};


/** Specialization of Glib::Variant containing a unsigned char type.
 *
 * @ingroup Variant
 */
template <>
class GLIBMM_API Variant<unsigned char> : public VariantBase
{
public:
  using CType = guchar;

  /// Default constructor.
  Variant()
  : VariantBase()
  {}

  /** GVariant constructor.
   * @param castitem The GVariant to wrap.
   * @param take_a_reference Whether to take an extra reference of the
   * GVariant or not (not taking one could destroy the GVariant with the
   * wrapper).
   */
  explicit Variant(GVariant* castitem, bool take_a_reference = false)
  : VariantBase(castitem, take_a_reference)
  {}

  /** Gets the Glib::VariantType.
   * @return The Glib::VariantType.
   */
  static const VariantType& variant_type() G_GNUC_CONST;

  /** Creates a new Glib::Variant<unsigned char>.
   * @param data The value of the new Glib::Variant<unsigned char>.
   * @return The new Glib::Variant<unsigned char>.
   */
  static Variant<unsigned char> create(unsigned char data);

  /** Gets the value of the Glib::Variant<unsigned char>.
   * @return The unsigned char value of the Glib::Variant<unsigned char>.
   */
  unsigned char get() const;
};


#if GLIBMM_SIZEOF_SHORT == 2 && GLIBMM_SIZEOF_INT == 2

/** Specialization of Glib::Variant containing a short type.
 *
 * @ingroup Variant
 */
template <>
class GLIBMM_API Variant<short> : public VariantBase
{
public:
  using CType = gint16;

  /// Default constructor.
  Variant()
  : VariantBase()
  {}

  /** GVariant constructor.
   * @param castitem The GVariant to wrap.
   * @param take_a_reference Whether to take an extra reference of the
   * GVariant or not (not taking one could destroy the GVariant with the
   * wrapper).
   */
  explicit Variant(GVariant* castitem, bool take_a_reference = false)
  : VariantBase(castitem, take_a_reference)
  {}

  /** Gets the Glib::VariantType.
   * @return The Glib::VariantType.
   */
  static const VariantType& variant_type() G_GNUC_CONST;

  /** Creates a new Glib::Variant<short>.
   * @param data The value of the new Glib::Variant<short>.
   * @return The new Glib::Variant<short>.
   */
  static Variant<short> create(short data);

  /** Gets the value of the Glib::Variant<short>.
   * @return The short value of the Glib::Variant<short>.
   */
  short get() const;
};


/** Specialization of Glib::Variant containing a unsigned short type.
 *
 * @ingroup Variant
 */
template <>
class GLIBMM_API Variant<unsigned short> : public VariantBase
{
public:
  using CType = guint16;

  /// Default constructor.
  Variant()
  : VariantBase()
  {}

  /** GVariant constructor.
   * @param castitem The GVariant to wrap.
   * @param take_a_reference Whether to take an extra reference of the
   * GVariant or not (not taking one could destroy the GVariant with the
   * wrapper).
   */
  explicit Variant(GVariant* castitem, bool take_a_reference = false)
  : VariantBase(castitem, take_a_reference)
  {}

  /** Gets the Glib::VariantType.
   * @return The Glib::VariantType.
   */
  static const VariantType& variant_type() G_GNUC_CONST;

  /** Creates a new Glib::Variant<unsigned short>.
   * @param data The value of the new Glib::Variant<unsigned short>.
   * @return The new Glib::Variant<unsigned short>.
   */
  static Variant<unsigned short> create(unsigned short data);

  /** Gets the value of the Glib::Variant<unsigned short>.
   * @return The unsigned short value of the Glib::Variant<unsigned short>.
   */
  unsigned short get() const;
};


/** Specialization of Glib::Variant containing a int type.
 *
 * @ingroup Variant
 */
template <>
class GLIBMM_API Variant<int> : public VariantBase
{
public:
  using CType = gint16;

  /// Default constructor.
  Variant()
  : VariantBase()
  {}

  /** GVariant constructor.
   * @param castitem The GVariant to wrap.
   * @param take_a_reference Whether to take an extra reference of the
   * GVariant or not (not taking one could destroy the GVariant with the
   * wrapper).
   */
  explicit Variant(GVariant* castitem, bool take_a_reference = false)
  : VariantBase(castitem, take_a_reference)
  {}

  /** Gets the Glib::VariantType.
   * @return The Glib::VariantType.
   */
  static const VariantType& variant_type() G_GNUC_CONST;

  /** Creates a new Glib::Variant<int>.
   * @param data The value of the new Glib::Variant<int>.
   * @return The new Glib::Variant<int>.
   */
  static Variant<int> create(int data);

  /** Gets the value of the Glib::Variant<int>.
   * @return The int value of the Glib::Variant<int>.
   */
  int get() const;
};


/** Specialization of Glib::Variant containing a unsigned int type.
 *
 * @ingroup Variant
 */
template <>
class GLIBMM_API Variant<unsigned int> : public VariantBase
{
public:
  using CType = guint16;

  /// Default constructor.
  Variant()
  : VariantBase()
  {}

  /** GVariant constructor.
   * @param castitem The GVariant to wrap.
   * @param take_a_reference Whether to take an extra reference of the
   * GVariant or not (not taking one could destroy the GVariant with the
   * wrapper).
   */
  explicit Variant(GVariant* castitem, bool take_a_reference = false)
  : VariantBase(castitem, take_a_reference)
  {}

  /** Gets the Glib::VariantType.
   * @return The Glib::VariantType.
   */
  static const VariantType& variant_type() G_GNUC_CONST;

  /** Creates a new Glib::Variant<unsigned int>.
   * @param data The value of the new Glib::Variant<unsigned int>.
   * @return The new Glib::Variant<unsigned int>.
   */
  static Variant<unsigned int> create(unsigned int data);

  /** Gets the value of the Glib::Variant<unsigned int>.
   * @return The unsigned int value of the Glib::Variant<unsigned int>.
   */
  unsigned int get() const;
};

#else

/** Specialization of Glib::Variant containing a gint16 type.
 *
 * @ingroup Variant
 */
template <>
class GLIBMM_API Variant<gint16> : public VariantBase
{
public:
  using CType = gint16;

  /// Default constructor.
  Variant()
  : VariantBase()
  {}

  /** GVariant constructor.
   * @param castitem The GVariant to wrap.
   * @param take_a_reference Whether to take an extra reference of the
   * GVariant or not (not taking one could destroy the GVariant with the
   * wrapper).
   */
  explicit Variant(GVariant* castitem, bool take_a_reference = false)
  : VariantBase(castitem, take_a_reference)
  {}

  /** Gets the Glib::VariantType.
   * @return The Glib::VariantType.
   */
  static const VariantType& variant_type() G_GNUC_CONST;

  /** Creates a new Glib::Variant<gint16>.
   * @param data The value of the new Glib::Variant<gint16>.
   * @return The new Glib::Variant<gint16>.
   */
  static Variant<gint16> create(gint16 data);

  /** Gets the value of the Glib::Variant<gint16>.
   * @return The gint16 value of the Glib::Variant<gint16>.
   */
  gint16 get() const;
};


/** Specialization of Glib::Variant containing a guint16 type.
 *
 * @ingroup Variant
 */
template <>
class GLIBMM_API Variant<guint16> : public VariantBase
{
public:
  using CType = guint16;

  /// Default constructor.
  Variant()
  : VariantBase()
  {}

  /** GVariant constructor.
   * @param castitem The GVariant to wrap.
   * @param take_a_reference Whether to take an extra reference of the
   * GVariant or not (not taking one could destroy the GVariant with the
   * wrapper).
   */
  explicit Variant(GVariant* castitem, bool take_a_reference = false)
  : VariantBase(castitem, take_a_reference)
  {}

  /** Gets the Glib::VariantType.
   * @return The Glib::VariantType.
   */
  static const VariantType& variant_type() G_GNUC_CONST;

  /** Creates a new Glib::Variant<guint16>.
   * @param data The value of the new Glib::Variant<guint16>.
   * @return The new Glib::Variant<guint16>.
   */
  static Variant<guint16> create(guint16 data);

  /** Gets the value of the Glib::Variant<guint16>.
   * @return The guint16 value of the Glib::Variant<guint16>.
   */
  guint16 get() const;
};

#endif

#if GLIBMM_SIZEOF_INT == 4 && GLIBMM_SIZEOF_LONG == 4

/** Specialization of Glib::Variant containing a int type.
 *
 * @ingroup Variant
 */
template <>
class GLIBMM_API Variant<int> : public VariantBase
{
public:
  using CType = gint32;

  /// Default constructor.
  Variant()
  : VariantBase()
  {}

  /** GVariant constructor.
   * @param castitem The GVariant to wrap.
   * @param take_a_reference Whether to take an extra reference of the
   * GVariant or not (not taking one could destroy the GVariant with the
   * wrapper).
   */
  explicit Variant(GVariant* castitem, bool take_a_reference = false)
  : VariantBase(castitem, take_a_reference)
  {}

  /** Gets the Glib::VariantType.
   * @return The Glib::VariantType.
   */
  static const VariantType& variant_type() G_GNUC_CONST;

  /** Creates a new Glib::Variant<int>.
   * @param data The value of the new Glib::Variant<int>.
   * @return The new Glib::Variant<int>.
   */
  static Variant<int> create(int data);

  /** Creates a new Glib::Variant<int> of type VARIANT_TYPE_HANDLE.
   * @param data The value of the new Glib::Variant<int>.
   * @return The new Glib::Variant<int>.
   */
  static Variant<int> create_handle(int data);

  /** Gets the value of the Glib::Variant<int>.
   * @return The int value of the Glib::Variant<int>.
   */
  int get() const;
};


/** Specialization of Glib::Variant containing a unsigned int type.
 *
 * @ingroup Variant
 */
template <>
class GLIBMM_API Variant<unsigned int> : public VariantBase
{
public:
  using CType = guint32;

  /// Default constructor.
  Variant()
  : VariantBase()
  {}

  /** GVariant constructor.
   * @param castitem The GVariant to wrap.
   * @param take_a_reference Whether to take an extra reference of the
   * GVariant or not (not taking one could destroy the GVariant with the
   * wrapper).
   */
  explicit Variant(GVariant* castitem, bool take_a_reference = false)
  : VariantBase(castitem, take_a_reference)
  {}

  /** Gets the Glib::VariantType.
   * @return The Glib::VariantType.
   */
  static const VariantType& variant_type() G_GNUC_CONST;

  /** Creates a new Glib::Variant<unsigned int>.
   * @param data The value of the new Glib::Variant<unsigned int>.
   * @return The new Glib::Variant<unsigned int>.
   */
  static Variant<unsigned int> create(unsigned int data);

  /** Gets the value of the Glib::Variant<unsigned int>.
   * @return The unsigned int value of the Glib::Variant<unsigned int>.
   */
  unsigned int get() const;
};


/** Specialization of Glib::Variant containing a long type.
 *
 * @ingroup Variant
 */
template <>
class GLIBMM_API Variant<long> : public VariantBase
{
public:
  using CType = gint32;

  /// Default constructor.
  Variant()
  : VariantBase()
  {}

  /** GVariant constructor.
   * @param castitem The GVariant to wrap.
   * @param take_a_reference Whether to take an extra reference of the
   * GVariant or not (not taking one could destroy the GVariant with the
   * wrapper).
   */
  explicit Variant(GVariant* castitem, bool take_a_reference = false)
  : VariantBase(castitem, take_a_reference)
  {}

  /** Gets the Glib::VariantType.
   * @return The Glib::VariantType.
   */
  static const VariantType& variant_type() G_GNUC_CONST;

  /** Creates a new Glib::Variant<long>.
   * @param data The value of the new Glib::Variant<long>.
   * @return The new Glib::Variant<long>.
   */
  static Variant<long> create(long data);

  /** Creates a new Glib::Variant<long> of type VARIANT_TYPE_HANDLE.
   * @param data The value of the new Glib::Variant<long>.
   * @return The new Glib::Variant<long>.
   */
  static Variant<long> create_handle(long data);

  /** Gets the value of the Glib::Variant<long>.
   * @return The long value of the Glib::Variant<long>.
   */
  long get() const;
};


/** Specialization of Glib::Variant containing a unsigned long type.
 *
 * @ingroup Variant
 */
template <>
class GLIBMM_API Variant<unsigned long> : public VariantBase
{
public:
  using CType = guint32;

  /// Default constructor.
  Variant()
  : VariantBase()
  {}

  /** GVariant constructor.
   * @param castitem The GVariant to wrap.
   * @param take_a_reference Whether to take an extra reference of the
   * GVariant or not (not taking one could destroy the GVariant with the
   * wrapper).
   */
  explicit Variant(GVariant* castitem, bool take_a_reference = false)
  : VariantBase(castitem, take_a_reference)
  {}

  /** Gets the Glib::VariantType.
   * @return The Glib::VariantType.
   */
  static const VariantType& variant_type() G_GNUC_CONST;

  /** Creates a new Glib::Variant<unsigned long>.
   * @param data The value of the new Glib::Variant<unsigned long>.
   * @return The new Glib::Variant<unsigned long>.
   */
  static Variant<unsigned long> create(unsigned long data);

  /** Gets the value of the Glib::Variant<unsigned long>.
   * @return The unsigned long value of the Glib::Variant<unsigned long>.
   */
  unsigned long get() const;
};

#else

/** Specialization of Glib::Variant containing a gint32 type.
 *
 * @ingroup Variant
 */
template <>
class GLIBMM_API Variant<gint32> : public VariantBase
{
public:
  using CType = gint32;

  /// Default constructor.
  Variant()
  : VariantBase()
  {}

  /** GVariant constructor.
   * @param castitem The GVariant to wrap.
   * @param take_a_reference Whether to take an extra reference of the
   * GVariant or not (not taking one could destroy the GVariant with the
   * wrapper).
   */
  explicit Variant(GVariant* castitem, bool take_a_reference = false)
  : VariantBase(castitem, take_a_reference)
  {}

  /** Gets the Glib::VariantType.
   * @return The Glib::VariantType.
   */
  static const VariantType& variant_type() G_GNUC_CONST;

  /** Creates a new Glib::Variant<gint32>.
   * @param data The value of the new Glib::Variant<gint32>.
   * @return The new Glib::Variant<gint32>.
   */
  static Variant<gint32> create(gint32 data);

  /** Creates a new Glib::Variant<gint32> of type VARIANT_TYPE_HANDLE.
   * @param data The value of the new Glib::Variant<gint32>.
   * @return The new Glib::Variant<gint32>.
   */
  static Variant<gint32> create_handle(gint32 data);

  /** Gets the value of the Glib::Variant<gint32>.
   * @return The gint32 value of the Glib::Variant<gint32>.
   */
  gint32 get() const;
};


/** Specialization of Glib::Variant containing a guint32 type.
 *
 * @ingroup Variant
 */
template <>
class GLIBMM_API Variant<guint32> : public VariantBase
{
public:
  using CType = guint32;

  /// Default constructor.
  Variant()
  : VariantBase()
  {}

  /** GVariant constructor.
   * @param castitem The GVariant to wrap.
   * @param take_a_reference Whether to take an extra reference of the
   * GVariant or not (not taking one could destroy the GVariant with the
   * wrapper).
   */
  explicit Variant(GVariant* castitem, bool take_a_reference = false)
  : VariantBase(castitem, take_a_reference)
  {}

  /** Gets the Glib::VariantType.
   * @return The Glib::VariantType.
   */
  static const VariantType& variant_type() G_GNUC_CONST;

  /** Creates a new Glib::Variant<guint32>.
   * @param data The value of the new Glib::Variant<guint32>.
   * @return The new Glib::Variant<guint32>.
   */
  static Variant<guint32> create(guint32 data);

  /** Gets the value of the Glib::Variant<guint32>.
   * @return The guint32 value of the Glib::Variant<guint32>.
   */
  guint32 get() const;
};

#endif

#if GLIBMM_SIZEOF_LONG == 8 && GLIBMM_SIZEOF_LONG_LONG == 8

/** Specialization of Glib::Variant containing a long type.
 *
 * @ingroup Variant
 */
template <>
class GLIBMM_API Variant<long> : public VariantBase
{
public:
  using CType = gint64;

  /// Default constructor.
  Variant()
  : VariantBase()
  {}

  /** GVariant constructor.
   * @param castitem The GVariant to wrap.
   * @param take_a_reference Whether to take an extra reference of the
   * GVariant or not (not taking one could destroy the GVariant with the
   * wrapper).
   */
  explicit Variant(GVariant* castitem, bool take_a_reference = false)
  : VariantBase(castitem, take_a_reference)
  {}

  /** Gets the Glib::VariantType.
   * @return The Glib::VariantType.
   */
  static const VariantType& variant_type() G_GNUC_CONST;

  /** Creates a new Glib::Variant<long>.
   * @param data The value of the new Glib::Variant<long>.
   * @return The new Glib::Variant<long>.
   */
  static Variant<long> create(long data);

  /** Gets the value of the Glib::Variant<long>.
   * @return The long value of the Glib::Variant<long>.
   */
  long get() const;
};


/** Specialization of Glib::Variant containing a unsigned long type.
 *
 * @ingroup Variant
 */
template <>
class GLIBMM_API Variant<unsigned long> : public VariantBase
{
public:
  using CType = guint64;

  /// Default constructor.
  Variant()
  : VariantBase()
  {}

  /** GVariant constructor.
   * @param castitem The GVariant to wrap.
   * @param take_a_reference Whether to take an extra reference of the
   * GVariant or not (not taking one could destroy the GVariant with the
   * wrapper).
   */
  explicit Variant(GVariant* castitem, bool take_a_reference = false)
  : VariantBase(castitem, take_a_reference)
  {}

  /** Gets the Glib::VariantType.
   * @return The Glib::VariantType.
   */
  static const VariantType& variant_type() G_GNUC_CONST;

  /** Creates a new Glib::Variant<unsigned long>.
   * @param data The value of the new Glib::Variant<unsigned long>.
   * @return The new Glib::Variant<unsigned long>.
   */
  static Variant<unsigned long> create(unsigned long data);

  /** Gets the value of the Glib::Variant<unsigned long>.
   * @return The unsigned long value of the Glib::Variant<unsigned long>.
   */
  unsigned long get() const;
};


/** Specialization of Glib::Variant containing a long long type.
 *
 * @ingroup Variant
 */
template <>
class GLIBMM_API Variant<long long> : public VariantBase
{
public:
  using CType = gint64;

  /// Default constructor.
  Variant()
  : VariantBase()
  {}

  /** GVariant constructor.
   * @param castitem The GVariant to wrap.
   * @param take_a_reference Whether to take an extra reference of the
   * GVariant or not (not taking one could destroy the GVariant with the
   * wrapper).
   */
  explicit Variant(GVariant* castitem, bool take_a_reference = false)
  : VariantBase(castitem, take_a_reference)
  {}

  /** Gets the Glib::VariantType.
   * @return The Glib::VariantType.
   */
  static const VariantType& variant_type() G_GNUC_CONST;

  /** Creates a new Glib::Variant<long long>.
   * @param data The value of the new Glib::Variant<long long>.
   * @return The new Glib::Variant<long long>.
   */
  static Variant<long long> create(long long data);

  /** Gets the value of the Glib::Variant<long long>.
   * @return The long long value of the Glib::Variant<long long>.
   */
  long long get() const;
};


/** Specialization of Glib::Variant containing a unsigned long long type.
 *
 * @ingroup Variant
 */
template <>
class GLIBMM_API Variant<unsigned long long> : public VariantBase
{
public:
  using CType = guint64;

  /// Default constructor.
  Variant()
  : VariantBase()
  {}

  /** GVariant constructor.
   * @param castitem The GVariant to wrap.
   * @param take_a_reference Whether to take an extra reference of the
   * GVariant or not (not taking one could destroy the GVariant with the
   * wrapper).
   */
  explicit Variant(GVariant* castitem, bool take_a_reference = false)
  : VariantBase(castitem, take_a_reference)
  {}

  /** Gets the Glib::VariantType.
   * @return The Glib::VariantType.
   */
  static const VariantType& variant_type() G_GNUC_CONST;

  /** Creates a new Glib::Variant<unsigned long long>.
   * @param data The value of the new Glib::Variant<unsigned long long>.
   * @return The new Glib::Variant<unsigned long long>.
   */
  static Variant<unsigned long long> create(unsigned long long data);

  /** Gets the value of the Glib::Variant<unsigned long long>.
   * @return The unsigned long long value of the Glib::Variant<unsigned long long>.
   */
  unsigned long long get() const;
};

#else

/** Specialization of Glib::Variant containing a gint64 type.
 *
 * @ingroup Variant
 */
template <>
class GLIBMM_API Variant<gint64> : public VariantBase
{
public:
  using CType = gint64;

  /// Default constructor.
  Variant()
  : VariantBase()
  {}

  /** GVariant constructor.
   * @param castitem The GVariant to wrap.
   * @param take_a_reference Whether to take an extra reference of the
   * GVariant or not (not taking one could destroy the GVariant with the
   * wrapper).
   */
  explicit Variant(GVariant* castitem, bool take_a_reference = false)
  : VariantBase(castitem, take_a_reference)
  {}

  /** Gets the Glib::VariantType.
   * @return The Glib::VariantType.
   */
  static const VariantType& variant_type() G_GNUC_CONST;

  /** Creates a new Glib::Variant<gint64>.
   * @param data The value of the new Glib::Variant<gint64>.
   * @return The new Glib::Variant<gint64>.
   */
  static Variant<gint64> create(gint64 data);

  /** Gets the value of the Glib::Variant<gint64>.
   * @return The gint64 value of the Glib::Variant<gint64>.
   */
  gint64 get() const;
};


/** Specialization of Glib::Variant containing a guint64 type.
 *
 * @ingroup Variant
 */
template <>
class GLIBMM_API Variant<guint64> : public VariantBase
{
public:
  using CType = guint64;

  /// Default constructor.
  Variant()
  : VariantBase()
  {}

  /** GVariant constructor.
   * @param castitem The GVariant to wrap.
   * @param take_a_reference Whether to take an extra reference of the
   * GVariant or not (not taking one could destroy the GVariant with the
   * wrapper).
   */
  explicit Variant(GVariant* castitem, bool take_a_reference = false)
  : VariantBase(castitem, take_a_reference)
  {}

  /** Gets the Glib::VariantType.
   * @return The Glib::VariantType.
   */
  static const VariantType& variant_type() G_GNUC_CONST;

  /** Creates a new Glib::Variant<guint64>.
   * @param data The value of the new Glib::Variant<guint64>.
   * @return The new Glib::Variant<guint64>.
   */
  static Variant<guint64> create(guint64 data);

  /** Gets the value of the Glib::Variant<guint64>.
   * @return The guint64 value of the Glib::Variant<guint64>.
   */
  guint64 get() const;
};

#endif


/** Specialization of Glib::Variant containing a DBusHandle type.
 *
 * @ingroup Variant
 */
template <>
class GLIBMM_API Variant<DBusHandle> : public VariantBase
{
public:
  using CType = gint32;

  /// Default constructor.
  Variant()
  : VariantBase()
  {}

  /** GVariant constructor.
   * @param castitem The GVariant to wrap.
   * @param take_a_reference Whether to take an extra reference of the
   * GVariant or not (not taking one could destroy the GVariant with the
   * wrapper).
   */
  explicit Variant(GVariant* castitem, bool take_a_reference = false)
  : VariantBase(castitem, take_a_reference)
  {}

  /** Gets the Glib::VariantType.
   * @return The Glib::VariantType.
   */
  static const VariantType& variant_type() G_GNUC_CONST;

  /** Creates a new Glib::Variant<DBusHandle>.
   * @param data The value of the new Glib::Variant<DBusHandle>.
   * @return The new Glib::Variant<DBusHandle>.
   */
  static Variant<DBusHandle> create(DBusHandle data);

  /** Gets the value of the Glib::Variant<DBusHandle>.
   * @return The DBusHandle value of the Glib::Variant<DBusHandle>.
   */
  DBusHandle get() const;
};


/** Specialization of Glib::Variant containing a double type.
 *
 * @ingroup Variant
 */
template <>
class GLIBMM_API Variant<double> : public VariantBase
{
public:
  using CType = gdouble;

  /// Default constructor.
  Variant()
  : VariantBase()
  {}

  /** GVariant constructor.
   * @param castitem The GVariant to wrap.
   * @param take_a_reference Whether to take an extra reference of the
   * GVariant or not (not taking one could destroy the GVariant with the
   * wrapper).
   */
  explicit Variant(GVariant* castitem, bool take_a_reference = false)
  : VariantBase(castitem, take_a_reference)
  {}

  /** Gets the Glib::VariantType.
   * @return The Glib::VariantType.
   */
  static const VariantType& variant_type() G_GNUC_CONST;

  /** Creates a new Glib::Variant<double>.
   * @param data The value of the new Glib::Variant<double>.
   * @return The new Glib::Variant<double>.
   */
  static Variant<double> create(double data);

  /** Gets the value of the Glib::Variant<double>.
   * @return The double value of the Glib::Variant<double>.
   */
  double get() const;
};


} // namespace Glib
