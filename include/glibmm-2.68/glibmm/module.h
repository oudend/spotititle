// Generated by gmmproc 2.78.1 -- DO NOT MODIFY!
#ifndef _GLIBMM_MODULE_H
#define _GLIBMM_MODULE_H


/* Copyright (C) 2002 The gtkmm Development Team
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


#include <glibmmconfig.h>
#include <string>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
extern "C" { typedef struct _GModule GModule; }
#endif

namespace Glib
{

//TODO: Replace get_last_error() with exceptions?
//Provide operator()?

/** Dynamic loading of modules.
 *
 * These functions provide a portable way to dynamically load object
 * files (commonly known as 'plug-ins'). The current implementation
 * supports all systems that provide an implementation of dlopen()
 * (e.g. Linux/Sun), as well as HP-UX via its shl_load() mechanism,
 * and Windows platforms via DLLs.
 */
class GLIBMM_API Module
{
  public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  using CppObjectType = Module;
  using BaseObjectType = GModule;
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

private:

  
public:

  /** @addtogroup glibmmEnums glibmm Enums and Flags */

  /** 
   *  @var Flags LAZY
   * Specifies that symbols are only resolved when
   * needed. The default action is to bind all symbols when the module
   * is loaded.
   * 
   *  @var Flags LOCAL
   * Specifies that symbols in the module should
   * not be added to the global name space. The default action on most
   * platforms is to place symbols in the module in the global name space,
   * which may cause conflicts with existing symbols.
   * 
   *  @var Flags MASK
   * Mask for all flags.
   * 
   *  @enum Flags
   * 
   * Flags passed to g_module_open().
   * Note that these flags are not supported on all platforms.
   *
   * @ingroup glibmmEnums
   * @par Bitwise operators:
   * <tt>Module::Flags operator|(Module::Flags, Module::Flags)</tt><br>
   * <tt>Module::Flags operator&(Module::Flags, Module::Flags)</tt><br>
   * <tt>Module::Flags operator^(Module::Flags, Module::Flags)</tt><br>
   * <tt>Module::Flags operator~(Module::Flags)</tt><br>
   * <tt>Module::Flags& operator|=(Module::Flags&, Module::Flags)</tt><br>
   * <tt>Module::Flags& operator&=(Module::Flags&, Module::Flags)</tt><br>
   * <tt>Module::Flags& operator^=(Module::Flags&, Module::Flags)</tt><br>
   */
  enum class Flags
  {
    LAZY = 1 << 0,
    LOCAL = 1 << 1,
    MASK = 0x03
  };


  /** Opens a module.
   *
   * If the module has already been opened, its reference count
   * is incremented. If not, the module is searched in the following order:
   *
   * 1. If @a file_name exists as a regular file, it is used as-is; else
   * 2. If @a file_name doesn't have the correct suffix and/or prefix for the
   *    platform, then possible suffixes and prefixes will be added to the
   *    basename till a file is found and whatever is found will be used; else
   * 3. If @a file_name doesn't have the ".la"-suffix, ".la" is appended. Either
   *    way, if a matching .la file exists (and is a libtool archive) the
   *    libtool archive is parsed to find the actual file name, and that is
   *    used.
   *
   * At the end of all this, we would have a file path that we can access on
   * disk, and it is opened as a module. If not, @a file_name is opened as
   * a module verbatim in the hopes that the system implementation will somehow
   * be able to access it.
   *
   * Use operator bool() to see whether the operation succeeded. For instance,
   * @code
   * Glib::Module module("plugins/helloworld");
   * if(module)
   * {
   *   void* func = nullptr;
   *   bool found = get_symbol("some_function", func);
   * }
   * @endcode
   *
   * @param file_name The name or path to the file containing the module,
   *                  or an empty string to obtain a module representing
   *                  the main program itself.
   * @param flags The flags used for opening the module.
   */
  explicit Module(const std::string& file_name, Flags flags = Flags(0));

  Module(const Module&) = delete;
  Module& operator=(const Module&) = delete;

  Module(Module&& other) noexcept;
  Module& operator=(Module&& other) noexcept;

