// Generated by gmmproc 2.74.0 -- DO NOT MODIFY!
#ifndef _PANGOMM_COVERAGE_H
#define _PANGOMM_COVERAGE_H

#include <pangommconfig.h>


#include <glibmm/ustring.h>
#include <sigc++/sigc++.h>

/* coverage.h
 *
 * Copyright (C) 1998-1999 The gtkmm Development Team
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
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


#include <glibmm/value.h>
#include <pango/pango.h>


namespace Pango
{

/** A Pango::Coverage represents a map from ISO-10646 character point to Pango::Coverage::Level.
 * It is often necessary in pango to determine if a particular font can represent a particular character,
 * and also how well it can represent that character. Pango::Coverage holds this information.
 */
class PANGOMM_API Coverage final
{
  public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  using CppObjectType = Coverage;
  using BaseObjectType = PangoCoverage;
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

  static Glib::RefPtr<Coverage> create();

  /** Increment the reference count for this object.
   * You should never need to do this manually - use the object via a RefPtr instead.
   */
  void reference()   const;

  /** Decrement the reference count for this object.
   * You should never need to do this manually - use the object via a RefPtr instead.
   */
  void unreference() const;

  ///Provides access to the underlying C instance.
  PangoCoverage*       gobj();

  ///Provides access to the underlying C instance.
  const PangoCoverage* gobj() const;

  ///Provides access to the underlying C instance. The caller is responsible for unrefing it. Use when directly setting fields in structs.
  PangoCoverage* gobj_copy() const;

  Coverage() = delete;

  // noncopyable
  Coverage(const Coverage&) = delete;
  Coverage& operator=(const Coverage&) = delete;

protected:
  // Do not derive this.  Pango::Coverage can neither be constructed nor deleted.

  void operator delete(void*, std::size_t);

private:

  
   //pango ref doc: "This function may now be unecessary since we refcount the structure. Mail otaylor redhat.com if you use it."
  
public:
  /** @addtogroup pangommEnums pangomm Enums and Flags */

  /** 
   *  @var Level NONE
   * The character is not representable with
   * the font.
   * 
   *  @var Level FALLBACK
   * The character is represented in a
   * way that may be comprehensible but is not the correct
   * graphical form. For instance, a Hangul character represented
   * as a a sequence of Jamos, or a Latin transliteration of a
   * Cyrillic word.
   * 
   *  @var Level APPROXIMATE
   * The character is represented as
   * basically the correct graphical form, but with a stylistic
   * variant inappropriate for the current script.
   * 
   *  @var Level EXACT
   * The character is represented as the
   * correct graphical form.
   * 
   *  @enum Level
   * 
   * `Pango::CoverageLevel` is used to indicate how well a font can
   * represent a particular Unicode character for a particular script.
   * 
   * Since 1.44, only Pango::Coverage::Level::NONE and Pango::Coverage::Level::EXACT
   * will be returned.
   *
   * @ingroup pangommEnums
   */
  enum class Level
  {
    NONE,
    FALLBACK,
    APPROXIMATE,
    EXACT
  };


  //_WRAP_METHOD(Glib::RefPtr<Coverage> copy() const, pango_coverage_copy) //see above

  
  /** Determine whether a particular index is covered by @a coverage.
   * 
   * @param index The index to check.
   * @return The coverage level of @a coverage for character @a index.
   */
  Level get(int index) const;
  
  /** Modify a particular index within @a coverage
   * 
   * @param index The index to modify.
   * @param level The new level for @a index.
   */
  void set(int index, Level level);


};

} // namespace Pango


#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace Glib
{

template <>
class PANGOMM_API Value<Pango::Coverage::Level> : public Glib::Value_Enum<Pango::Coverage::Level>
{
public:
  static GType value_type() G_GNUC_CONST;
};

} // namespace Glib
#endif /* DOXYGEN_SHOULD_SKIP_THIS */


namespace Glib
{

/** A Glib::wrap() method for this object.
 *
 * @param object The C instance.
 * @param take_copy False if the result should take ownership of the C instance. True if it should take a new copy or ref.
 * @result A C++ instance that wraps this C instance.
 *
 * @relates Pango::Coverage
 */
PANGOMM_API
Glib::RefPtr<Pango::Coverage> wrap(PangoCoverage* object, bool take_copy = false);

} // namespace Glib


#endif /* _PANGOMM_COVERAGE_H */
