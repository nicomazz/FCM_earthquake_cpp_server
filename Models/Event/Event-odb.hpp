// This file was generated by ODB, object-relational mapping (ORM)
// compiler for C++.
//

#ifndef EVENT_ODB_HPP
#define EVENT_ODB_HPP

#include <odb/version.hxx>

#if (ODB_VERSION != 20400UL)
#error ODB runtime version mismatch
#endif

#include <odb/pre.hxx>

#include "Event.hpp"

#include <memory>
#include <cstddef>

#include <odb/core.hxx>
#include <odb/traits.hxx>
#include <odb/callback.hxx>
#include <odb/wrapper-traits.hxx>
#include <odb/pointer-traits.hxx>
#include <odb/container-traits.hxx>
#include <odb/no-op-cache-traits.hxx>
#include <odb/result.hxx>
#include <odb/simple-object-result.hxx>

#include <odb/details/unused.hxx>
#include <odb/details/shared-ptr.hxx>

namespace odb
{
  // Event
  //
  template <>
  struct class_traits< ::Event >
  {
    static const class_kind kind = class_object;
  };

  template <>
  class access::object_traits< ::Event >
  {
    public:
    typedef ::Event object_type;
    typedef ::Event* pointer_type;
    typedef odb::pointer_traits<pointer_type> pointer_traits;

    static const bool polymorphic = false;

    typedef long int id_type;

    static const bool auto_id = false;

    static const bool abstract = false;

    static id_type
    id (const object_type&);

    typedef
    no_op_pointer_cache_traits<pointer_type>
    pointer_cache_traits;

    typedef
    no_op_reference_cache_traits<object_type>
    reference_cache_traits;

    static void
    callback (database&, object_type&, callback_event);

    static void
    callback (database&, const object_type&, callback_event);
  };
}

#include <odb/details/buffer.hxx>

#include <odb/sqlite/version.hxx>
#include <odb/sqlite/forward.hxx>
#include <odb/sqlite/binding.hxx>
#include <odb/sqlite/sqlite-types.hxx>
#include <odb/sqlite/query.hxx>

namespace odb
{
  // Event
  //
  template <typename A>
  struct query_columns< ::Event, id_sqlite, A >
  {
    // id
    //
    typedef
    sqlite::query_column<
      sqlite::value_traits<
        long int,
        sqlite::id_integer >::query_type,
      sqlite::id_integer >
    id_type_;

    static const id_type_ id;

    // date
    //
    typedef
    sqlite::query_column<
      sqlite::value_traits<
        ::std::string,
        sqlite::id_text >::query_type,
      sqlite::id_text >
    date_type_;

    static const date_type_ date;

    // depthKm
    //
    typedef
    sqlite::query_column<
      sqlite::value_traits<
        double,
        sqlite::id_real >::query_type,
      sqlite::id_real >
    depthKm_type_;

    static const depthKm_type_ depthKm;

    // author
    //
    typedef
    sqlite::query_column<
      sqlite::value_traits<
        ::std::string,
        sqlite::id_text >::query_type,
      sqlite::id_text >
    author_type_;

    static const author_type_ author;

    // catalog
    //
    typedef
    sqlite::query_column<
      sqlite::value_traits<
        ::std::string,
        sqlite::id_text >::query_type,
      sqlite::id_text >
    catalog_type_;

    static const catalog_type_ catalog;

    // contributor
    //
    typedef
    sqlite::query_column<
      sqlite::value_traits<
        ::std::string,
        sqlite::id_text >::query_type,
      sqlite::id_text >
    contributor_type_;

    static const contributor_type_ contributor;

    // contributorId
    //
    typedef
    sqlite::query_column<
      sqlite::value_traits<
        ::std::string,
        sqlite::id_text >::query_type,
      sqlite::id_text >
    contributorId_type_;

    static const contributorId_type_ contributorId;

    // magType
    //
    typedef
    sqlite::query_column<
      sqlite::value_traits<
        ::std::string,
        sqlite::id_text >::query_type,
      sqlite::id_text >
    magType_type_;

    static const magType_type_ magType;

    // magnitude
    //
    typedef
    sqlite::query_column<
      sqlite::value_traits<
        double,
        sqlite::id_real >::query_type,
      sqlite::id_real >
    magnitude_type_;

    static const magnitude_type_ magnitude;

