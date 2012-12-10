/**
 * @file  Column.h
 * @brief Encapsulation of a Column in a Row of the result.
 *
 * Copyright (c) 2012 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#pragma once

#include <sqlite3.h>
#include "Exception.h"

namespace SQLite
{


/**
 * @brief Encapsulation of a Column in a Row of the result.
 *
 * A Column is a particular field of SQLite data in the current row of result of the Statement.
 *
 * @todo mpStmtRefCount is thread unsafe !
 */
class Column
{
public:
    /**
     * @brief Compile and register the SQL query for the provided SQLite Database Connection
     */
    explicit Column(sqlite3* apSQLite, sqlite3_stmt* apStmt, unsigned int* apStmtRefCount, int aIndex) throw(); // nothrow
    /// Simple destructor
    virtual ~Column(void)                   throw(); // nothrow

    /// @brief copy constructor : only way to copy a valid instance
             Column(const Column& aOther)   throw(); // nothrow

    /// Return the integer value of the column.
    int             getInt   (void) const throw();
    /// Return the 64bits integer value of the column.
    sqlite3_int64   getInt64 (void) const throw();
    /// Return the double (64bits float) value of the column.
    double          getDouble(void) const throw();
    /// Return a pointer to the text value (NULL terminated string) of the column.
    /// Warning, the value pointed at is only valid while the statement is valid (ie. not finalized),
    /// thus you must copy it before using it beyond its scope (to a std::string for instance).
    const char*     getText  (void) const throw();

    /// Inline cast operator to int
    inline operator int() const
    {
        return getInt();
    }
    /// Inline cast operator to 64bits integer
    inline operator sqlite3_int64() const
    {
        return getInt64();
    }
    /// Inline cast operator to double
    inline operator double() const
    {
        return getDouble();
    }
    /// Inline cast operator to char*
    inline operator const char*() const
    {
        return getText();
    }
#ifdef __GNUC__
    // NOTE : the following is required by GCC to cast a Column result in a std::string
    // (error: conversion from ‘SQLite::Column’ to non-scalar type ‘std::string {aka std::basic_string<char>}’ requested)
    // but is not working under Microsoft Visual Studio 2010 and 2012
    // (error C2440: 'initializing' : cannot convert from 'SQLite::Column' to 'std::basic_string<_Elem,_Traits,_Ax>'
    //  [...] constructor overload resolution was ambiguous)
    /// Inline cast operator to std::string
    inline operator const std::string() const
    {
        return getText();
    }
#endif

private:
    // Forbid default constructor and assignment operator (no implementation)
    // so that there is no way of having a Column instance not initialized
    Column(void);
    Column& operator=(const Column&);

private:
    sqlite3*        mpSQLite;       //!< Pointer to SQLite Database Connection Handle
    sqlite3_stmt*   mpStmt;         //!< Pointer to SQLite Statement Object
    unsigned int*   mpStmtRefCount; //!< Pointer to the heap allocated reference counter of the sqlite3_stmt (shared with the Statement object)
    int             mIndex;         //!< Index of the column in the row of result
};

/// Standard std::ostream inserter
std::ostream& operator<<(std::ostream &stream, const Column& column);

}  // namespace SQLite