#ifndef BINARY_FILE_PARSER_HPP
#define BINARY_FILE_PARSER_HPP

#include "Core.hpp"
#include "PyString.hpp"
#include "PyInteger.hpp"
#include "CodeObject.hpp"
#include "ArrayList.hpp"
#include "NonCopyable.hpp"
#include "FileInputStream.hpp"


class BinaryFileParser : public fromboost::NonCopyable {
public:
    explicit BinaryFileParser(FileInputStream *fileInputStream) NOEXCEPT;
    ~BinaryFileParser();
    CodeObject* parse();
private:
    CodeObject *get_code_object();
    PyString *get_byte_codes();
    ArrayList<PyObject*> *get_consts();
    ArrayList<PyObject*> *get_names();
    ArrayList<PyObject*> *get_var_names();
    ArrayList<PyObject*> *get_free_vars();
    ArrayList<PyObject*> *get_cell_vars();
    PyString* get_file_name();
    PyString* get_name();
    PyString* get_no_table();
    // support
    PyString *get_string();
    ArrayList<PyObject*> *get_tuple();
private:
    // not owned the target object, do not delete at ~BinaryFileParser()
    FileInputStream* m_Stream;
    ArrayList<PyString*> m_StringTable;
};

#endif
