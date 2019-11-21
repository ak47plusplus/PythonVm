#include "PycFileParser.hpp"
#include "VM.hpp"
#include "Panic.hpp"
#include "Logger.hpp"
#include "PyInteger.hpp"
#include "PyDouble.hpp"
#include "PyString.hpp"

#include <cstdio>
#include <memory>
#include <assert.h>


PycFileParser::PycFileParser(FileInputStream *fileInputStream) NOEXCEPT
{
    this->m_Stream = fileInputStream;
}

PycFileParser::~PycFileParser(){}

CodeObject* PycFileParser::parse()
{
    int magicNumber = m_Stream->read_int();
    printf("读取到魔数: %x \n", magicNumber);

    int updateDate = m_Stream->read_int();
    printf("读取到更新时间: %x \n", updateDate);

    char objectType = m_Stream->read();
    if (objectType == 'c') {
        CodeObject* result = this->get_code_object();
        printf("字节码读取解析完毕,可以转交解释器执行!\n");
        return result;
    }
    return nullptr;
}

CodeObject *PycFileParser::get_code_object()
{
    printf("开始获取CodeObject.\n");
    int argCount = m_Stream->read_int();
    int nLocals = m_Stream->read_int();
    int stackSize = m_Stream->read_int();
    int flags = m_Stream->read_int();
    LOG(INFO) << "参数: argCount: " << argCount << ", nLocals: " << nLocals
        << ", stackSize: " << stackSize << ", flags: " << flags;

    printf("\t开始读取字节操作码...\n");
    PyString *byteCodes = this->get_byte_codes();
    byteCodes->print();

    printf("\t开始加载常量表/变量表/符号表/模块名等等...\n");
    ArrayList<PyObject*> *consts = this->get_consts();
    ArrayList<PyObject*> *names = this->get_names();
    ArrayList<PyObject*> *varNames = this->get_var_names();
    ArrayList<PyObject*> *freeVars = this->get_free_vars();
    ArrayList<PyObject*> *cellVars = this->get_cell_vars();
    PyString *fileName = this->get_file_name();
    PyString *moduleName = this->get_name();
    int beginLineNo = this->m_Stream->read_int();
    PyString *lnotab = this->get_no_table();

    printf("CodeObject读取完毕\n");
    return new CodeObject(argCount, nLocals, stackSize,flags, byteCodes,
      consts,names, varNames,freeVars,cellVars,fileName,moduleName,beginLineNo, lnotab);
}

/*
 * Read the bytecode from file stream.
 * |'s'|strLen|content|
 */
PyString *PycFileParser::get_byte_codes()
{
    assert(m_Stream->read() == 's');
    return this->get_string();
}

PyString *PycFileParser::get_string()
{
    int strLen = m_Stream->read_int();
    char *strArr = new char[strLen];
    std::shared_ptr<char> charGuard(strArr, [](char *p){delete[] p;});
    for(int i = 0; i < strLen; i++) {
        strArr[i] = this->m_Stream->read();
    }
    return new PyString(strArr,strLen);
}

ArrayList<PyObject*> *PycFileParser::get_tuple()
{
    int length = this->m_Stream->read_int();
    PyString *str;
    ArrayList<PyObject *> *list = new ArrayList<PyObject*>(length);
    for(int i = 0; i < length; i++)
    {
        char objType = this->m_Stream->read();
        switch (objType) {
            case 'c':
                list->add(this->get_code_object()); // 为c带表内嵌一个CodeObject.
                break;
            case 'i':
                list->add(new PyInteger(m_Stream->read_int()));
                break;
            case 'g':
                list->add(new PyDouble(m_Stream->read_double()));
                break;
            case 'N':
                list->add(VM::PyNone);
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
                list->add(this->m_StringTable.get(m_Stream->read_int()));
                break;
            case '(':
                // TODO.
                break;
            default:
                __panic("Unrecognized obj type: %c \n", objType);
        }
    }
    return list;
}
ArrayList<PyObject*> *PycFileParser::get_consts()
{
    if(m_Stream->read() == '(')
    {
        return this->get_tuple();
    }
    m_Stream->unread();
    return nullptr;
}

ArrayList<PyObject*> *PycFileParser::get_names()
{
    if(m_Stream->read() == '(')
    {
        return this->get_tuple();
    }
    m_Stream->unread();
    return nullptr;
}

ArrayList<PyObject*> *PycFileParser::get_var_names()
{
    if(m_Stream->read() == '(')
    {
        return this->get_tuple();
    }
    m_Stream->unread();
    return nullptr;
}

ArrayList<PyObject*> *PycFileParser::get_free_vars()
{
    if(m_Stream->read() == '(')
    {
        return this->get_tuple();
    }
    m_Stream->unread();
    return nullptr;
}

ArrayList<PyObject*> *PycFileParser::get_cell_vars()
{
    if(m_Stream->read() == '(')
    {
        return this->get_tuple();
    }
    m_Stream->unread();
    return nullptr;
}

PyString* PycFileParser::get_file_name()
{
    return get_name();
}

PyString* PycFileParser::get_name()
{
    int8_t ch = m_Stream->read();
    if(ch == 's')
    {
        return get_string();
    }
    else if(ch == 't')
    {
        PyString *str = get_string();
        m_StringTable.add(str);
        return str;
    }
    else if(ch == 'R')
    {
        int index = m_Stream->read_int();
        return m_StringTable.get(index);
    }
    return nullptr;
}

PyString* PycFileParser::get_no_table()
{
    int8_t ch = m_Stream->read();
    if(ch != 's' && ch != 't')
    {
        m_Stream->unread();
        return nullptr;
    }
    return get_string();
}
