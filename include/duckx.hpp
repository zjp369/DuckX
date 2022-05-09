/*
 * Under MIT license
 * Author: Amir Mohamadi (@amiremohamadi)
 * DuckX is a free library to work with docx files.
 */

#ifndef DUCKX_H
#define DUCKX_H

#include <cstdio>
#include <stdlib.h>
#include <string>

#include <constants.hpp>
#include <duckxiterator.hpp>
#include <pugixml.hpp>

// TODO: Use container-iterator design pattern!

namespace duckx {
// Run contains runs in a paragraph
class Run {
  private:
    friend class IteratorHelper;
    // Store the parent node (a paragraph)
    pugi::xml_node parent;
    // And store current node also
    pugi::xml_node current;

  public:
    Run();
    Run(pugi::xml_node, pugi::xml_node);
    void set_parent(pugi::xml_node);
    void set_current(pugi::xml_node);

    std::string get_text() const;
    inline bool set_text(const std::string &t) const { return set_text(t.c_str()); };
    bool set_text(const char *) const;

    Run &next();
    bool has_next() const;
};

// Paragraph contains a paragraph
// and stores runs
class Paragraph {
  private:
    friend class IteratorHelper;
    // Store parent node (usually the body node)
    pugi::xml_node parent;
    // And store current node also
    pugi::xml_node current;
    // A paragraph consists of runs
    Run run;

  public:
    Paragraph();
    Paragraph(pugi::xml_node, pugi::xml_node);
    void set_parent(pugi::xml_node);
    void set_current(pugi::xml_node);

    Paragraph &next();
    bool has_next() const;

    Run &runs();
    inline Run &add_run(const std::string &t, duckx::formatting_flag f = duckx::none, unsigned char nFontSize = 0, const std::string &strFontName = std::string()) { return add_run(t.c_str(),f, nFontSize, strFontName.data()); };
    Run &add_run(const char *, duckx::formatting_flag = duckx::none, unsigned char nFontSize = 0, const char *pszFontName = NULL);
    inline Paragraph &append(const std::string &t, duckx::formatting_flag f = duckx::none, unsigned char nFontSize = 0, const std::string &strFontName = std::string()) { return append(t.c_str(), f, nFontSize, strFontName.data()); };
    Paragraph &append(const char *, duckx::formatting_flag = duckx::none, unsigned char nFontSize = 0, const char *pszFontName = NULL);
    void set_alignment(paragraph_alignment a = duckx::align_left, unsigned char indent = 0, unsigned char font_size=10);
    inline bool set_text(const std::string &t, duckx::formatting_flag f = duckx::none) { return set_text(t.c_str(), f); };
    bool set_text(const char *, duckx::formatting_flag = duckx::none);
    void add(pugi::xml_node);
};

// TableCell contains one or more paragraphs
class TableCell {
  private:
    friend class IteratorHelper;
    pugi::xml_node parent;
    pugi::xml_node current;

    Paragraph paragraph;

  public:
    TableCell();
    TableCell(pugi::xml_node, pugi::xml_node);

    void set_parent(pugi::xml_node);
    void set_current(pugi::xml_node);

    Paragraph &paragraphs();

    TableCell &next();
    bool has_next() const;
    void resize(int nCol = 1);
    void mergeCol(int nSpanCol = 2);
    void mergeRow(bool bStart = false);
    void add();
};

// TableRow consists of one or more TableCells
class TableRow {
    friend class IteratorHelper;
    pugi::xml_node parent;
    pugi::xml_node current;

    TableCell cell;

  public:
    TableRow();
    TableRow(pugi::xml_node, pugi::xml_node);
    void set_parent(pugi::xml_node);
    void set_current(pugi::xml_node);

    TableCell &cells();
    bool has_next() const;
    TableRow &next();
    void resize(int nRow = 1, int nCol = 1);
    void merge(int nStartRow = 1, int nStartCol = 1, int nSpanRow = 1, int nSpanCol = 2);
    void add();
};

// Table consists of one or more TableRow objects
class Table {
  private:
    friend class IteratorHelper;
    pugi::xml_node parent;
    pugi::xml_node current;

    TableRow row;

  public:
    Table();
    Table(pugi::xml_node, pugi::xml_node);
    void set_parent(pugi::xml_node);
    void set_current(pugi::xml_node);

    Table &append();
    Table &next();
    bool has_next() const;
    void resize(int nRow = 1, int nCol = 1);
    void merge(int nStartRow = 1, int nStartCol = 1, int nSpanRow = 1, int nSpanCol = 2);
    void set_text(int nRow, int nCol, const char *lpszText);

    TableRow &rows();
};

// Document contains whole the docx file
// and stores paragraphs
class Document {
  private:
    friend class IteratorHelper;
    std::string directory;
    Paragraph paragraph;
    Table table;
    pugi::xml_document document;

  public:
    Document();
    Document(std::string);
    void file(std::string);
    void open();
    void save() const;
    void clear();

	Paragraph &paragraphs();
	Table &tables();
};
} // namespace duckx

#endif
