#include <QtQml/qqmlprivate.h>
#include <QtCore/qdir.h>
#include <QtCore/qurl.h>

static const unsigned char qt_resource_tree[] = {
0,
0,0,0,0,2,0,0,0,11,0,0,0,1,0,0,0,
230,0,2,0,0,0,3,0,0,0,72,0,0,6,100,0,
2,0,0,0,11,0,0,0,61,0,0,0,98,0,0,0,
0,0,1,0,0,0,0,0,0,0,194,0,0,0,0,0,
1,0,0,0,0,0,0,0,8,0,0,0,0,0,1,0,
0,0,0,0,0,1,86,0,2,0,0,0,34,0,0,0,
27,0,0,7,190,0,2,0,0,0,15,0,0,0,12,0,
0,0,76,0,0,0,0,0,1,0,0,0,0,0,0,0,
168,0,0,0,0,0,1,0,0,0,0,0,0,0,42,0,
0,0,0,0,1,0,0,0,0,0,0,0,130,0,0,0,
0,0,1,0,0,0,0,0,0,8,18,0,0,0,0,0,
1,0,0,0,0,0,0,10,26,0,0,0,0,0,1,0,
0,0,0,0,0,8,62,0,0,0,0,0,1,0,0,0,
0,0,0,8,166,0,0,0,0,0,1,0,0,0,0,0,
0,10,64,0,0,0,0,0,1,0,0,0,0,0,0,9,
240,0,0,0,0,0,1,0,0,0,0,0,0,9,38,0,
0,0,0,0,1,0,0,0,0,0,0,9,160,0,0,0,
0,0,1,0,0,0,0,0,0,9,120,0,0,0,0,0,
1,0,0,0,0,0,0,8,112,0,0,0,0,0,1,0,
0,0,0,0,0,7,208,0,0,0,0,0,1,0,0,0,
0,0,0,9,78,0,0,0,0,0,1,0,0,0,0,0,
0,9,16,0,0,0,0,0,1,0,0,0,0,0,0,8,
220,0,0,0,0,0,1,0,0,0,0,0,0,9,194,0,
0,0,0,0,1,0,0,0,0,0,0,3,138,0,0,0,
0,0,1,0,0,0,0,0,0,2,8,0,0,0,0,0,
1,0,0,0,0,0,0,6,30,0,0,0,0,0,1,0,
0,0,0,0,0,3,104,0,0,0,0,0,1,0,0,0,
0,0,0,1,176,0,0,0,0,0,1,0,0,0,0,0,
0,3,170,0,0,0,0,0,1,0,0,0,0,0,0,5,
26,0,0,0,0,0,1,0,0,0,0,0,0,2,218,0,
0,0,0,0,1,0,0,0,0,0,0,1,138,0,0,0,
0,0,1,0,0,0,0,0,0,1,218,0,0,0,0,0,
1,0,0,0,0,0,0,2,166,0,0,0,0,0,1,0,
0,0,0,0,0,5,102,0,0,0,0,0,1,0,0,0,
0,0,0,5,210,0,0,0,0,0,1,0,0,0,0,0,
0,2,132,0,0,0,0,0,1,0,0,0,0,0,0,4,
174,0,0,0,0,0,1,0,0,0,0,0,0,5,138,0,
0,0,0,0,1,0,0,0,0,0,0,2,248,0,0,0,
0,0,1,0,0,0,0,0,0,5,240,0,0,0,0,0,
1,0,0,0,0,0,0,5,180,0,0,0,0,0,1,0,
0,0,0,0,0,1,112,0,0,0,0,0,1,0,0,0,
0,0,0,5,66,0,0,0,0,0,1,0,0,0,0,0,
0,4,70,0,0,0,0,0,1,0,0,0,0,0,0,4,
136,0,0,0,0,0,1,0,0,0,0,0,0,4,212,0,
0,0,0,0,1,0,0,0,0,0,0,3,32,0,0,0,
0,0,1,0,0,0,0,0,0,3,210,0,0,0,0,0,
1,0,0,0,0,0,0,2,56,0,0,0,0,0,1,0,
0,0,0,0,0,6,60,0,0,0,0,0,1,0,0,0,
0,0,0,4,94,0,0,0,0,0,1,0,0,0,0,0,
0,3,238,0,0,0,0,0,1,0,0,0,0,0,0,4,
24,0,0,0,0,0,1,0,0,0,0,0,0,3,56,0,
0,0,0,0,1,0,0,0,0,0,0,4,248,0,0,0,
0,0,1,0,0,0,0,0,0,2,94,0,0,0,0,0,
1,0,0,0,0,0,0,7,16,0,0,0,0,0,1,0,
0,0,0,0,0,7,74,0,0,0,0,0,1,0,0,0,
0,0,0,7,138,0,0,0,0,0,1,0,0,0,0,0,
0,6,244,0,0,0,0,0,1,0,0,0,0,0,0,7,
102,0,0,0,0,0,1,0,0,0,0,0,0,7,162,0,
0,0,0,0,1,0,0,0,0,0,0,6,214,0,0,0,
0,0,1,0,0,0,0,0,0,6,116,0,0,0,0,0,
1,0,0,0,0,0,0,7,42,0,0,0,0,0,1,0,
0,0,0,0,0,6,184,0,0,0,0,0,1,0,0,0,
0,0,0,6,162,0,0,0,0,0,1,0,0,0,0,0,
0,0,244,0,0,0,0,0,1,0,0,0,0,0,0,1,
22,0,0,0,0,0,1,0,0,0,0,0,0,1,50,0,
0,0,0,0,1,0,0,0,0};
static const unsigned char qt_resource_names[] = {
0,
1,0,0,0,47,0,47,0,14,5,128,75,28,0,66,0,
97,0,115,0,105,0,99,0,80,0,97,0,110,0,101,0,
108,0,46,0,113,0,109,0,108,0,14,11,0,111,28,0,
82,0,105,0,103,0,104,0,116,0,80,0,97,0,110,0,
101,0,108,0,46,0,113,0,109,0,108,0,8,8,1,90,
92,0,109,0,97,0,105,0,110,0,46,0,113,0,109,0,
108,0,13,1,196,125,252,0,76,0,101,0,102,0,116,0,
80,0,97,0,110,0,101,0,108,0,46,0,113,0,109,0,
108,0,16,12,104,207,115,0,73,0,110,0,116,0,101,0,
110,0,115,0,101,0,67,0,111,0,110,0,102,0,105,0,
103,0,46,0,106,0,115,0,10,10,13,140,51,0,118,0,
101,0,114,0,115,0,105,0,111,0,110,0,46,0,106,0,
115,0,15,1,219,87,92,0,77,0,105,0,100,0,100,0,
108,0,101,0,80,0,97,0,110,0,101,0,108,0,46,0,
113,0,109,0,108,0,4,0,7,167,147,0,116,0,97,0,
98,0,115,0,14,3,240,152,147,0,116,0,119,0,101,0,
101,0,116,0,83,0,101,0,97,0,114,0,99,0,104,0,
46,0,106,0,115,0,11,10,114,153,92,0,84,0,119,0,
105,0,116,0,116,0,101,0,114,0,46,0,113,0,109,0,
108,0,15,13,40,187,252,0,84,0,119,0,101,0,101,0,
116,0,115,0,77,0,111,0,100,0,101,0,108,0,46,0,
113,0,109,0,108,0,10,7,106,9,51,0,99,0,111,0,
109,0,112,0,111,0,110,0,101,0,110,0,116,0,115,0,
10,6,36,101,28,0,83,0,99,0,114,0,111,0,108,0,
108,0,46,0,113,0,109,0,108,0,16,1,131,22,124,0,
73,0,110,0,116,0,101,0,110,0,115,0,101,0,84,0,
97,0,98,0,108,0,101,0,46,0,113,0,109,0,108,0,
18,0,151,26,188,0,68,0,97,0,115,0,104,0,98,0,
111,0,97,0,114,0,100,0,84,0,97,0,98,0,108,0,
101,0,46,0,113,0,109,0,108,0,20,1,139,74,252,0,
65,0,100,0,100,0,114,0,101,0,115,0,115,0,66,0,
111,0,111,0,107,0,84,0,97,0,98,0,108,0,101,0,
46,0,113,0,109,0,108,0,21,0,51,60,220,0,78,0,
101,0,116,0,119,0,111,0,114,0,107,0,83,0,116,0,
97,0,116,0,117,0,115,0,73,0,116,0,101,0,109,0,
46,0,113,0,109,0,108,0,16,10,131,152,156,0,77,0,
111,0,98,0,105,0,108,0,101,0,72,0,101,0,97,0,
100,0,101,0,114,0,46,0,113,0,109,0,108,0,16,15,
232,66,124,0,72,0,105,0,115,0,116,0,111,0,114,0,
121,0,84,0,97,0,98,0,108,0,101,0,46,0,113,0,
109,0,108,0,14,4,18,125,188,0,77,0,101,0,110,0,
117,0,66,0,117,0,116,0,116,0,111,0,110,0,46,0,
113,0,109,0,108,0,23,2,77,253,124,0,68,0,97,0,
101,0,109,0,111,0,110,0,77,0,97,0,110,0,97,0,
103,0,101,0,114,0,68,0,105,0,97,0,108,0,111,0,
103,0,46,0,113,0,109,0,108,0,12,1,103,199,92,0,
76,0,105,0,110,0,101,0,69,0,100,0,105,0,116,0,
46,0,113,0,109,0,108,0,17,4,250,129,252,0,84,0,
97,0,98,0,108,0,101,0,68,0,114,0,111,0,112,0,
100,0,111,0,119,0,110,0,46,0,113,0,109,0,108,0,
9,8,191,244,220,0,76,0,97,0,98,0,101,0,108,0,
46,0,113,0,109,0,108,0,21,14,163,128,124,0,80,0,
114,0,105,0,118,0,97,0,99,0,121,0,76,0,101,0,
118,0,101,0,108,0,83,0,109,0,97,0,108,0,108,0,
46,0,113,0,109,0,108,0,14,0,145,157,220,0,68,0,
97,0,116,0,101,0,80,0,105,0,99,0,107,0,101,0,
114,0,46,0,113,0,109,0,108,0,13,0,5,71,60,0,
84,0,101,0,120,0,116,0,66,0,108,0,111,0,99,0,
107,0,46,0,113,0,109,0,108,0,17,0,183,237,220,0,
73,0,110,0,116,0,101,0,110,0,115,0,101,0,72,0,
101,0,97,0,100,0,101,0,114,0,46,0,113,0,109,0,
108,0,11,10,10,94,60,0,84,0,105,0,112,0,73,0,
116,0,101,0,109,0,46,0,113,0,109,0,108,0,18,13,
26,127,220,0,83,0,116,0,97,0,110,0,100,0,97,0,
114,0,100,0,66,0,117,0,116,0,116,0,111,0,110,0,
46,0,113,0,109,0,108,0,20,13,193,22,60,0,83,0,
116,0,97,0,110,0,100,0,97,0,114,0,100,0,68,0,
114,0,111,0,112,0,100,0,111,0,119,0,110,0,46,0,
113,0,109,0,108,0,9,7,199,248,156,0,73,0,110,0,
112,0,117,0,116,0,46,0,113,0,109,0,108,0,18,11,
237,165,156,0,80,0,97,0,115,0,115,0,119,0,111,0,
114,0,100,0,68,0,105,0,97,0,108,0,111,0,103,0,
46,0,113,0,109,0,108,0,16,7,226,193,252,0,84,0,
105,0,99,0,107,0,68,0,101,0,108,0,101,0,103,0,
97,0,116,0,101,0,46,0,113,0,109,0,108,0,16,4,
95,249,156,0,80,0,114,0,105,0,118,0,97,0,99,0,
121,0,76,0,101,0,118,0,101,0,108,0,46,0,113,0,
109,0,108,0,15,8,151,235,60,0,84,0,97,0,98,0,
108,0,101,0,72,0,101,0,97,0,100,0,101,0,114,0,
46,0,113,0,109,0,108,0,14,15,164,126,188,0,73,0,
99,0,111,0,110,0,66,0,117,0,116,0,116,0,111,0,
110,0,46,0,113,0,109,0,108,0,17,0,207,225,252,0,
81,0,82,0,67,0,111,0,100,0,101,0,83,0,99,0,
97,0,110,0,110,0,101,0,114,0,46,0,113,0,109,0,
108,0,15,6,213,170,124,0,83,0,101,0,97,0,114,0,
99,0,104,0,73,0,110,0,112,0,117,0,116,0,46,0,
113,0,109,0,108,0,15,3,85,158,156,0,80,0,114,0,
111,0,103,0,114,0,101,0,115,0,115,0,66,0,97,0,
114,0,46,0,113,0,109,0,108,0,18,4,229,28,188,0,
83,0,116,0,97,0,110,0,100,0,97,0,114,0,100,0,
68,0,105,0,97,0,108,0,111,0,103,0,46,0,113,0,
109,0,108,0,12,5,104,11,188,0,78,0,111,0,116,0,
105,0,102,0,105,0,101,0,114,0,46,0,113,0,109,0,
108,0,12,3,228,53,28,0,84,0,105,0,116,0,108,0,
101,0,66,0,97,0,114,0,46,0,113,0,109,0,108,0,
20,5,101,27,220,0,80,0,114,0,111,0,99,0,101,0,
115,0,115,0,105,0,110,0,103,0,83,0,112,0,108,0,
97,0,115,0,104,0,46,0,113,0,109,0,108,0,12,0,
86,39,220,0,67,0,104,0,101,0,99,0,107,0,66,0,
111,0,120,0,46,0,113,0,109,0,108,0,17,11,93,238,
124,0,68,0,97,0,101,0,109,0,111,0,110,0,67,0,
111,0,110,0,115,0,111,0,108,0,101,0,46,0,113,0,
109,0,108,0,5,0,118,125,195,0,112,0,97,0,103,0,
101,0,115,0,20,11,37,204,220,0,73,0,110,0,116,0,
101,0,110,0,115,0,101,0,68,0,97,0,115,0,104,0,
98,0,111,0,97,0,114,0,100,0,46,0,113,0,109,0,
108,0,8,15,225,95,28,0,83,0,105,0,103,0,110,0,
46,0,113,0,109,0,108,0,12,15,223,34,60,0,83,0,
101,0,116,0,116,0,105,0,110,0,103,0,115,0,46,0,
113,0,109,0,108,0,12,7,180,95,28,0,84,0,114,0,
97,0,110,0,115,0,102,0,101,0,114,0,46,0,113,0,
109,0,108,0,11,5,56,229,188,0,73,0,110,0,116,0,
101,0,110,0,115,0,101,0,46,0,113,0,109,0,108,0,
10,0,68,216,156,0,77,0,105,0,110,0,105,0,110,0,
103,0,46,0,113,0,109,0,108,0,13,13,148,135,188,0,
68,0,97,0,115,0,104,0,98,0,111,0,97,0,114,0,
100,0,46,0,113,0,109,0,108,0,11,0,121,39,188,0,
82,0,101,0,99,0,101,0,105,0,118,0,101,0,46,0,
113,0,109,0,108,0,15,5,70,9,188,0,65,0,100,0,
100,0,114,0,101,0,115,0,115,0,66,0,111,0,111,0,
107,0,46,0,113,0,109,0,108,0,9,1,204,227,156,0,
84,0,120,0,75,0,101,0,121,0,46,0,113,0,109,0,
108,0,11,6,6,69,92,0,72,0,105,0,115,0,116,0,
111,0,114,0,121,0,46,0,113,0,109,0,108,0,6,7,
225,8,132,0,119,0,105,0,122,0,97,0,114,0,100,0,
30,8,208,148,188,0,87,0,105,0,122,0,97,0,114,0,
100,0,67,0,114,0,101,0,97,0,116,0,101,0,86,0,
105,0,101,0,119,0,79,0,110,0,108,0,121,0,87,0,
97,0,108,0,108,0,101,0,116,0,46,0,113,0,109,0,
108,0,19,1,143,235,60,0,87,0,105,0,122,0,97,0,
114,0,100,0,67,0,111,0,110,0,102,0,105,0,103,0,
117,0,114,0,101,0,46,0,113,0,109,0,108,0,22,1,
159,94,252,0,87,0,105,0,122,0,97,0,114,0,100,0,
67,0,114,0,101,0,97,0,116,0,101,0,87,0,97,0,
108,0,108,0,101,0,116,0,46,0,113,0,109,0,108,0,
24,7,229,106,220,0,87,0,105,0,122,0,97,0,114,0,
100,0,77,0,97,0,110,0,97,0,103,0,101,0,87,0,
97,0,108,0,108,0,101,0,116,0,85,0,73,0,46,0,
113,0,109,0,108,0,24,2,220,35,124,0,87,0,105,0,
122,0,97,0,114,0,100,0,82,0,101,0,99,0,111,0,
118,0,101,0,114,0,121,0,87,0,97,0,108,0,108,0,
101,0,116,0,46,0,113,0,109,0,108,0,23,11,188,226,
124,0,87,0,105,0,122,0,97,0,114,0,100,0,80,0,
97,0,115,0,115,0,119,0,111,0,114,0,100,0,73,0,
110,0,112,0,117,0,116,0,46,0,113,0,109,0,108,0,
8,11,3,106,147,0,117,0,116,0,105,0,108,0,115,0,
46,0,106,0,115,0,17,6,2,240,28,0,87,0,105,0,
122,0,97,0,114,0,100,0,79,0,112,0,116,0,105,0,
111,0,110,0,115,0,46,0,113,0,109,0,108,0,18,10,
209,231,124,0,87,0,105,0,122,0,97,0,114,0,100,0,
68,0,111,0,110,0,97,0,116,0,105,0,111,0,110,0,
46,0,113,0,109,0,108,0,17,6,119,19,92,0,87,0,
105,0,122,0,97,0,114,0,100,0,87,0,101,0,108,0,
99,0,111,0,109,0,101,0,46,0,113,0,109,0,108,0,
14,6,16,85,220,0,87,0,105,0,122,0,97,0,114,0,
100,0,77,0,97,0,105,0,110,0,46,0,113,0,109,0,
108,0,20,12,239,147,156,0,87,0,105,0,122,0,97,0,
114,0,100,0,80,0,97,0,115,0,115,0,119,0,111,0,
114,0,100,0,85,0,73,0,46,0,113,0,109,0,108,0,
18,3,210,35,252,0,87,0,105,0,122,0,97,0,114,0,
100,0,80,0,97,0,115,0,115,0,119,0,111,0,114,0,
100,0,46,0,113,0,109,0,108,0,16,1,157,76,188,0,
87,0,105,0,122,0,97,0,114,0,100,0,70,0,105,0,
110,0,105,0,115,0,104,0,46,0,113,0,109,0,108,0,
23,2,230,221,252,0,87,0,105,0,122,0,97,0,114,0,
100,0,77,0,101,0,109,0,111,0,84,0,101,0,120,0,
116,0,73,0,110,0,112,0,117,0,116,0,46,0,113,0,
109,0,108};
static const unsigned char qt_resource_empty_payout[] = { 0, 0, 0, 0, 0 };
QT_BEGIN_NAMESPACE
extern Q_CORE_EXPORT bool qRegisterResourceData(int, const unsigned char *, const unsigned char *, const unsigned char *);
QT_END_NAMESPACE
namespace QmlCacheGeneratedCode {
namespace _wizard_WizardMemoTextInput_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _pages_History_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_DaemonConsole_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _wizard_WizardFinish_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_CheckBox_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_ProcessingSplash_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _wizard_WizardPassword_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _wizard_WizardPasswordUI_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_TitleBar_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _tabs_TweetsModel_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _wizard_WizardMain_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_Notifier_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace __MiddlePanel_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _pages_TxKey_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_StandardDialog_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _wizard_WizardWelcome_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _pages_AddressBook_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _pages_Receive_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_ProgressBar_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_SearchInput_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_QRCodeScanner_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_IconButton_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _wizard_WizardDonation_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace __version_js { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _pages_Dashboard_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_TableHeader_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_PrivacyLevel_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _tabs_Twitter_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_TickDelegate_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _tabs_tweetSearch_js { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_PasswordDialog_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_Input_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace __IntenseConfig_js { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_StandardDropdown_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_StandardButton_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _pages_Mining_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _wizard_WizardOptions_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _wizard_utils_js { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_TipItem_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_IntenseHeader_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace __LeftPanel_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_TextBlock_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_DatePicker_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_PrivacyLevelSmall_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace __main_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _wizard_WizardPasswordInput_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _pages_Intense_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace __RightPanel_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_Label_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _pages_Transfer_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_TableDropdown_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _wizard_WizardRecoveryWallet_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_LineEdit_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _pages_Settings_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _wizard_WizardManageWalletUI_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _wizard_WizardCreateWallet_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _wizard_WizardConfigure_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_DaemonManagerDialog_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_MenuButton_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_HistoryTable_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace __BasicPanel_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_MobileHeader_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_NetworkStatusItem_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_AddressBookTable_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _pages_Sign_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_DashboardTable_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_IntenseTable_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _components_Scroll_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _pages_IntenseDashboard_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _wizard_WizardCreateViewOnlyWallet_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}

}
namespace {
struct Registry {
    Registry();
    QHash<QString, const QQmlPrivate::CachedQmlUnit*> resourcePathToCachedUnit;
    static const QQmlPrivate::CachedQmlUnit *lookupCachedUnit(const QUrl &url);
};

Q_GLOBAL_STATIC(Registry, unitRegistry);


Registry::Registry() {
        resourcePathToCachedUnit.insert(QStringLiteral("/wizard/WizardMemoTextInput.qml"), &QmlCacheGeneratedCode::_wizard_WizardMemoTextInput_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/pages/History.qml"), &QmlCacheGeneratedCode::_pages_History_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/DaemonConsole.qml"), &QmlCacheGeneratedCode::_components_DaemonConsole_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/wizard/WizardFinish.qml"), &QmlCacheGeneratedCode::_wizard_WizardFinish_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/CheckBox.qml"), &QmlCacheGeneratedCode::_components_CheckBox_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/ProcessingSplash.qml"), &QmlCacheGeneratedCode::_components_ProcessingSplash_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/wizard/WizardPassword.qml"), &QmlCacheGeneratedCode::_wizard_WizardPassword_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/wizard/WizardPasswordUI.qml"), &QmlCacheGeneratedCode::_wizard_WizardPasswordUI_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/TitleBar.qml"), &QmlCacheGeneratedCode::_components_TitleBar_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/tabs/TweetsModel.qml"), &QmlCacheGeneratedCode::_tabs_TweetsModel_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/wizard/WizardMain.qml"), &QmlCacheGeneratedCode::_wizard_WizardMain_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/Notifier.qml"), &QmlCacheGeneratedCode::_components_Notifier_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/MiddlePanel.qml"), &QmlCacheGeneratedCode::__MiddlePanel_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/pages/TxKey.qml"), &QmlCacheGeneratedCode::_pages_TxKey_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/StandardDialog.qml"), &QmlCacheGeneratedCode::_components_StandardDialog_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/wizard/WizardWelcome.qml"), &QmlCacheGeneratedCode::_wizard_WizardWelcome_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/pages/AddressBook.qml"), &QmlCacheGeneratedCode::_pages_AddressBook_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/pages/Receive.qml"), &QmlCacheGeneratedCode::_pages_Receive_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/ProgressBar.qml"), &QmlCacheGeneratedCode::_components_ProgressBar_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/SearchInput.qml"), &QmlCacheGeneratedCode::_components_SearchInput_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/QRCodeScanner.qml"), &QmlCacheGeneratedCode::_components_QRCodeScanner_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/IconButton.qml"), &QmlCacheGeneratedCode::_components_IconButton_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/wizard/WizardDonation.qml"), &QmlCacheGeneratedCode::_wizard_WizardDonation_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/version.js"), &QmlCacheGeneratedCode::__version_js::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/pages/Dashboard.qml"), &QmlCacheGeneratedCode::_pages_Dashboard_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/TableHeader.qml"), &QmlCacheGeneratedCode::_components_TableHeader_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/PrivacyLevel.qml"), &QmlCacheGeneratedCode::_components_PrivacyLevel_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/tabs/Twitter.qml"), &QmlCacheGeneratedCode::_tabs_Twitter_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/TickDelegate.qml"), &QmlCacheGeneratedCode::_components_TickDelegate_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/tabs/tweetSearch.js"), &QmlCacheGeneratedCode::_tabs_tweetSearch_js::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/PasswordDialog.qml"), &QmlCacheGeneratedCode::_components_PasswordDialog_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/Input.qml"), &QmlCacheGeneratedCode::_components_Input_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/IntenseConfig.js"), &QmlCacheGeneratedCode::__IntenseConfig_js::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/StandardDropdown.qml"), &QmlCacheGeneratedCode::_components_StandardDropdown_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/StandardButton.qml"), &QmlCacheGeneratedCode::_components_StandardButton_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/pages/Mining.qml"), &QmlCacheGeneratedCode::_pages_Mining_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/wizard/WizardOptions.qml"), &QmlCacheGeneratedCode::_wizard_WizardOptions_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/wizard/utils.js"), &QmlCacheGeneratedCode::_wizard_utils_js::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/TipItem.qml"), &QmlCacheGeneratedCode::_components_TipItem_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/IntenseHeader.qml"), &QmlCacheGeneratedCode::_components_IntenseHeader_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/LeftPanel.qml"), &QmlCacheGeneratedCode::__LeftPanel_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/TextBlock.qml"), &QmlCacheGeneratedCode::_components_TextBlock_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/DatePicker.qml"), &QmlCacheGeneratedCode::_components_DatePicker_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/PrivacyLevelSmall.qml"), &QmlCacheGeneratedCode::_components_PrivacyLevelSmall_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/main.qml"), &QmlCacheGeneratedCode::__main_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/wizard/WizardPasswordInput.qml"), &QmlCacheGeneratedCode::_wizard_WizardPasswordInput_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/pages/Intense.qml"), &QmlCacheGeneratedCode::_pages_Intense_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/RightPanel.qml"), &QmlCacheGeneratedCode::__RightPanel_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/Label.qml"), &QmlCacheGeneratedCode::_components_Label_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/pages/Transfer.qml"), &QmlCacheGeneratedCode::_pages_Transfer_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/TableDropdown.qml"), &QmlCacheGeneratedCode::_components_TableDropdown_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/wizard/WizardRecoveryWallet.qml"), &QmlCacheGeneratedCode::_wizard_WizardRecoveryWallet_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/LineEdit.qml"), &QmlCacheGeneratedCode::_components_LineEdit_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/pages/Settings.qml"), &QmlCacheGeneratedCode::_pages_Settings_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/wizard/WizardManageWalletUI.qml"), &QmlCacheGeneratedCode::_wizard_WizardManageWalletUI_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/wizard/WizardCreateWallet.qml"), &QmlCacheGeneratedCode::_wizard_WizardCreateWallet_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/wizard/WizardConfigure.qml"), &QmlCacheGeneratedCode::_wizard_WizardConfigure_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/DaemonManagerDialog.qml"), &QmlCacheGeneratedCode::_components_DaemonManagerDialog_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/MenuButton.qml"), &QmlCacheGeneratedCode::_components_MenuButton_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/HistoryTable.qml"), &QmlCacheGeneratedCode::_components_HistoryTable_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/BasicPanel.qml"), &QmlCacheGeneratedCode::__BasicPanel_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/MobileHeader.qml"), &QmlCacheGeneratedCode::_components_MobileHeader_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/NetworkStatusItem.qml"), &QmlCacheGeneratedCode::_components_NetworkStatusItem_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/AddressBookTable.qml"), &QmlCacheGeneratedCode::_components_AddressBookTable_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/pages/Sign.qml"), &QmlCacheGeneratedCode::_pages_Sign_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/DashboardTable.qml"), &QmlCacheGeneratedCode::_components_DashboardTable_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/IntenseTable.qml"), &QmlCacheGeneratedCode::_components_IntenseTable_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/components/Scroll.qml"), &QmlCacheGeneratedCode::_components_Scroll_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/pages/IntenseDashboard.qml"), &QmlCacheGeneratedCode::_pages_IntenseDashboard_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/wizard/WizardCreateViewOnlyWallet.qml"), &QmlCacheGeneratedCode::_wizard_WizardCreateViewOnlyWallet_qml::unit);
    QQmlPrivate::RegisterQmlUnitCacheHook registration;
    registration.version = 0;
    registration.lookupCachedQmlUnit = &lookupCachedUnit;
    QQmlPrivate::qmlregister(QQmlPrivate::QmlUnitCacheHookRegistration, &registration);
QT_PREPEND_NAMESPACE(qRegisterResourceData)(/*version*/0x01, qt_resource_tree, qt_resource_names, qt_resource_empty_payout);
}
const QQmlPrivate::CachedQmlUnit *Registry::lookupCachedUnit(const QUrl &url) {
    if (url.scheme() != QLatin1String("qrc"))
        return nullptr;
    QString resourcePath = QDir::cleanPath(url.path());
    if (resourcePath.isEmpty())
        return nullptr;
    if (!resourcePath.startsWith(QLatin1Char('/')))
        resourcePath.prepend(QLatin1Char('/'));
    return unitRegistry()->resourcePathToCachedUnit.value(resourcePath, nullptr);
}
}
int QT_MANGLE_NAMESPACE(qInitResources_qml)() {
    ::unitRegistry();
    Q_INIT_RESOURCE(qml_qmlcache);
    return 1;
}
Q_CONSTRUCTOR_FUNCTION(QT_MANGLE_NAMESPACE(qInitResources_qml));
int QT_MANGLE_NAMESPACE(qCleanupResources_qml)() {
    Q_CLEANUP_RESOURCE(qml_qmlcache);
    return 1;
}
