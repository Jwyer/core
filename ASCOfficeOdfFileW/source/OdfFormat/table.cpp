#include "precompiled_cpodf.h"
#include "table.h"

#include <boost/make_shared.hpp>
#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>

#include <cpdoccore/odf/odf_document.h>

#include <cpdoccore/xml/simple_xml_writer.h>

#include "common_attlists.h"

namespace cpdoccore { 
namespace odf {

using xml::xml_char_wc;

const wchar_t * table_table_source::ns = L"table";
const wchar_t * table_table_source::name = L"table-source";

void table_table_source::add_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

// table:table
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_table::ns = L"table";
const wchar_t * table_table::name = L"table";

void table_table::add_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (L"table" == Ns && L"table-source" == Name)
    {
		CP_CREATE_ELEMENT(table_table_source_);
    } 
    else if ( (L"table" == Ns && L"table-column-group" == Name) ||
              (L"table" == Ns && L"table-columns" == Name) ||
              (L"table" == Ns && L"table-column" == Name) ||
              (L"table" == Ns && L"table-header-columns" == Name)
            )
    {
        table_columns_and_groups_.add_child_element(Ns, Name, getContext());
    }
    else if ((L"table" == Ns && L"table-row-group" == Name) ||
              (L"table" == Ns && L"table-rows" == Name) ||
              (L"table" == Ns && L"table-row" == Name) ||
              (L"table" == Ns && L"table-header-rows" == Name))
    {
        table_rows_and_groups_.add_child_element(Ns, Name, getContext());
    }
    else if CP_CHECK_NAME(L"table", L"shapes")
    {
  	    CP_CREATE_ELEMENT(table_shapes_);
    }
    else
        CP_NOT_APPLICABLE_ELM();
}


void table_table::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT( L"table:name", table_table_attlist_.table_name_);
			CP_XML_ATTR_OPT( L"table:style-name", table_table_attlist_.table_style_name_);
			CP_XML_ATTR_OPT( L"table:template-name", table_table_attlist_.table_template_name_);

			if (table_table_attlist_.table_protected_)
				CP_XML_ATTR_OPT( L"table:protection-key", table_table_attlist_.table_protection_key_); 
			
			if (table_table_attlist_.table_print_)
				CP_XML_ATTR_OPT( L"table:print-ranges", table_table_attlist_.table_print_ranges_);

		
			if (table_shapes_)table_shapes_->serialize(CP_XML_STREAM());
 			if (table_shapes_)table_shapes_->serialize(CP_XML_STREAM());
   
			table_columns_and_groups_.serialize(CP_XML_STREAM());
			table_rows_and_groups_.serialize(CP_XML_STREAM());
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// table:table-column
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_table_column::ns = L"table";
const wchar_t * table_table_column::name = L"table-column";

void table_table_column::add_child_element( const ::std::wstring & Ns, const ::std::wstring & Name, odf_conversion_context * Context)
{
    CP_NOT_APPLICABLE_ELM();
}

// table:table-columns
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_table_columns::ns = L"table";
const wchar_t * table_table_columns::name = L"table-columns";

void table_table_columns::add_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME(L"table", L"table-column")
    {
   	    CP_CREATE_ELEMENT(table_table_column_);
    }
    else
        CP_NOT_APPLICABLE_ELM();
}

// table:table-header-columns
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_table_header_columns::ns = L"table";
const wchar_t * table_table_header_columns::name = L"table-header-columns";

void table_table_header_columns::add_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME(L"table", L"table-column")
    {
		 CP_CREATE_ELEMENT(table_table_column_);
    }
    else
        CP_NOT_APPLICABLE_ELM();
}

// table-columns
//////////////////////////////////////////////////////////////////////////////////////////////////
void table_columns::add_child_element( const ::std::wstring & Ns, const ::std::wstring & Name, odf_conversion_context * Context)
{
    if CP_CHECK_NAME(L"table", L"table-columns")
    {
		CP_CREATE_ELEMENT_SIMPLE(table_table_columns_);
    }
    else if CP_CHECK_NAME(L"table", L"table-column")
    {
        CP_CREATE_ELEMENT_SIMPLE(table_table_column_);                
    } 
    else
        not_applicable_element(L"table-columns",  Ns, Name);
}

// table-columns-no-group
//////////////////////////////////////////////////////////////////////////////////////////////////
table_columns_no_group::table_columns_no_group() : was_header_(false) 
{
};

void table_columns_no_group::add_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name, odf_conversion_context * Context)
{
    if (CP_CHECK_NAME(L"table", L"table-columns") || CP_CHECK_NAME(L"table", L"table-column"))
    {
        if (!was_header_)
            table_columns_1_.add_child_element(Ns, Name, Context);
        else
            table_columns_2_.add_child_element( Ns, Name, Context);

    } 
    else if CP_CHECK_NAME(L"table", L"table-header-columns")
    {
        was_header_ = true;
		CP_CREATE_ELEMENT(table_table_header_columns_);
    }
    else
        not_applicable_element(L"table-columns-no-group", Ns, Name);
}

_CP_PTR(table_columns_no_group) table_columns_no_group::create()
{
    return boost::make_shared<table_columns_no_group>();
}




// table:table-column-group
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_table_column_group::ns = L"table";
const wchar_t * table_table_column_group::name = L"table-column-group";

void table_table_column_group::add_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    table_columns_and_groups_.add_child_element(Ns, Name, getContext());
}

// table-columns-and-groups
//////////////////////////////////////////////////////////////////////////////////////////////////

table_columns_and_groups::table_columns_and_groups()
{
}


