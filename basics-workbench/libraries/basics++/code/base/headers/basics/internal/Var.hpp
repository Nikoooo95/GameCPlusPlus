/*
 * VAR
 * Copyright © 2011+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712190125
 */

#ifndef BASICS_VAR_HEADER
#define BASICS_VAR_HEADER

    #include <basics/fnv>

    namespace basics
    {

        class Var
        {
        public:

            class Type
            {
            public:

                typedef unsigned Id;
                typedef byte     Blob[8];

            public:

                struct Info
                {
                    const Id            id;
                    const char        * name;
                    const void (Type::* clear) ();
                };

                const Info * info;

            protected:

                Blob  blob;

            public:

                Type(const Info * const info) : info(info)
                {
                }

               ~Type()
                {
                    if (info->clear) (this->*(info->clear)) ();
                }

            protected:

                template< typename TYPE >
                TYPE & data ()
                {
                    static_assert(sizeof(TYPE) <= sizeof(blob), "Error: TYPE is bigger than blob.");
                    return reinterpret_cast< TYPE & >(*blob);
                }

                template< typename TYPE >
                const TYPE & data () const
                {
                    static_assert(sizeof(TYPE) <= sizeof(blob), "Error: TYPE is bigger than blob.");
                    return reinterpret_cast< const TYPE & >(*blob);
                }
            };

            Type var;

        public:

            Var();

            template< typename TYPE >
            bool is () const
            {
                return var.info->id == TYPE::id;
            }

            template< typename TYPE >
            TYPE * as ()
            {
                return var.info->id == TYPE::id ? static_cast< TYPE * >(&var) : nullptr;
            }

            Var & operator = (const Var & other)
            {
                this->var = other.var;
                return *this;
            }

            Var & operator = (const bool );
            Var & operator = (const float );

        };

        namespace var
        {

            class Bool : public Var::Type
            {
            public:

                static constexpr Id id = FNV(Bool);

            private:

                static const Info info;

            public:

                Bool() : Type(&info)
                {
                }

                Bool & operator = (const bool value)
                {
                    return data< bool > () = value, *this;
                }

                operator const bool & () const
                {
                    return data< bool > ();
                }
            };

            class Float : public Var::Type
            {
            public:

                static constexpr Id id = FNV(Float);

            private:

                static const Info info;

            public:

                Float() : Type(&info)
                {
                }

                Float(float x) : Type(&info)
                {
                    *this = x;
                }

                Float & operator = (const float value)
                {
                    return data< float > () = value, *this;
                }

                operator const float & () const
                {
                    return data< float > ();
                }
            };

            // Tipos simples:

            class Void;
            class Bool;
            class Byte;
            class Word;
            class Char;
            class WChar;
            class Int;
            class Unsigned;
            class Int8;
            class Int16;
            class Int32;
            class Int64;
            class UInt8;
            class UInt16;
            class UInt32;
            class UInt64;
            class Float;
            class Double;

            // Tipos derivados:

            class Pointer;              // template< typename T > class Pointer : public Type...

            // Tipos complejos:

            class String;
            class Array;
            class Map;

        }

        inline Var::Var() : var(var::Bool())
        {
        }

        inline Var & Var::operator = (const bool )
        {
            this->var = var::Bool();
            return *this;
        }

        inline Var & Var::operator = (const float x)
        {
            this->var = var::Float(x);
            return *this;
        }

    }

#endif