  /** Close a module. The module will be removed from memory, unless
   * <tt>make_resident</tt> has been called.
   */
  virtual ~Module();

  /** Check whether the module was found.
   */
  explicit operator bool() const;

  
  /** Checks if modules are supported on the current platform.
   * @returns true if available, false otherwise
   */
  static bool get_supported();

  
  /** Ensures that a module will never be unloaded. Any calls to the
   * Glib::Module destructor will not unload the module.
   */
  void make_resident();

  
  /** Gets a string describing the last module error.
   * @returns The error string
   */
  static std::string get_last_error();

  
  /** Gets a symbol pointer from the module.
   * @param symbol_name The name of the symbol to lookup
   * @param symbol A pointer to set to the symbol
   * @returns True if the symbol was found, false otherwise.
   */
  bool get_symbol(const std::string& symbol_name, void*& symbol) const;

  
  /** Get the name of the module.
   * @returns The name of the module
   */
  std::string get_name() const;

  
#ifndef GLIBMM_DISABLE_DEPRECATED

  /** A portable way to build the filename of a module. The
   * platform-specific prefix and suffix are added to the filename, if
   * needed, and the result is added to the directory, using the
   * correct separator character.
   *
   * The directory should specify the directory where the module can
   * be found. It can be an empty string to indicate that the
   * module is in a standard platform-specific directory, though this
   * is not recommended since the wrong module may be found.
   *
   * For example, calling <tt>build_path()</tt> on a Linux
   * system with a directory of <tt>/lib</tt> and a module_name of
   * "mylibrary" will return <tt>/lib/libmylibrary.so</tt>. On a
   * Windows system, using <tt>\\Windows</tt> as the directory it will
   * return <tt>\\Windows\\mylibrary.dll</tt>.
   *
   * @param directory The directory the module is in
   * @param module_name The name of the module
   * @returns The system-specific filename of the module
   *
   * @deprecated 2.76: You will get the wrong results most of the time.
   *   Use the constructor instead with @a module_name as the
   *   basename of the file_name argument.
   */
  static std::string build_path(const std::string& directory, const std::string& module_name);
#endif // GLIBMM_DISABLE_DEPRECATED


  GModule*       gobj()       { return gobject_; }
  const GModule* gobj() const { return gobject_; }

protected:
  GModule* gobject_;


};

} // namespace Glib


namespace Glib
{

/** @ingroup glibmmEnums */
inline Module::Flags operator|(Module::Flags lhs, Module::Flags rhs)
  { return static_cast<Module::Flags>(static_cast<unsigned>(lhs) | static_cast<unsigned>(rhs)); }

/** @ingroup glibmmEnums */
inline Module::Flags operator&(Module::Flags lhs, Module::Flags rhs)
  { return static_cast<Module::Flags>(static_cast<unsigned>(lhs) & static_cast<unsigned>(rhs)); }

/** @ingroup glibmmEnums */
inline Module::Flags operator^(Module::Flags lhs, Module::Flags rhs)
  { return static_cast<Module::Flags>(static_cast<unsigned>(lhs) ^ static_cast<unsigned>(rhs)); }

/** @ingroup glibmmEnums */
inline Module::Flags operator~(Module::Flags flags)
  { return static_cast<Module::Flags>(~static_cast<unsigned>(flags)); }

/** @ingroup glibmmEnums */
inline Module::Flags& operator|=(Module::Flags& lhs, Module::Flags rhs)
  { return (lhs = static_cast<Module::Flags>(static_cast<unsigned>(lhs) | static_cast<unsigned>(rhs))); }

/** @ingroup glibmmEnums */
inline Module::Flags& operator&=(Module::Flags& lhs, Module::Flags rhs)
  { return (lhs = static_cast<Module::Flags>(static_cast<unsigned>(lhs) & static_cast<unsigned>(rhs))); }

/** @ingroup glibmmEnums */
inline Module::Flags& operator^=(Module::Flags& lhs, Module::Flags rhs)
  { return (lhs = static_cast<Module::Flags>(static_cast<unsigned>(lhs) ^ static_cast<unsigned>(rhs))); }
} // namespace Glib


#endif /* _GLIBMM_MODULE_H */
