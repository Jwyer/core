#include "precompiled_cpodf.h"


#include "ods_conversion_context.h"

#include "office_spreadsheet.h"

namespace cpdoccore { 
namespace odf {


ods_conversion_context::ods_conversion_context(package::odf_document * outputDocument) 
		: odf_conversion_context(outputDocument), ods_table_context_(*this)
{
}


void ods_conversion_context::start_document()
{
	create_element(L"office", L"spreadsheet", content_, this,true);

	current_spreadsheet_ = (office_spreadsheet*)dynamic_cast<office_spreadsheet*>(content_.back().get());
}


void ods_conversion_context::start_sheet(std::wstring & name)
{
	create_element(L"table", L"table",current_spreadsheet_->getContent(),this);
	
	ods_table_context_.start_table(current_spreadsheet_->getContent().back(),name);
	
}

void ods_conversion_context::end_sheet()
{
	ods_table_context_.end_table();
}



}
}
