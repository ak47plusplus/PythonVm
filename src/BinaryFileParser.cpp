#include "BinaryFileParser.hpp"
#include <cstdio>
#include <memory>
#include <assert.h>
#include "Logger.hpp"

BinaryFileParser::BinaryFileParser(FileInputStream *fileInputStream) NOEXCEPT
{
    this->m_Stream = fileInputStream;
}

BinaryFileParser::~BinaryFileParser(){}

CodeObject* BinaryFileParser::parse()
{
    int magicNumber = m_Stream->read_int();
    printf("magic nuber: 0x%x\n", magicNumber);

    int updateDate = m_Stream->read_int();
    printf("update date: 0x%x\n", updateDate);

    char objectType = m_Stream->read();
    if (objectType == 'c') {
        CodeObject* result = this->get_code_object();
        printf("Parser Ok!\n");
        return result;
    }
    return nullptr;
}

CodeObject *BinaryFileParser::get_code_object()
{
    int argCount = m_Stream->read_int();
    int nLocals = m_Stream->read_int();
    int stackSize = m_Stream->read_int();
    int flags = m_Stream->read_int();
    LOG(INFO) << "argCount: " << argCount << "nLocals: " << nLocals
        << "stackSize: " << stackSize << "flags: " << flags;

    PyString *byteCodes = this->get_byte_codes();
    ArrayList<PyObject*> *consts = this->get_consts();
    ArrayList<PyObject*> *names = this->get_names();
    ArrayList<PyObject*> *varNames = this->get_var_names();
    ArrayList<PyObject*> *freeVars = this->get_free_vars();
    ArrayList<PyObject*> *cellVars = this->get_cell_vars();
    PyString *fileName = this->get_file_name();
    PyString *moduleName = this->get_name();
    int beginLineNo = this->m_Stream->read_int();
    PyString *lnotab = this->get_no_table();

    return new CodeObject(argCount, nLocals, stackSize,flags, byteCodes,
      consts,names, varNames,freeVars,cellVars,fileName,moduleName,beginLineNo, lnotab);
}

/*
 * Read the bytecode from file stream.
 * |'s'|strLen|content|
 */
PyString *BinaryFileParser::get_byte_codes()
{
    assert(m_Stream->read() == 's');
    return this->get_string();
}

PyString *BinaryFileParser::get_string()
{
    int strLen = m_Stream->read_int();
    char *strArr = new char[strLen];
    std::shared_ptr<char> charGuard(strArr, [](char *p){delete[] p;});
    for(int i = 0; i < strlen; i++) {
        strArr[i] = m_Stream->read();
    }
    return new PyString(strArr,strLen);
}

ArrayList<PyObject*> *BinaryFileParser::get_tuple()
{

}
ArrayList<PyObject*> *BinaryFileParser::get_consts()
{
    return nullptr;
}
ArrayList<PyObject*> *BinaryFileParser::get_names()
{
    return nullptr;
}
ArrayList<PyObject*> *BinaryFileParser::get_var_names()
{
    return nullptr;
}
ArrayList<PyObject*> *BinaryFileParser::get_free_vars()
{
    return nullptr;
}
ArrayList<PyObject*> *BinaryFileParser::get_cell_vars()
{
    return nullptr;
}
PyString* BinaryFileParser::get_file_name()
{
    return nullptr;
}
PyString* BinaryFileParser::get_name()
{
    return nullptr;
}
PyString* BinaryFileParser::get_no_table()
{
    return nullptr;
}