    // magAuthor
    //
    typedef
    sqlite::query_column<
      sqlite::value_traits<
        ::std::string,
        sqlite::id_text >::query_type,
      sqlite::id_text >
    magAuthor_type_;

    static const magAuthor_type_ magAuthor;

    // eventLocation
    //
    typedef
    sqlite::query_column<
      sqlite::value_traits<
        ::std::string,
        sqlite::id_text >::query_type,
      sqlite::id_text >
    eventLocation_type_;

    static const eventLocation_type_ eventLocation;

    // millis
    //
    typedef
    sqlite::query_column<
      sqlite::value_traits<
        long int,
        sqlite::id_integer >::query_type,
      sqlite::id_integer >
    millis_type_;

    static const millis_type_ millis;

    // lat
    //
    typedef
    sqlite::query_column<
      sqlite::value_traits<
        double,
        sqlite::id_real >::query_type,
      sqlite::id_real >
    lat_type_;

    static const lat_type_ lat;

    // lng
    //
    typedef
    sqlite::query_column<
      sqlite::value_traits<
        double,
        sqlite::id_real >::query_type,
      sqlite::id_real >
    lng_type_;

    static const lng_type_ lng;

    // isRealTimeReport
    //
    typedef
    sqlite::query_column<
      sqlite::value_traits<
        bool,
        sqlite::id_integer >::query_type,
      sqlite::id_integer >
    isRealTimeReport_type_;

    static const isRealTimeReport_type_ isRealTimeReport;
  };

  template <typename A>
  const typename query_columns< ::Event, id_sqlite, A >::id_type_
  query_columns< ::Event, id_sqlite, A >::
  id (A::table_name, "\"id\"", 0);

  template <typename A>
  const typename query_columns< ::Event, id_sqlite, A >::date_type_
  query_columns< ::Event, id_sqlite, A >::
  date (A::table_name, "\"date\"", 0);

  template <typename A>
  const typename query_columns< ::Event, id_sqlite, A >::depthKm_type_
  query_columns< ::Event, id_sqlite, A >::
  depthKm (A::table_name, "\"depthKm\"", 0);

  template <typename A>
  const typename query_columns< ::Event, id_sqlite, A >::author_type_
  query_columns< ::Event, id_sqlite, A >::
  author (A::table_name, "\"author\"", 0);

  template <typename A>
  const typename query_columns< ::Event, id_sqlite, A >::catalog_type_
  query_columns< ::Event, id_sqlite, A >::
  catalog (A::table_name, "\"catalog\"", 0);

  template <typename A>
  const typename query_columns< ::Event, id_sqlite, A >::contributor_type_
  query_columns< ::Event, id_sqlite, A >::
  contributor (A::table_name, "\"contributor\"", 0);

  template <typename A>
  const typename query_columns< ::Event, id_sqlite, A >::contributorId_type_
  query_columns< ::Event, id_sqlite, A >::
  contributorId (A::table_name, "\"contributorId\"", 0);

  template <typename A>
  const typename query_columns< ::Event, id_sqlite, A >::magType_type_
  query_columns< ::Event, id_sqlite, A >::
  magType (A::table_name, "\"magType\"", 0);

  template <typename A>
  const typename query_columns< ::Event, id_sqlite, A >::magnitude_type_
  query_columns< ::Event, id_sqlite, A >::
  magnitude (A::table_name, "\"magnitude\"", 0);

  template <typename A>
  const typename query_columns< ::Event, id_sqlite, A >::magAuthor_type_
  query_columns< ::Event, id_sqlite, A >::
  magAuthor (A::table_name, "\"magAuthor\"", 0);

  template <typename A>
  const typename query_columns< ::Event, id_sqlite, A >::eventLocation_type_
  query_columns< ::Event, id_sqlite, A >::
  eventLocation (A::table_name, "\"eventLocation\"", 0);

  template <typename A>
  const typename query_columns< ::Event, id_sqlite, A >::millis_type_
  query_columns< ::Event, id_sqlite, A >::
  millis (A::table_name, "\"millis\"", 0);

  template <typename A>
  const typename query_columns< ::Event, id_sqlite, A >::lat_type_
  query_columns< ::Event, id_sqlite, A >::
  lat (A::table_name, "\"lat\"", 0);

  template <typename A>
  const typename query_columns< ::Event, id_sqlite, A >::lng_type_
  query_columns< ::Event, id_sqlite, A >::
  lng (A::table_name, "\"lng\"", 0);

