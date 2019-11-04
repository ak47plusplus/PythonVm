#include "BinaryFileParser.hpp"
#include <cstdio>
#include <memory>
#include <assert.h>
#include "Logger.hpp"

BinaryFileParser::BinaryFileParser(FileInputStream *fileInputStream) NOEXCEPT
{
    this->m_Stream = fileInputStream;
    this->m_Logger = el::Loggers::getLogger("default");
}

BinaryFileParser::~BinaryFileParser(){}

CodeObject* BinaryFileParser::parse()
{
    int magicNumber = m_Stream->read_int();
    m_Logger->info("magic number: %x \n", magicNumber);

    int updateDate = m_Stream->read_int();
    m_Logger->info("update date: %x \n", updateDate);

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
    for(int i = 0; i < strLen; i++) {
        strArr[i] = this->m_Stream->read();
    }
    return new PyString(strArr,strLen);
}

ArrayList<PyObject*> *BinaryFileParser::get_tuple()
{
    int length = this->m_Stream->read_int();
    PyString *str;
    ArrayList<PyObject *> *list = new ArrayList<PyObject*>(length);
    for(int i = 0; i < length; i++)
    {
        char objType = this->m_Stream->read();
        switch (objType) {
            case 'c':
                LOG(DEBUG) << "this is a code object.";
                list->add(this->get_code_object()); // 为c带表内嵌一个CodeObject.
                break;
            case 'i':
                list->add(new PyInteger(this->m_Stream->read_int()));
                break;
            case 'N':
                list->add(NULL); // not nullptr. TODO
                break;
            case 't':
                str = this->get_string();
                list->add(str);
                this->m_StringTable.add(str);
                break;
            case 's':
                list->add(get_string());
                break;
            case 'R':
                list->add(this->m_StringTable.get(this->m_Stream->read_int()));
                break;
        }
    }
    return list;
}
ArrayList<PyObject*> *BinaryFileParser::get_consts()
{
    if(m_Stream->read() == '(')
    {
        return this->get_tuple();
    }
    m_Stream->unread();
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
