// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef APPCOMMON_WIDGETS_REPORT_TABLE_H_
#define APPCOMMON_WIDGETS_REPORT_TABLE_H_

#include <list>
#include <string>

#include "wx/listctrl.h"
#include "wx/wx.h"
#include "wx/xml/xml.h"

/// \par OVERVIEW
///
/// This struct represents a single column header.
struct ReportColumnHeader {
  /// \var format
  ///   The column format.
  wxListColumnFormat format;

  /// \var title
  ///   The column title.
  std::string title;

  /// \var width
  ///   The column width.
  int width;
};

/// \par OVERVIEW
///
/// This struct represents a single row of data.
struct ReportRow {
  /// \var values
  ///   The data values, which are stored as strings.
  std::list<std::string> values;
};

/// \par OVERVIEW
///
/// This struct
struct ReportData {
  /// \var headers
  ///   The column headers of the data.
  std::list<ReportColumnHeader> headers;

  /// \var rows
  ///   The data rows.
  std::list<ReportRow> rows;
};

/// \par OVERVIEW
///
/// This enum contains types of sort orders.
enum class SortOrderType {
  kAscending,
  kDescending,
  kNone
};

/// \par OVERVIEW
///
/// This class uses a listctrl to display data in a report view.
///
/// \par DATA
///
/// This class uses the data provided to generate listctrl entries. The listctrl
/// stores its own strings within the control, but it uses the associated data
/// to sort/organize as needed.
///
/// \par SORTING
///
/// Sorting is handled by the listctrl and does NOT affect the sort order of the
/// provided data, but rather, just inside the control itself. It is structured
/// this way to use the wxListCtrl::Sort function.
///
/// \par COPY TO CLIPBOARD
///
/// This class supports several copy to clipboard commands. All of the data is
/// taken from the listctrl itself, so it will reflect the sorted order (if
/// any).
///
/// \par EXPORT TO XML
///
/// This class supports exporting to an xml file. All of the data is taken from
/// the listctrl itself, so it will reflect the sorted order (if any).
class ReportTable : public wxPanel {
 public:
  /// \brief Constructor.
  /// \param[in] parent
  ///   The parent window.
  ReportTable(wxWindow* parent);

  /// \brief Destructor.
  ~ReportTable();

  /// \brief Gets the report row index.
  /// \param[in] index_listctrl
  ///   The listctrl index, which reflects any sorting that is applied.
  /// \return The report row index, which does not reflect any sorting.
  const long IndexReportRow(const long& index_listctrl) const;

  /// \brief Gets if the listctrl is refreshing.
  /// \return If the listctrl is refreshing.
  bool IsRefreshing() const;

  /// \brief Refreshes the listctrl with the data.
  /// This method will clear the listctrl and re-insert the data. Any sorting
  /// and selection will be reapplied as well.
  void Refresh();

  /// \brief Gets a header title.
  /// \param[in] index_column
  ///   The column index.
  /// \return The header title.
  std::string TitleHeader(const int& index_column) const;

  /// \brief Gets a table value.
  /// \param[in] index_row
  ///   The row index.
  /// \param[in] index_column
  ///   The column index.
  /// \return The table value.
  std::string ValueTable(const int& index_row, const int& index_column) const;

  /// \brief Gets the data that is displayed.
  /// \return The displayed data.
  const ReportData* data() const;

  /// \brief Gets the selected item index.
  /// \return The selected item index. If no item is selected, -1 is returned.
  long index_selected() const;

  /// \brief Gets the sorted column index.
  /// \return The sorted column index.
  long index_sorted() const;

  /// \brief Sets the data to be displayed.
  /// \param[in] data
  ///   The data.
  void set_data(const ReportData* data);

  /// \brief Sets the selected row index.
  /// \param[in] index
  ///   The index to be selected.
  /// Requires a Refresh() in order to take effect.
  void set_index_selected(const long& index);

  /// \brief Sets the sorted column index.
  /// \param[in] index
  ///   The index to be sorted.
  /// Requires a Refresh() in order to take effect.
  void set_index_sorted(const long& index);

  /// \brief Sets the sort type.
  /// \param[in] type_sort
  ///   The sort type.
  /// Requires a Refresh() in order to take effect.
  void set_type_sort(const SortOrderType& type_sort);

  /// \brief Gets the type of sorting being applied.
  /// \return The sort type that is being applied.
  SortOrderType type_sort() const;

 private:
  /// \brief Changes the column header sort image.
  /// \param[in] index
  ///   The column index.
  /// \param[in] type_sort
  ///   The sort type.
  void ChangeColumnSortImage(const int& index, const SortOrderType& type_sort);

  /// \brief Clears the listctrl.
  void ClearListCtrl();

  /// \brief Gets a clipboard string for the column headers.
  /// \return A concatenated string. Starting from left to right, with each
  ///   value (column) separated a tab character. The end of the string will
  ///   contain a new line character.
  std::string ClipboardStringHeaders() const;

  /// \brief Gets a clipboard string for a specific row.
  /// \param[in] index
  ///   The row index.
  /// \return A concatenated string. Starting from left to right, with each
  ///   value (column) separated by a tab character. The end of the string will
  ///   contain a new line character.
  std::string ClipboardStringRow(const int& index) const;

  /// \brief Gets a clipboard string for the entire table, excluding headers.
  /// \return A concatenated string. Starting from left to right, with each
  ///   value (column) separated by a tab character. The end of a row will
  ///   contain a new line character.
  std::string ClipBoardStringTable() const;

  /// \brief Copies the string to the clipboard.
  /// \param[in] str
  ///   The string.
  void CopyToClipboard(const std::string& str);

  /// \brief Handles the column click event.
  /// \param[in] event
  ///   The event.
  void OnColumnClick(wxListEvent& event);

  /// \brief Handles the column click event.
  /// \param[in] event
  ///   The event.
  void OnColumnRightClick(wxListEvent& event);

  /// \brief Handles the context menu select event.
  /// \param[in] event
  ///   The event.
  void OnContextMenuSelect(wxCommandEvent& event);

  /// \brief Handles the column right click event.
  /// \param[in] event
  ///   The event.
  void OnItemRightClick(wxListEvent& event);

  /// \brief Sorts the listctrl.
  void Sort();

  /// \brief Gets an xml-safe string.
  /// \param[in] str
  ///   The starting string, with potentially illegal characters.
  /// \param[in] ch
  ///   The legal xml character that will replace illegal characters.
  /// \return An xml-safe string.
  std::string StringXml(const std::string& str, const char& ch) const;

  /// \brief Gets an xml node for the entire table.
  /// \return An xml node for the entire table.
  wxXmlNode* XmlNodeTable() const;

  /// \var data_
  ///   The data that is passed onto the listctrl.
  const ReportData* data_;

  /// \var index_selected_
  ///   The row index to select during a refresh. Set to -1 if nothing is
  ///   selected.
  long index_selected_;

  /// \var index_sorted_
  ///   The column index that is currently sorted. Set to -1 if none are sorted.
  long index_sorted_;

  /// \var is_refreshing_
  ///   An indicator that determines if the listctrl is being refreshed.
  bool is_refreshing_;

  /// \var type_sort_
  ///   The type of sorting that is currently applied.
  SortOrderType type_sort_;

  /// \var listctrl_
  ///   The listctrl that is used to display the data.
  wxListCtrl* listctrl_;

  DECLARE_EVENT_TABLE()
};

#endif  // APPCOMMON_WIDGETS_REPORT_TABLE_H_