  template <typename A>
  const typename query_columns< ::Event, id_sqlite, A >::isRealTimeReport_type_
  query_columns< ::Event, id_sqlite, A >::
  isRealTimeReport (A::table_name, "\"isRealTimeReport\"", 0);

  template <typename A>
  struct pointer_query_columns< ::Event, id_sqlite, A >:
    query_columns< ::Event, id_sqlite, A >
  {
  };

  template <>
  class access::object_traits_impl< ::Event, id_sqlite >:
    public access::object_traits< ::Event >
  {
    public:
    struct id_image_type
    {
      long long id_value;
      bool id_null;

      std::size_t version;
    };

    struct image_type
    {
      // id
      //
      long long id_value;
      bool id_null;

      // date
      //
      details::buffer date_value;
      std::size_t date_size;
      bool date_null;

      // depthKm
      //
      double depthKm_value;
      bool depthKm_null;

      // author
      //
      details::buffer author_value;
      std::size_t author_size;
      bool author_null;

      // catalog
      //
      details::buffer catalog_value;
      std::size_t catalog_size;
      bool catalog_null;

      // contributor
      //
      details::buffer contributor_value;
      std::size_t contributor_size;
      bool contributor_null;

      // contributorId
      //
      details::buffer contributorId_value;
      std::size_t contributorId_size;
      bool contributorId_null;

      // magType
      //
      details::buffer magType_value;
      std::size_t magType_size;
      bool magType_null;

      // magnitude
      //
      double magnitude_value;
      bool magnitude_null;

      // magAuthor
      //
      details::buffer magAuthor_value;
      std::size_t magAuthor_size;
      bool magAuthor_null;

      // eventLocation
      //
      details::buffer eventLocation_value;
      std::size_t eventLocation_size;
      bool eventLocation_null;

      // millis
      //
      long long millis_value;
      bool millis_null;

      // lat
      //
      double lat_value;
      bool lat_null;

      // lng
      //
      double lng_value;
      bool lng_null;

      // isRealTimeReport
      //
      long long isRealTimeReport_value;
      bool isRealTimeReport_null;

      std::size_t version;
    };

    struct extra_statement_cache_type;

    using object_traits<object_type>::id;

    static id_type
    id (const image_type&);

    static bool
    grow (image_type&,
          bool*);

    static void
    bind (sqlite::bind*,
          image_type&,
          sqlite::statement_kind);

    static void
    bind (sqlite::bind*, id_image_type&);

    static bool
    init (image_type&,
          const object_type&,
          sqlite::statement_kind);

    static void
    init (object_type&,
          const image_type&,
          database*);

    static void
    init (id_image_type&, const id_type&);

    typedef sqlite::object_statements<object_type> statements_type;

    typedef sqlite::query_base query_base_type;

    static const std::size_t column_count = 15UL;
    static const std::size_t id_column_count = 1UL;
    static const std::size_t inverse_column_count = 0UL;
    static const std::size_t readonly_column_count = 0UL;
    static const std::size_t managed_optimistic_column_count = 0UL;

    static const std::size_t separate_load_column_count = 0UL;
    static const std::size_t separate_update_column_count = 0UL;

    static const bool versioned = false;

    static const char persist_statement[];
    static const char find_statement[];
    static const char update_statement[];
    static const char erase_statement[];
    static const char query_statement[];
    static const char erase_query_statement[];

    static const char table_name[];

    static void
    persist (database&, const object_type&);

    static pointer_type
    find (database&, const id_type&);

    static bool
    find (database&, const id_type&, object_type&);

    static bool
    reload (database&, object_type&);

    static void
    update (database&, const object_type&);

    static void
    erase (database&, const id_type&);

    static void
    erase (database&, const object_type&);

    static result<object_type>
    query (database&, const query_base_type&);

    static unsigned long long
    erase_query (database&, const query_base_type&);

    public:
    static bool
    find_ (statements_type&,
           const id_type*);

    static void
    load_ (statements_type&,
           object_type&,
           bool reload);
  };

  template <>
  class access::object_traits_impl< ::Event, id_common >:
    public access::object_traits_impl< ::Event, id_sqlite >
  {
  };

  // Event
  //
}

#include "Event-odb.ixx"

#include <odb/post.hxx>

#endif // EVENT_ODB_HPP
