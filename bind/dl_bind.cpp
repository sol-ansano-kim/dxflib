#include <pybind11/pybind11.h>
#include "dl_dxf.h"
#include "dl_creationadapter.h"
#include <vector>
#include <codecvt>
#include <string>


using namespace pybind11;


class TestReader : public DL_CreationAdapter
{
    public:
        TestReader() : DL_CreationAdapter() {};
        ~TestReader() {};
        void addText(const DL_TextData& t)
        {
            m_text.push_back(t.text);
        };

        const std::vector<std::string> &getText()
        {
            return m_text;
        }

    private:
        std::vector<std::string> m_text;
};


pybind11::list read_text(const std::string &path)
{
    setlocale(LC_CTYPE, "");
    pybind11::list res;
    TestReader* reader;
    DL_Dxf *dxf = new DL_Dxf();
    reader = new TestReader();

    if (!dxf->in(path.c_str(), reader))
    {
        printf("[dxf2abc] Failed to read dxf file.\n");
        return res;
    }

    const std::vector<std::string> t = reader->getText();
    for (std::vector<std::string>::const_iterator it = t.begin(); it != t.end(); ++it)
    {
        res.append((*it));
    }

    delete dxf;
    delete reader;
    return res;
}


void bindDXF(pybind11::module mod)
{
    mod.def("read_text", &read_text);
}


PYBIND11_MODULE(_pydxf, mod)
{
    bindDXF(mod);
}
