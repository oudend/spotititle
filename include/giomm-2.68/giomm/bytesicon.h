// Generated by gmmproc 2.78.1 -- DO NOT MODIFY!
#ifndef _GIOMM_BYTESICON_H
#define _GIOMM_BYTESICON_H


#include <glibmm/ustring.h>
#include <sigc++/sigc++.h>

/* Copyright (C) 2022 The gtkmm Development Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <glibmm/object.h>
#include <giomm/icon.h>
#include <giomm/loadableicon.h>
#include <glibmm/bytes.h>


#ifndef DOXYGEN_SHOULD_SKIP_THIS
using GBytesIcon = struct _GBytesIcon;
using GBytesIconClass = struct _GBytesIconClass;
#endif /* DOXYGEN_SHOULD_SKIP_THIS */


#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace Gio
{ class GIOMM_API BytesIcon_Class; } // namespace Gio
#endif //DOXYGEN_SHOULD_SKIP_THIS

namespace Gio
{

/** An icon stored in memory as a Glib::Bytes.
 *
 * %Gio::BytesIcon specifies an image held in memory in a common format
 * (usually png) to be used as icon.
 *
 * @see Icon, LoadableIcon, Glib::Bytes
 * @newin{2,76}
 */

class GIOMM_API BytesIcon : public Glib::Object, public Icon, public LoadableIcon
{
  
#ifndef DOXYGEN_SHOULD_SKIP_THIS

public:
  using CppObjectType = BytesIcon;
  using CppClassType = BytesIcon_Class;
  using BaseObjectType = GBytesIcon;
  using BaseClassType = GBytesIconClass;

  // noncopyable
  BytesIcon(const BytesIcon&) = delete;
  BytesIcon& operator=(const BytesIcon&) = delete;

private:  friend class BytesIcon_Class;
  static CppClassType bytesicon_class_;

protected:
  explicit BytesIcon(const Glib::ConstructParams& construct_params);
  explicit BytesIcon(GBytesIcon* castitem);

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

public:

  BytesIcon(BytesIcon&& src) noexcept;
  BytesIcon& operator=(BytesIcon&& src) noexcept;

  ~BytesIcon() noexcept override;

  /** Get the GType for this class, for use with the underlying GObject type system.
   */
  static GType get_type()      G_GNUC_CONST;

#ifndef DOXYGEN_SHOULD_SKIP_THIS


  static GType get_base_type() G_GNUC_CONST;
#endif

  ///Provides access to the underlying C GObject.
  GBytesIcon*       gobj()       { return reinterpret_cast<GBytesIcon*>(gobject_); }

  ///Provides access to the underlying C GObject.
  const GBytesIcon* gobj() const { return reinterpret_cast<GBytesIcon*>(gobject_); }

  ///Provides access to the underlying C instance. The caller is responsible for unrefing it. Use when directly setting fields in structs.
  GBytesIcon* gobj_copy();

private:

  
protected:
    explicit BytesIcon(const Glib::RefPtr<const Glib::Bytes>& bytes);


public:
  /** Creates a new icon for a Glib::Bytes.
   *
   * This cannot fail, but loading and interpreting the bytes may fail later on
   * (for example, if Gio::LoadableIcon::load() is called) if the image is invalid.
   *
   * @newin{2,76}
   *
   * @param bytes A Glib::Bytes.
   * @return A Icon for the given @a bytes.
   */
  
  static Glib::RefPtr<BytesIcon> create(const Glib::RefPtr<const Glib::Bytes>& bytes);


  /** Gets the Bytes associated with the given @a icon.
   * 
   * @newin{2,76}
   * 
   * @return A Bytes.
   */
  Glib::RefPtr<const Glib::Bytes> get_bytes() const;

  /** The bytes containing the icon.
   * 
   * @newin{2,76}
   *
   * @return A PropertyProxy_ReadOnly that allows you to get the value of the property,
   * or receive notification when the value of the property changes.
   */
  Glib::PropertyProxy_ReadOnly< Glib::RefPtr<Glib::Bytes> > property_bytes() const;


public:

public:
  //C++ methods used to invoke GTK+ virtual functions:

protected:
  //GTK+ Virtual Functions (override these to change behaviour):

  //Default Signal Handlers::


};

} // namespace Gio


namespace Glib
{
  /** A Glib::wrap() method for this object.
   *
   * @param object The C instance.
   * @param take_copy False if the result should take ownership of the C instance. True if it should take a new copy or ref.
   * @result A C++ instance that wraps this C instance.
   *
   * @relates Gio::BytesIcon
   */
  GIOMM_API
  Glib::RefPtr<Gio::BytesIcon> wrap(GBytesIcon* object, bool take_copy = false);
}


#endif /* _GIOMM_BYTESICON_H */