void table_columns_and_groups::add_child_element(const ::std::wstring & Ns, const ::std::wstring & Name, odf_conversion_context * Context)
{
    if (CP_CHECK_NAME(L"table", L"table-column-group"))
    {
		CP_CREATE_ELEMENT_SIMPLE(content_);
    }
    else if (CP_CHECK_NAME(L"table", L"table-columns") ||
        CP_CHECK_NAME(L"table", L"table-column") ||
        CP_CHECK_NAME(L"table", L"table-header-columns"))
    {
        _CP_PTR(table_columns_no_group) elm = table_columns_no_group::create();
        elm->add_child_element(Ns, Name, Context);
        content_.push_back(elm);
    }
    else
        not_applicable_element(L"table-columns-and-groups", Ns, Name);

}

// table-table-cell-content
//////////////////////////////////////////////////////////////////////////////////////////////////

void table_table_cell_content::add_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name, odf_conversion_context * Context)
{
	CP_CREATE_ELEMENT_SIMPLE(text_content_);
}

// table:table-cell
// table-table-cell
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_table_cell::ns = L"table";
const wchar_t * table_table_cell::name = L"table-cell";

void table_table_cell::add_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    table_table_cell_content_.add_child_element(Ns, Name, getContext());
}


// table:covered-table-cell
// table-covered-table-cell
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_covered_table_cell::ns = L"table";
const wchar_t * table_covered_table_cell::name = L"covered-table-cell";

void table_covered_table_cell::add_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name)
{
	empty_ = false;
    table_table_cell_content_.add_child_element( Ns, Name, getContext());
}


// table:table-row
// table-table-row
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_table_row::ns = L"table";
const wchar_t * table_table_row::name = L"table-row";

void table_table_row::add_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if ( CP_CHECK_NAME(L"table", L"table-cell") || 
         CP_CHECK_NAME(L"table", L"covered-table-cell") )
    {
		CP_CREATE_ELEMENT(content_);
    }
    else
        CP_NOT_APPLICABLE_ELM();    
}

// table:table-rows
// table-table-rows
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_table_rows::ns = L"table";
const wchar_t * table_table_rows::name = L"table-rows";

void table_table_rows::add_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(table_table_row_);
}

// table:table-header-rows
// table-table-header-rows
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_table_header_rows::ns = L"table";
const wchar_t * table_table_header_rows::name = L"table-header-rows";

void table_table_header_rows::add_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME(L"table", L"table-row")
    {
		CP_CREATE_ELEMENT(table_table_row_);
    }
    else
        CP_NOT_APPLICABLE_ELM();    
}


// table-rows
//////////////////////////////////////////////////////////////////////////////////////////////////
void table_rows::add_child_element(const ::std::wstring & Ns, const ::std::wstring & Name, odf_conversion_context * Context)
{
    if CP_CHECK_NAME(L"table", L"table-rows")
    {
		CP_CREATE_ELEMENT_SIMPLE(table_table_rows_);
    } 
    else if CP_CHECK_NAME(L"table", L"table-row")
    {
		CP_CREATE_ELEMENT_SIMPLE(table_table_row_);
    }
    else
    {
        not_applicable_element(L"table-rows", Ns, Name);        
    }
}

// table-rows-no-group
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_rows_no_group::ns = L"table";
const wchar_t * table_rows_no_group::name = L"table-rows-no-group";

_CP_PTR(table_rows_no_group) table_rows_no_group::create()
{
    return boost::make_shared<table_rows_no_group>();
}

table_rows_no_group::table_rows_no_group() : was_header_(false)
{
};

void table_rows_no_group::add_child_element( const ::std::wstring & Ns, const ::std::wstring & Name, odf_conversion_context * Context)
{
    if (CP_CHECK_NAME(L"table", L"table-rows") || CP_CHECK_NAME(L"table", L"table-row"))
    {
        if (!was_header_)
            table_rows_1_.add_child_element(Ns, Name, Context);
        else
            table_rows_2_.add_child_element(Ns, Name, Context);
    } 
    else if CP_CHECK_NAME(L"table", L"table-header-rows")
    {
        was_header_ = true;
		CP_CREATE_ELEMENT(table_table_header_rows_);
    }
    else
        not_applicable_element(L"table-rows-no-group",Ns, Name);
}

// table-rows-and-groups
//////////////////////////////////////////////////////////////////////////////////////////////////

table_rows_and_groups::table_rows_and_groups()
{
}

void table_rows_and_groups::add_child_element( const ::std::wstring & Ns, const ::std::wstring & Name, odf_conversion_context * Context)
{
    if (L"table" == Ns && L"table-row-group" == Name)
    {
		CP_CREATE_ELEMENT_SIMPLE(content_);
    } 
    else if (L"table" == Ns && (L"table-rows" == Name || L"table-row" == Name || L"table-header-rows" == Name) )
    {
        _CP_PTR(table_rows_no_group) elm = table_rows_no_group::create();
        elm->add_child_element(Ns, Name, Context);
        content_.push_back(elm);
   }
    else
        not_applicable_element(L"table-rows-and-groups", Ns, Name);
}

// table:table-row-group
// table-table-row-group
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_table_row_group::ns = L"table";
const wchar_t * table_table_row_group::name = L"table-row-group";


void table_table_row_group::add_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    table_rows_and_groups_.add_child_element(Ns, Name, getContext());
}


///////////


// table:shapes
// table-shapes
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_shapes::ns = L"table";
const wchar_t * table_shapes::name = L"shapes";

void table_shapes::add_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
	create_element(Ns,Name,content_,getContext());
}



}
}
