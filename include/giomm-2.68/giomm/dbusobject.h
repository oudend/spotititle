// Generated by gmmproc 2.78.1 -- DO NOT MODIFY!
#ifndef _GIOMM_DBUSOBJECT_H
#define _GIOMM_DBUSOBJECT_H

#include <giommconfig.h>


#include <glibmm/ustring.h>
#include <sigc++/sigc++.h>

/* Copyright (C) 2012 The giomm Development Team
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


#include <glibmm/interface.h>


#ifndef DOXYGEN_SHOULD_SKIP_THIS
typedef struct _GDBusObjectIface GDBusObjectIface;
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#ifndef DOXYGEN_SHOULD_SKIP_THIS
using GDBusObject = struct _GDBusObject;
using GDBusObjectClass = struct _GDBusObjectClass;
#endif /* DOXYGEN_SHOULD_SKIP_THIS */


#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace Gio
{

namespace DBus
{ class GIOMM_API Object_Class; } // namespace DBus

} // namespace Gio
#endif // DOXYGEN_SHOULD_SKIP_THIS

namespace Gio
{

namespace DBus
{

class GIOMM_API Interface;


/** Object - Base type for D-Bus objects.
 * The Object type is the base type for D-Bus objects on both the service side
 * (see ObjectSkeleton) and the client side (see ObjectProxy). It is
 * essentially just a container of interfaces.
 * @newin{2,34}
 */


class GIOMM_API Object : public Glib::Interface
{
  
#ifndef DOXYGEN_SHOULD_SKIP_THIS

public:
  using CppObjectType = Object;
  using CppClassType = Object_Class;
  using BaseObjectType = GDBusObject;
  using BaseClassType = GDBusObjectIface;

  // noncopyable
  Object(const Object&) = delete;
  Object& operator=(const Object&) = delete;

private:
  friend class Object_Class;
  static CppClassType object_class_;

#endif /* DOXYGEN_SHOULD_SKIP_THIS */
protected:
  /**
   * You should derive from this class to use it.
   */
  Object();

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  /** Called by constructors of derived classes. Provide the result of
   * the Class init() function to ensure that it is properly
   * initialized.
   *
   * @param interface_class The Class object for the derived type.
   */
  explicit Object(const Glib::Interface_Class& interface_class);

public:
  // This is public so that C++ wrapper instances can be
  // created for C instances of unwrapped types.
  // For instance, if an unexpected C type implements the C interface.
  explicit Object(GDBusObject* castitem);

protected:
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

public:

  Object(Object&& src) noexcept;
  Object& operator=(Object&& src) noexcept;

  ~Object() noexcept override;

  static void add_interface(GType gtype_implementer);

  /** Get the GType for this class, for use with the underlying GObject type system.
   */
  static GType get_type()      G_GNUC_CONST;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  static GType get_base_type() G_GNUC_CONST;
#endif

  ///Provides access to the underlying C GObject.
  GDBusObject*       gobj()       { return reinterpret_cast<GDBusObject*>(gobject_); }

  ///Provides access to the underlying C GObject.
  const GDBusObject* gobj() const { return reinterpret_cast<GDBusObject*>(gobject_); }

private:


public:
  
  /** Gets the object path for @a object.
   * 
   * @newin{2,30}
   * 
   * @return A string owned by @a object. Do not free.
   */
  Glib::ustring get_object_path() const;

 
  /** Gets the D-Bus interfaces associated with @a object.
   * 
   * @newin{2,30}
   * 
   * @return A list of DBusInterface instances.
   */
  std::vector<Glib::RefPtr<Gio::DBus::Interface>> get_interfaces();

 
  /** Gets the D-Bus interfaces associated with @a object.
   * 
   * @newin{2,30}
   * 
   * @return A list of DBusInterface instances.
   */
  std::vector<Glib::RefPtr<const Gio::DBus::Interface>> get_interfaces() const;

  
  /** Gets the D-Bus interface with name @a interface_name associated with
   *  @a object, if any.
   * 
   * @newin{2,30}
   * 
   * @param interface_name A D-Bus interface name.
   * @return <tt>nullptr</tt> if not found, otherwise a
   * DBusInterface that must be freed with Glib::object_unref().
   */
  Glib::RefPtr<Gio::DBus::Interface> get_interface(const Glib::ustring& interface_name);
  
  /** Gets the D-Bus interface with name @a interface_name associated with
   *  @a object, if any.
   * 
   * @newin{2,30}
   * 
   * @param interface_name A D-Bus interface name.
   * @return <tt>nullptr</tt> if not found, otherwise a
   * DBusInterface that must be freed with Glib::object_unref().
   */
  Glib::RefPtr<const Gio::DBus::Interface> get_interface(const Glib::ustring& interface_name) const;

 
  // The parameter name 'interface' can cause compilation errors with MinGW.
  // See https://bugzilla.gnome.org/show_bug.cgi?id=735137
  // The parameter name in glib is 'interface_'.
  
  /**
   * @par Slot Prototype:
   * <tt>void on_my_%interface_added(const Glib::RefPtr<Gio::DBus::Interface>& iface)</tt>
   *
   * Flags: Run Last
   *
   * Emitted when @a iface is added to @a object.
   * 
   * @newin{2,30}
   * 
   * @param iface The DBusInterface that was added.
   */

  Glib::SignalProxy<void(const Glib::RefPtr<Gio::DBus::Interface>&)> signal_interface_added();

  
  /**
   * @par Slot Prototype:
   * <tt>void on_my_%interface_removed(const Glib::RefPtr<Gio::DBus::Interface>& iface)</tt>
   *
   * Flags: Run Last
   *
   * Emitted when @a iface is removed from @a object.
   * 
   * @newin{2,30}
   * 
   * @param iface The DBusInterface that was removed.
   */

  Glib::SignalProxy<void(const Glib::RefPtr<Gio::DBus::Interface>&)> signal_interface_removed();


protected:
 

    virtual Glib::ustring get_object_path_vfunc() const;


    virtual std::vector<Glib::RefPtr<Gio::DBus::Interface>> get_interfaces_vfunc() const;


  // Can't use refreturn_ctype here. It generates a call to Glib::unwrap_copy(), which
  // can't be used for Glib::Interface classes. They have no gobj_copy() method.
 

    virtual Glib::RefPtr<Gio::DBus::Interface> get_interface_vfunc(const Glib::ustring& interface_name) const;


public:

public:
  //C++ methods used to invoke GTK+ virtual functions:

protected:
  //GTK+ Virtual Functions (override these to change behaviour):

  //Default Signal Handlers::
  /// This is a default handler for the signal signal_interface_added().
  virtual void on_interface_added(const Glib::RefPtr<Gio::DBus::Interface>& iface);
  /// This is a default handler for the signal signal_interface_removed().
  virtual void on_interface_removed(const Glib::RefPtr<Gio::DBus::Interface>& iface);


};

} // namespace DBus

} // namespace Gio


namespace Glib
{
  /** A Glib::wrap() method for this object.
   *
   * @param object The C instance.
   * @param take_copy False if the result should take ownership of the C instance. True if it should take a new copy or ref.
   * @result A C++ instance that wraps this C instance.
   *
   * @relates Gio::DBus::Object
   */
  GIOMM_API
  Glib::RefPtr<Gio::DBus::Object> wrap(GDBusObject* object, bool take_copy = false);

} // namespace Glib


#endif /* _GIOMM_DBUSOBJECT_H */

