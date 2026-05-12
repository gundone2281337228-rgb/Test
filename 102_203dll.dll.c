typedef unsigned char   undefined;

typedef unsigned char    bool;
typedef unsigned char    byte;
typedef unsigned int    dword;
typedef pointer32 ImageBaseOffset32;

typedef long double    longdouble;
typedef long long    longlong;
typedef unsigned char    uchar;
typedef unsigned int    uint;
typedef unsigned long    ulong;
typedef unsigned char    undefined1;
typedef unsigned short    undefined2;
typedef unsigned int    undefined4;
typedef unsigned long long    undefined8;
typedef unsigned short    ushort;
typedef int    wchar_t;
typedef unsigned short    word;
typedef union IMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryUnion IMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryUnion, *PIMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryUnion;

typedef struct IMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryStruct IMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryStruct, *PIMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryStruct;

struct IMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryStruct {
    dword OffsetToDirectory:31;
    dword DataIsDirectory:1;
};

union IMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryUnion {
    dword OffsetToData;
    struct IMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryStruct IMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryStruct;
};

typedef unsigned short    wchar16;
typedef int BOOL;

typedef struct HWND__ HWND__, *PHWND__;

typedef struct HWND__ *HWND;

typedef long LONG_PTR;

typedef LONG_PTR LPARAM;

typedef BOOL (*WNDENUMPROC)(HWND, LPARAM);

struct HWND__ {
    int unused;
};

typedef struct _GUID _GUID, *P_GUID;

typedef struct _GUID GUID;

struct _GUID {
    ulong Data1;
    ushort Data2;
    ushort Data3;
    uchar Data4[8];
};

typedef GUID IID;

typedef struct _cpinfo _cpinfo, *P_cpinfo;

typedef uint UINT;

typedef uchar BYTE;

struct _cpinfo {
    UINT MaxCharSize;
    BYTE DefaultChar[2];
    BYTE LeadByte[12];
};

typedef char CHAR;

typedef CHAR *LPSTR;

typedef BOOL (*CALINFO_ENUMPROCA)(LPSTR);

typedef struct _cpinfo *LPCPINFO;

typedef ulong DWORD;

typedef DWORD LCTYPE;

typedef DWORD CALID;

typedef DWORD CALTYPE;

typedef struct _STARTUPINFOA _STARTUPINFOA, *P_STARTUPINFOA;

typedef ushort WORD;

typedef BYTE *LPBYTE;

typedef void *HANDLE;

struct _STARTUPINFOA {
    DWORD cb;
    LPSTR lpReserved;
    LPSTR lpDesktop;
    LPSTR lpTitle;
    DWORD dwX;
    DWORD dwY;
    DWORD dwXSize;
    DWORD dwYSize;
    DWORD dwXCountChars;
    DWORD dwYCountChars;
    DWORD dwFillAttribute;
    DWORD dwFlags;
    WORD wShowWindow;
    WORD cbReserved2;
    LPBYTE lpReserved2;
    HANDLE hStdInput;
    HANDLE hStdOutput;
    HANDLE hStdError;
};

typedef struct _MEMORYSTATUS _MEMORYSTATUS, *P_MEMORYSTATUS;

typedef ulong ULONG_PTR;

typedef ULONG_PTR SIZE_T;

struct _MEMORYSTATUS {
    DWORD dwLength;
    DWORD dwMemoryLoad;
    SIZE_T dwTotalPhys;
    SIZE_T dwAvailPhys;
    SIZE_T dwTotalPageFile;
    SIZE_T dwAvailPageFile;
    SIZE_T dwTotalVirtual;
    SIZE_T dwAvailVirtual;
};

typedef struct _SECURITY_ATTRIBUTES _SECURITY_ATTRIBUTES, *P_SECURITY_ATTRIBUTES;

typedef void *LPVOID;

struct _SECURITY_ATTRIBUTES {
    DWORD nLength;
    LPVOID lpSecurityDescriptor;
    BOOL bInheritHandle;
};

typedef struct _STARTUPINFOA *LPSTARTUPINFOA;

typedef struct _OVERLAPPED _OVERLAPPED, *P_OVERLAPPED;

typedef union _union_518 _union_518, *P_union_518;

typedef struct _struct_519 _struct_519, *P_struct_519;

typedef void *PVOID;

struct _struct_519 {
    DWORD Offset;
    DWORD OffsetHigh;
};

union _union_518 {
    struct _struct_519 s;
    PVOID Pointer;
};

struct _OVERLAPPED {
    ULONG_PTR Internal;
    ULONG_PTR InternalHigh;
    union _union_518 u;
    HANDLE hEvent;
};

typedef struct _SYSTEMTIME _SYSTEMTIME, *P_SYSTEMTIME;

typedef struct _SYSTEMTIME SYSTEMTIME;

struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
};

typedef struct _MEMORYSTATUS *LPMEMORYSTATUS;

typedef struct _RTL_CRITICAL_SECTION _RTL_CRITICAL_SECTION, *P_RTL_CRITICAL_SECTION;

typedef struct _RTL_CRITICAL_SECTION *PRTL_CRITICAL_SECTION;

typedef PRTL_CRITICAL_SECTION LPCRITICAL_SECTION;

typedef struct _RTL_CRITICAL_SECTION_DEBUG _RTL_CRITICAL_SECTION_DEBUG, *P_RTL_CRITICAL_SECTION_DEBUG;

typedef struct _RTL_CRITICAL_SECTION_DEBUG *PRTL_CRITICAL_SECTION_DEBUG;

typedef long LONG;

typedef struct _LIST_ENTRY _LIST_ENTRY, *P_LIST_ENTRY;

typedef struct _LIST_ENTRY LIST_ENTRY;

struct _RTL_CRITICAL_SECTION {
    PRTL_CRITICAL_SECTION_DEBUG DebugInfo;
    LONG LockCount;
    LONG RecursionCount;
    HANDLE OwningThread;
    HANDLE LockSemaphore;
    ULONG_PTR SpinCount;
};

struct _LIST_ENTRY {
    struct _LIST_ENTRY *Flink;
    struct _LIST_ENTRY *Blink;
};

struct _RTL_CRITICAL_SECTION_DEBUG {
    WORD Type;
    WORD CreatorBackTraceIndex;
    struct _RTL_CRITICAL_SECTION *CriticalSection;
    LIST_ENTRY ProcessLocksList;
    DWORD EntryCount;
    DWORD ContentionCount;
    DWORD Flags;
    WORD CreatorBackTraceIndexHigh;
    WORD SpareWORD;
};

typedef struct _OVERLAPPED *LPOVERLAPPED;

typedef struct _SECURITY_ATTRIBUTES *LPSECURITY_ATTRIBUTES;

typedef struct _SYSTEMTIME *LPSYSTEMTIME;

typedef struct _MEMORY_BASIC_INFORMATION _MEMORY_BASIC_INFORMATION, *P_MEMORY_BASIC_INFORMATION;

struct _MEMORY_BASIC_INFORMATION {
    PVOID BaseAddress;
    PVOID AllocationBase;
    DWORD AllocationProtect;
    SIZE_T RegionSize;
    DWORD State;
    DWORD Protect;
    DWORD Type;
};

typedef struct _CONTEXT _CONTEXT, *P_CONTEXT;

typedef struct _CONTEXT CONTEXT;

typedef struct _FLOATING_SAVE_AREA _FLOATING_SAVE_AREA, *P_FLOATING_SAVE_AREA;

typedef struct _FLOATING_SAVE_AREA FLOATING_SAVE_AREA;

struct _FLOATING_SAVE_AREA {
    DWORD ControlWord;
    DWORD StatusWord;
    DWORD TagWord;
    DWORD ErrorOffset;
    DWORD ErrorSelector;
    DWORD DataOffset;
    DWORD DataSelector;
    BYTE RegisterArea[80];
    DWORD Cr0NpxState;
};

struct _CONTEXT {
    DWORD ContextFlags;
    DWORD Dr0;
    DWORD Dr1;
    DWORD Dr2;
    DWORD Dr3;
    DWORD Dr6;
    DWORD Dr7;
    FLOATING_SAVE_AREA FloatSave;
    DWORD SegGs;
    DWORD SegFs;
    DWORD SegEs;
    DWORD SegDs;
    DWORD Edi;
    DWORD Esi;
    DWORD Ebx;
    DWORD Edx;
    DWORD Ecx;
    DWORD Eax;
    DWORD Ebp;
    DWORD Eip;
    DWORD SegCs;
    DWORD EFlags;
    DWORD Esp;
    DWORD SegSs;
    BYTE ExtendedRegisters[512];
};

typedef struct _EXCEPTION_RECORD _EXCEPTION_RECORD, *P_EXCEPTION_RECORD;

typedef struct _EXCEPTION_RECORD EXCEPTION_RECORD;

typedef EXCEPTION_RECORD *PEXCEPTION_RECORD;

struct _EXCEPTION_RECORD {
    DWORD ExceptionCode;
    DWORD ExceptionFlags;
    struct _EXCEPTION_RECORD *ExceptionRecord;
    PVOID ExceptionAddress;
    DWORD NumberParameters;
    ULONG_PTR ExceptionInformation[15];
};

typedef double ULONGLONG;

typedef double LONGLONG;

typedef wchar_t WCHAR;

typedef WCHAR *LPWSTR;

typedef WCHAR *LPCWSTR;

typedef long HRESULT;

typedef CHAR *LPCSTR;

typedef struct _MEMORY_BASIC_INFORMATION *PMEMORY_BASIC_INFORMATION;

typedef LONG *PLONG;

typedef CHAR *LPCH;

typedef struct _OSVERSIONINFOA _OSVERSIONINFOA, *P_OSVERSIONINFOA;

struct _OSVERSIONINFOA {
    DWORD dwOSVersionInfoSize;
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    DWORD dwBuildNumber;
    DWORD dwPlatformId;
    CHAR szCSDVersion[128];
};

typedef struct _OSVERSIONINFOA *LPOSVERSIONINFOA;

typedef CONTEXT *PCONTEXT;

typedef short SHORT;

typedef DWORD ACCESS_MASK;

typedef DWORD LCID;

typedef CHAR *PCNZCH;

typedef struct IMAGE_DOS_HEADER IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;

struct IMAGE_DOS_HEADER {
    char e_magic[2]; // Magic number
    word e_cblp; // Bytes of last page
    word e_cp; // Pages in file
    word e_crlc; // Relocations
    word e_cparhdr; // Size of header in paragraphs
    word e_minalloc; // Minimum extra paragraphs needed
    word e_maxalloc; // Maximum extra paragraphs needed
    word e_ss; // Initial (relative) SS value
    word e_sp; // Initial SP value
    word e_csum; // Checksum
    word e_ip; // Initial IP value
    word e_cs; // Initial (relative) CS value
    word e_lfarlc; // File address of relocation table
    word e_ovno; // Overlay number
    word e_res[4][4]; // Reserved words
    word e_oemid; // OEM identifier (for e_oeminfo)
    word e_oeminfo; // OEM information; e_oemid specific
    word e_res2[10][10]; // Reserved words
    dword e_lfanew; // File address of new exe header
    byte e_program[448]; // Actual DOS program
};

typedef struct tagFUNCDESC tagFUNCDESC, *PtagFUNCDESC;

typedef LONG DISPID;

typedef DISPID MEMBERID;

typedef LONG SCODE;

typedef struct tagELEMDESC tagELEMDESC, *PtagELEMDESC;

typedef struct tagELEMDESC ELEMDESC;

typedef enum tagFUNCKIND {
    FUNC_VIRTUAL=0,
    FUNC_PUREVIRTUAL=1,
    FUNC_NONVIRTUAL=2,
    FUNC_STATIC=3,
    FUNC_DISPATCH=4
} tagFUNCKIND;

typedef enum tagFUNCKIND FUNCKIND;

typedef enum tagINVOKEKIND {
    INVOKE_FUNC=1,
    INVOKE_PROPERTYGET=2,
    INVOKE_PROPERTYPUT=4,
    INVOKE_PROPERTYPUTREF=8
} tagINVOKEKIND;

typedef enum tagINVOKEKIND INVOKEKIND;

typedef enum tagCALLCONV {
    CC_FASTCALL=0,
    CC_CDECL=1,
    CC_MSCPASCAL=2,
    CC_PASCAL=3,
    CC_MACPASCAL=4,
    CC_STDCALL=5,
    CC_FPFASTCALL=6,
    CC_SYSCALL=7,
    CC_MPWCDECL=8,
    CC_MPWPASCAL=9,
    CC_MAX=10
} tagCALLCONV;

typedef enum tagCALLCONV CALLCONV;

typedef struct tagTYPEDESC tagTYPEDESC, *PtagTYPEDESC;

typedef struct tagTYPEDESC TYPEDESC;

typedef union _union_2702 _union_2702, *P_union_2702;

typedef union _union_2691 _union_2691, *P_union_2691;

typedef ushort VARTYPE;

typedef struct tagIDLDESC tagIDLDESC, *PtagIDLDESC;

typedef struct tagIDLDESC IDLDESC;

typedef struct tagPARAMDESC tagPARAMDESC, *PtagPARAMDESC;

typedef struct tagPARAMDESC PARAMDESC;

typedef struct tagARRAYDESC tagARRAYDESC, *PtagARRAYDESC;

typedef DWORD HREFTYPE;

typedef ushort USHORT;

typedef struct tagPARAMDESCEX tagPARAMDESCEX, *PtagPARAMDESCEX;

typedef struct tagPARAMDESCEX *LPPARAMDESCEX;

typedef struct tagSAFEARRAYBOUND tagSAFEARRAYBOUND, *PtagSAFEARRAYBOUND;

typedef struct tagSAFEARRAYBOUND SAFEARRAYBOUND;

typedef DWORD ULONG;

typedef struct tagVARIANT tagVARIANT, *PtagVARIANT;

typedef struct tagVARIANT VARIANT;

typedef VARIANT VARIANTARG;

typedef union _union_2683 _union_2683, *P_union_2683;

typedef struct __tagVARIANT __tagVARIANT, *P__tagVARIANT;

typedef struct tagDEC tagDEC, *PtagDEC;

typedef struct tagDEC DECIMAL;

typedef union _union_2685 _union_2685, *P_union_2685;

typedef union _union_1695 _union_1695, *P_union_1695;

typedef union _union_1697 _union_1697, *P_union_1697;

typedef float FLOAT;

typedef double DOUBLE;

typedef short VARIANT_BOOL;

typedef union tagCY tagCY, *PtagCY;

typedef union tagCY CY;

typedef double DATE;

typedef WCHAR OLECHAR;

typedef OLECHAR *BSTR;

typedef struct IUnknown IUnknown, *PIUnknown;

typedef struct IDispatch IDispatch, *PIDispatch;

typedef struct tagSAFEARRAY tagSAFEARRAY, *PtagSAFEARRAY;

typedef struct tagSAFEARRAY SAFEARRAY;

typedef int INT;

typedef struct __tagBRECORD __tagBRECORD, *P__tagBRECORD;

typedef struct _struct_1696 _struct_1696, *P_struct_1696;

typedef struct _struct_1698 _struct_1698, *P_struct_1698;

typedef struct _struct_1693 _struct_1693, *P_struct_1693;

typedef struct IUnknownVtbl IUnknownVtbl, *PIUnknownVtbl;

typedef struct IDispatchVtbl IDispatchVtbl, *PIDispatchVtbl;

typedef struct ITypeInfo ITypeInfo, *PITypeInfo;

typedef OLECHAR *LPOLESTR;

typedef struct tagDISPPARAMS tagDISPPARAMS, *PtagDISPPARAMS;

typedef struct tagDISPPARAMS DISPPARAMS;

typedef struct tagEXCEPINFO tagEXCEPINFO, *PtagEXCEPINFO;

typedef struct tagEXCEPINFO EXCEPINFO;

typedef struct IRecordInfo IRecordInfo, *PIRecordInfo;

typedef struct ITypeInfoVtbl ITypeInfoVtbl, *PITypeInfoVtbl;

typedef struct tagTYPEATTR tagTYPEATTR, *PtagTYPEATTR;

typedef struct tagTYPEATTR TYPEATTR;

typedef struct ITypeComp ITypeComp, *PITypeComp;

typedef struct tagFUNCDESC FUNCDESC;

typedef struct tagVARDESC tagVARDESC, *PtagVARDESC;

typedef struct tagVARDESC VARDESC;

typedef struct ITypeLib ITypeLib, *PITypeLib;

typedef struct IRecordInfoVtbl IRecordInfoVtbl, *PIRecordInfoVtbl;

typedef OLECHAR *LPCOLESTR;

typedef enum tagTYPEKIND {
    TKIND_ENUM=0,
    TKIND_RECORD=1,
    TKIND_MODULE=2,
    TKIND_INTERFACE=3,
    TKIND_DISPATCH=4,
    TKIND_COCLASS=5,
    TKIND_ALIAS=6,
    TKIND_UNION=7,
    TKIND_MAX=8
} tagTYPEKIND;

typedef enum tagTYPEKIND TYPEKIND;

typedef struct ITypeCompVtbl ITypeCompVtbl, *PITypeCompVtbl;

typedef enum tagDESCKIND {
    DESCKIND_NONE=0,
    DESCKIND_FUNCDESC=1,
    DESCKIND_VARDESC=2,
    DESCKIND_TYPECOMP=3,
    DESCKIND_IMPLICITAPPOBJ=4,
    DESCKIND_MAX=5
} tagDESCKIND;

typedef enum tagDESCKIND DESCKIND;

typedef union tagBINDPTR tagBINDPTR, *PtagBINDPTR;

typedef union tagBINDPTR BINDPTR;

typedef union _union_2711 _union_2711, *P_union_2711;

typedef enum tagVARKIND {
    VAR_PERINSTANCE=0,
    VAR_STATIC=1,
    VAR_CONST=2,
    VAR_DISPATCH=3
} tagVARKIND;

typedef enum tagVARKIND VARKIND;

typedef struct ITypeLibVtbl ITypeLibVtbl, *PITypeLibVtbl;

typedef struct tagTLIBATTR tagTLIBATTR, *PtagTLIBATTR;

typedef struct tagTLIBATTR TLIBATTR;

typedef enum tagSYSKIND {
    SYS_WIN16=0,
    SYS_WIN32=1,
    SYS_MAC=2,
    SYS_WIN64=3
} tagSYSKIND;

typedef enum tagSYSKIND SYSKIND;

struct _struct_1693 {
    ulong Lo;
    long Hi;
};

union tagCY {
    struct _struct_1693 s;
    LONGLONG int64;
};

struct _struct_1698 {
    ULONG Lo32;
    ULONG Mid32;
};

union _union_1697 {
    struct _struct_1698 s2;
    ULONGLONG Lo64;
};

struct _struct_1696 {
    BYTE scale;
    BYTE sign;
};

union _union_1695 {
    struct _struct_1696 s;
    USHORT signscale;
};

struct tagDEC {
    USHORT wReserved;
    union _union_1695 u;
    ULONG Hi32;
    union _union_1697 u2;
};

struct __tagBRECORD {
    PVOID pvRecord;
    struct IRecordInfo *pRecInfo;
};

union _union_2685 {
    LONGLONG llVal;
    LONG lVal;
    BYTE bVal;
    SHORT iVal;
    FLOAT fltVal;
    DOUBLE dblVal;
    VARIANT_BOOL boolVal;
    SCODE scode;
    CY cyVal;
    DATE date;
    BSTR bstrVal;
    struct IUnknown *punkVal;
    struct IDispatch *pdispVal;
    SAFEARRAY *parray;
    BYTE *pbVal;
    SHORT *piVal;
    LONG *plVal;
    LONGLONG *pllVal;
    FLOAT *pfltVal;
    DOUBLE *pdblVal;
    VARIANT_BOOL *pboolVal;
    SCODE *pscode;
    CY *pcyVal;
    DATE *pdate;
    BSTR *pbstrVal;
    struct IUnknown **ppunkVal;
    struct IDispatch **ppdispVal;
    SAFEARRAY **pparray;
    VARIANT *pvarVal;
    PVOID byref;
    CHAR cVal;
    USHORT uiVal;
    ULONG ulVal;
    ULONGLONG ullVal;
    INT intVal;
    UINT uintVal;
    DECIMAL *pdecVal;
    CHAR *pcVal;
    USHORT *puiVal;
    ULONG *pulVal;
    ULONGLONG *pullVal;
    INT *pintVal;
    UINT *puintVal;
    struct __tagBRECORD brecVal;
};

struct __tagVARIANT {
    VARTYPE vt;
    WORD wReserved1;
    WORD wReserved2;
    WORD wReserved3;
    union _union_2685 n3;
};

union _union_2683 {
    struct __tagVARIANT n2;
    DECIMAL decVal;
};

union _union_2691 {
    struct tagTYPEDESC *lptdesc;
    struct tagARRAYDESC *lpadesc;
    HREFTYPE hreftype;
};

struct tagTYPEDESC {
    union _union_2691 u;
    VARTYPE vt;
};

struct tagIDLDESC {
    ULONG_PTR dwReserved;
    USHORT wIDLFlags;
};

struct tagPARAMDESC {
    LPPARAMDESCEX pparamdescex;
    USHORT wParamFlags;
};

union _union_2702 {
    IDLDESC idldesc;
    PARAMDESC paramdesc;
};

struct tagELEMDESC {
    TYPEDESC tdesc;
    union _union_2702 u;
};

struct tagFUNCDESC {
    MEMBERID memid;
    SCODE *lprgscode;
    ELEMDESC *lprgelemdescParam;
    FUNCKIND funckind;
    INVOKEKIND invkind;
    CALLCONV callconv;
    SHORT cParams;
    SHORT cParamsOpt;
    SHORT oVft;
    SHORT cScodes;
    ELEMDESC elemdescFunc;
    WORD wFuncFlags;
};

struct tagVARIANT {
    union _union_2683 n1;
};

struct tagPARAMDESCEX {
    ULONG cBytes;
    VARIANTARG varDefaultValue;
};

union _union_2711 {
    ULONG oInst;
    VARIANT *lpvarValue;
};

struct tagVARDESC {
    MEMBERID memid;
    LPOLESTR lpstrSchema;
    union _union_2711 u;
    ELEMDESC elemdescVar;
    WORD wVarFlags;
    VARKIND varkind;
};

struct ITypeCompVtbl {
    HRESULT (*QueryInterface)(struct ITypeComp *, IID *, void **);
    ULONG (*AddRef)(struct ITypeComp *);
    ULONG (*Release)(struct ITypeComp *);
    HRESULT (*Bind)(struct ITypeComp *, LPOLESTR, ULONG, WORD, struct ITypeInfo **, DESCKIND *, BINDPTR *);
    HRESULT (*BindType)(struct ITypeComp *, LPOLESTR, ULONG, struct ITypeInfo **, struct ITypeComp **);
};

struct tagSAFEARRAYBOUND {
    ULONG cElements;
    LONG lLbound;
};

struct tagSAFEARRAY {
    USHORT cDims;
    USHORT fFeatures;
    ULONG cbElements;
    ULONG cLocks;
    PVOID pvData;
    SAFEARRAYBOUND rgsabound[1];
};

struct ITypeInfoVtbl {
    HRESULT (*QueryInterface)(struct ITypeInfo *, IID *, void **);
    ULONG (*AddRef)(struct ITypeInfo *);
    ULONG (*Release)(struct ITypeInfo *);
    HRESULT (*GetTypeAttr)(struct ITypeInfo *, TYPEATTR **);
    HRESULT (*GetTypeComp)(struct ITypeInfo *, struct ITypeComp **);
    HRESULT (*GetFuncDesc)(struct ITypeInfo *, UINT, FUNCDESC **);
    HRESULT (*GetVarDesc)(struct ITypeInfo *, UINT, VARDESC **);
    HRESULT (*GetNames)(struct ITypeInfo *, MEMBERID, BSTR *, UINT, UINT *);
    HRESULT (*GetRefTypeOfImplType)(struct ITypeInfo *, UINT, HREFTYPE *);
    HRESULT (*GetImplTypeFlags)(struct ITypeInfo *, UINT, INT *);
    HRESULT (*GetIDsOfNames)(struct ITypeInfo *, LPOLESTR *, UINT, MEMBERID *);
    HRESULT (*Invoke)(struct ITypeInfo *, PVOID, MEMBERID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *);
    HRESULT (*GetDocumentation)(struct ITypeInfo *, MEMBERID, BSTR *, BSTR *, DWORD *, BSTR *);
    HRESULT (*GetDllEntry)(struct ITypeInfo *, MEMBERID, INVOKEKIND, BSTR *, BSTR *, WORD *);
    HRESULT (*GetRefTypeInfo)(struct ITypeInfo *, HREFTYPE, struct ITypeInfo **);
    HRESULT (*AddressOfMember)(struct ITypeInfo *, MEMBERID, INVOKEKIND, PVOID *);
    HRESULT (*CreateInstance)(struct ITypeInfo *, struct IUnknown *, IID *, PVOID *);
    HRESULT (*GetMops)(struct ITypeInfo *, MEMBERID, BSTR *);
    HRESULT (*GetContainingTypeLib)(struct ITypeInfo *, struct ITypeLib **, UINT *);
    void (*ReleaseTypeAttr)(struct ITypeInfo *, TYPEATTR *);
    void (*ReleaseFuncDesc)(struct ITypeInfo *, FUNCDESC *);
    void (*ReleaseVarDesc)(struct ITypeInfo *, VARDESC *);
};

struct ITypeLibVtbl {
    HRESULT (*QueryInterface)(struct ITypeLib *, IID *, void **);
    ULONG (*AddRef)(struct ITypeLib *);
    ULONG (*Release)(struct ITypeLib *);
    UINT (*GetTypeInfoCount)(struct ITypeLib *);
    HRESULT (*GetTypeInfo)(struct ITypeLib *, UINT, struct ITypeInfo **);
    HRESULT (*GetTypeInfoType)(struct ITypeLib *, UINT, TYPEKIND *);
    HRESULT (*GetTypeInfoOfGuid)(struct ITypeLib *, GUID *, struct ITypeInfo **);
    HRESULT (*GetLibAttr)(struct ITypeLib *, TLIBATTR **);
    HRESULT (*GetTypeComp)(struct ITypeLib *, struct ITypeComp **);
    HRESULT (*GetDocumentation)(struct ITypeLib *, INT, BSTR *, BSTR *, DWORD *, BSTR *);
    HRESULT (*IsName)(struct ITypeLib *, LPOLESTR, ULONG, BOOL *);
    HRESULT (*FindName)(struct ITypeLib *, LPOLESTR, ULONG, struct ITypeInfo **, MEMBERID *, USHORT *);
    void (*ReleaseTLibAttr)(struct ITypeLib *, TLIBATTR *);
};

struct tagTLIBATTR {
    GUID guid;
    LCID lcid;
    SYSKIND syskind;
    WORD wMajorVerNum;
    WORD wMinorVerNum;
    WORD wLibFlags;
};

struct tagARRAYDESC {
    TYPEDESC tdescElem;
    USHORT cDims;
    SAFEARRAYBOUND rgbounds[1];
};

struct ITypeComp {
    struct ITypeCompVtbl *lpVtbl;
};

struct IRecordInfo {
    struct IRecordInfoVtbl *lpVtbl;
};

struct tagTYPEATTR {
    GUID guid;
    LCID lcid;
    DWORD dwReserved;
    MEMBERID memidConstructor;
    MEMBERID memidDestructor;
    LPOLESTR lpstrSchema;
    ULONG cbSizeInstance;
    TYPEKIND typekind;
    WORD cFuncs;
    WORD cVars;
    WORD cImplTypes;
    WORD cbSizeVft;
    WORD cbAlignment;
    WORD wTypeFlags;
    WORD wMajorVerNum;
    WORD wMinorVerNum;
    TYPEDESC tdescAlias;
    IDLDESC idldescType;
};

struct IRecordInfoVtbl {
    HRESULT (*QueryInterface)(struct IRecordInfo *, IID *, void **);
    ULONG (*AddRef)(struct IRecordInfo *);
    ULONG (*Release)(struct IRecordInfo *);
    HRESULT (*RecordInit)(struct IRecordInfo *, PVOID);
    HRESULT (*RecordClear)(struct IRecordInfo *, PVOID);
    HRESULT (*RecordCopy)(struct IRecordInfo *, PVOID, PVOID);
    HRESULT (*GetGuid)(struct IRecordInfo *, GUID *);
    HRESULT (*GetName)(struct IRecordInfo *, BSTR *);
    HRESULT (*GetSize)(struct IRecordInfo *, ULONG *);
    HRESULT (*GetTypeInfo)(struct IRecordInfo *, struct ITypeInfo **);
    HRESULT (*GetField)(struct IRecordInfo *, PVOID, LPCOLESTR, VARIANT *);
    HRESULT (*GetFieldNoCopy)(struct IRecordInfo *, PVOID, LPCOLESTR, VARIANT *, PVOID *);
    HRESULT (*PutField)(struct IRecordInfo *, ULONG, PVOID, LPCOLESTR, VARIANT *);
    HRESULT (*PutFieldNoCopy)(struct IRecordInfo *, ULONG, PVOID, LPCOLESTR, VARIANT *);
    HRESULT (*GetFieldNames)(struct IRecordInfo *, ULONG *, BSTR *);
    BOOL (*IsMatchingType)(struct IRecordInfo *, struct IRecordInfo *);
    PVOID (*RecordCreate)(struct IRecordInfo *);
    HRESULT (*RecordCreateCopy)(struct IRecordInfo *, PVOID, PVOID *);
    HRESULT (*RecordDestroy)(struct IRecordInfo *, PVOID);
};

struct tagDISPPARAMS {
    VARIANTARG *rgvarg;
    DISPID *rgdispidNamedArgs;
    UINT cArgs;
    UINT cNamedArgs;
};

union tagBINDPTR {
    FUNCDESC *lpfuncdesc;
    VARDESC *lpvardesc;
    struct ITypeComp *lptcomp;
};

struct IDispatch {
    struct IDispatchVtbl *lpVtbl;
};

struct IUnknownVtbl {
    HRESULT (*QueryInterface)(struct IUnknown *, IID *, void **);
    ULONG (*AddRef)(struct IUnknown *);
    ULONG (*Release)(struct IUnknown *);
};

struct IDispatchVtbl {
    HRESULT (*QueryInterface)(struct IDispatch *, IID *, void **);
    ULONG (*AddRef)(struct IDispatch *);
    ULONG (*Release)(struct IDispatch *);
    HRESULT (*GetTypeInfoCount)(struct IDispatch *, UINT *);
    HRESULT (*GetTypeInfo)(struct IDispatch *, UINT, LCID, struct ITypeInfo **);
    HRESULT (*GetIDsOfNames)(struct IDispatch *, IID *, LPOLESTR *, UINT, LCID, DISPID *);
    HRESULT (*Invoke)(struct IDispatch *, DISPID, IID *, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *);
};

struct IUnknown {
    struct IUnknownVtbl *lpVtbl;
};

struct ITypeLib {
    struct ITypeLibVtbl *lpVtbl;
};

struct ITypeInfo {
    struct ITypeInfoVtbl *lpVtbl;
};

struct tagEXCEPINFO {
    WORD wCode;
    WORD wReserved;
    BSTR bstrSource;
    BSTR bstrDescription;
    BSTR bstrHelpFile;
    DWORD dwHelpContext;
    PVOID pvReserved;
    HRESULT (*pfnDeferredFillIn)(struct tagEXCEPINFO *);
    SCODE scode;
};

typedef struct IErrorInfo IErrorInfo, *PIErrorInfo;

typedef struct IErrorInfoVtbl IErrorInfoVtbl, *PIErrorInfoVtbl;

struct IErrorInfo {
    struct IErrorInfoVtbl *lpVtbl;
};

struct IErrorInfoVtbl {
    HRESULT (*QueryInterface)(struct IErrorInfo *, IID *, void **);
    ULONG (*AddRef)(struct IErrorInfo *);
    ULONG (*Release)(struct IErrorInfo *);
    HRESULT (*GetGUID)(struct IErrorInfo *, GUID *);
    HRESULT (*GetSource)(struct IErrorInfo *, BSTR *);
    HRESULT (*GetDescription)(struct IErrorInfo *, BSTR *);
    HRESULT (*GetHelpFile)(struct IErrorInfo *, BSTR *);
    HRESULT (*GetHelpContext)(struct IErrorInfo *, DWORD *);
};

typedef struct _EXCEPTION_POINTERS _EXCEPTION_POINTERS, *P_EXCEPTION_POINTERS;

struct _EXCEPTION_POINTERS {
    PEXCEPTION_RECORD ExceptionRecord;
    PCONTEXT ContextRecord;
};

typedef int (*FARPROC)(void);

typedef struct HKEY__ HKEY__, *PHKEY__;

struct HKEY__ {
    int unused;
};

typedef DWORD *LPDWORD;

typedef WORD *LPWORD;

typedef struct HKEY__ *HKEY;

typedef HKEY *PHKEY;

typedef struct HINSTANCE__ HINSTANCE__, *PHINSTANCE__;

struct HINSTANCE__ {
    int unused;
};

typedef BOOL *LPBOOL;

typedef struct HINSTANCE__ *HINSTANCE;

typedef void *LPCVOID;

typedef HINSTANCE HMODULE;

typedef HANDLE HLOCAL;

typedef struct IMAGE_RESOURCE_DIRECTORY_ENTRY IMAGE_RESOURCE_DIRECTORY_ENTRY, *PIMAGE_RESOURCE_DIRECTORY_ENTRY;

typedef union IMAGE_RESOURCE_DIRECTORY_ENTRY_NameUnion IMAGE_RESOURCE_DIRECTORY_ENTRY_NameUnion, *PIMAGE_RESOURCE_DIRECTORY_ENTRY_NameUnion;

typedef struct IMAGE_RESOURCE_DIRECTORY_ENTRY_NameStruct IMAGE_RESOURCE_DIRECTORY_ENTRY_NameStruct, *PIMAGE_RESOURCE_DIRECTORY_ENTRY_NameStruct;

struct IMAGE_RESOURCE_DIRECTORY_ENTRY_NameStruct {
    dword NameOffset:31;
    dword NameIsString:1;
};

union IMAGE_RESOURCE_DIRECTORY_ENTRY_NameUnion {
    struct IMAGE_RESOURCE_DIRECTORY_ENTRY_NameStruct IMAGE_RESOURCE_DIRECTORY_ENTRY_NameStruct;
    dword Name;
    word Id;
};

struct IMAGE_RESOURCE_DIRECTORY_ENTRY {
    union IMAGE_RESOURCE_DIRECTORY_ENTRY_NameUnion NameUnion;
    union IMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryUnion DirectoryUnion;
};

typedef struct IMAGE_OPTIONAL_HEADER32 IMAGE_OPTIONAL_HEADER32, *PIMAGE_OPTIONAL_HEADER32;

typedef struct IMAGE_DATA_DIRECTORY IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;

struct IMAGE_DATA_DIRECTORY {
    ImageBaseOffset32 VirtualAddress;
    dword Size;
};

struct IMAGE_OPTIONAL_HEADER32 {
    word Magic;
    byte MajorLinkerVersion;
    byte MinorLinkerVersion;
    dword SizeOfCode;
    dword SizeOfInitializedData;
    dword SizeOfUninitializedData;
    ImageBaseOffset32 AddressOfEntryPoint;
    ImageBaseOffset32 BaseOfCode;
    ImageBaseOffset32 BaseOfData;
    pointer32 ImageBase;
    dword SectionAlignment;
    dword FileAlignment;
    word MajorOperatingSystemVersion;
    word MinorOperatingSystemVersion;
    word MajorImageVersion;
    word MinorImageVersion;
    word MajorSubsystemVersion;
    word MinorSubsystemVersion;
    dword Win32VersionValue;
    dword SizeOfImage;
    dword SizeOfHeaders;
    dword CheckSum;
    word Subsystem;
    word DllCharacteristics;
    dword SizeOfStackReserve;
    dword SizeOfStackCommit;
    dword SizeOfHeapReserve;
    dword SizeOfHeapCommit;
    dword LoaderFlags;
    dword NumberOfRvaAndSizes;
    struct IMAGE_DATA_DIRECTORY DataDirectory[16];
};

typedef struct IMAGE_SECTION_HEADER IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;

typedef union Misc Misc, *PMisc;

typedef enum SectionFlags {
    IMAGE_SCN_TYPE_NO_PAD=8,
    IMAGE_SCN_RESERVED_0001=16,
    IMAGE_SCN_CNT_CODE=32,
    IMAGE_SCN_CNT_INITIALIZED_DATA=64,
    IMAGE_SCN_CNT_UNINITIALIZED_DATA=128,
    IMAGE_SCN_LNK_OTHER=256,
    IMAGE_SCN_LNK_INFO=512,
    IMAGE_SCN_RESERVED_0040=1024,
    IMAGE_SCN_LNK_REMOVE=2048,
    IMAGE_SCN_LNK_COMDAT=4096,
    IMAGE_SCN_GPREL=32768,
    IMAGE_SCN_MEM_16BIT=131072,
    IMAGE_SCN_MEM_PURGEABLE=131072,
    IMAGE_SCN_MEM_LOCKED=262144,
    IMAGE_SCN_MEM_PRELOAD=524288,
    IMAGE_SCN_ALIGN_1BYTES=1048576,
    IMAGE_SCN_ALIGN_2BYTES=2097152,
    IMAGE_SCN_ALIGN_4BYTES=3145728,
    IMAGE_SCN_ALIGN_8BYTES=4194304,
    IMAGE_SCN_ALIGN_16BYTES=5242880,
    IMAGE_SCN_ALIGN_32BYTES=6291456,
    IMAGE_SCN_ALIGN_64BYTES=7340032,
    IMAGE_SCN_ALIGN_128BYTES=8388608,
    IMAGE_SCN_ALIGN_256BYTES=9437184,
    IMAGE_SCN_ALIGN_512BYTES=10485760,
    IMAGE_SCN_ALIGN_1024BYTES=11534336,
    IMAGE_SCN_ALIGN_2048BYTES=12582912,
    IMAGE_SCN_ALIGN_4096BYTES=13631488,
    IMAGE_SCN_ALIGN_8192BYTES=14680064,
    IMAGE_SCN_LNK_NRELOC_OVFL=16777216,
    IMAGE_SCN_MEM_DISCARDABLE=33554432,
    IMAGE_SCN_MEM_NOT_CACHED=67108864,
    IMAGE_SCN_MEM_NOT_PAGED=134217728,
    IMAGE_SCN_MEM_SHARED=268435456,
    IMAGE_SCN_MEM_EXECUTE=536870912,
    IMAGE_SCN_MEM_READ=1073741824,
    IMAGE_SCN_MEM_WRITE=2147483648
} SectionFlags;

union Misc {
    dword PhysicalAddress;
    dword VirtualSize;
};

struct IMAGE_SECTION_HEADER {
    char Name[8];
    union Misc Misc;
    ImageBaseOffset32 VirtualAddress;
    dword SizeOfRawData;
    dword PointerToRawData;
    dword PointerToRelocations;
    dword PointerToLinenumbers;
    word NumberOfRelocations;
    word NumberOfLinenumbers;
    enum SectionFlags Characteristics;
};

typedef struct IMAGE_RESOURCE_DATA_ENTRY IMAGE_RESOURCE_DATA_ENTRY, *PIMAGE_RESOURCE_DATA_ENTRY;

struct IMAGE_RESOURCE_DATA_ENTRY {
    dword OffsetToData;
    dword Size;
    dword CodePage;
    dword Reserved;
};

typedef struct IMAGE_RESOURCE_DIRECTORY IMAGE_RESOURCE_DIRECTORY, *PIMAGE_RESOURCE_DIRECTORY;

struct IMAGE_RESOURCE_DIRECTORY {
    dword Characteristics;
    dword TimeDateStamp;
    word MajorVersion;
    word MinorVersion;
    word NumberOfNamedEntries;
    word NumberOfIdEntries;
};

typedef struct IMAGE_DIRECTORY_ENTRY_EXPORT IMAGE_DIRECTORY_ENTRY_EXPORT, *PIMAGE_DIRECTORY_ENTRY_EXPORT;

struct IMAGE_DIRECTORY_ENTRY_EXPORT {
    dword Characteristics;
    dword TimeDateStamp;
    word MajorVersion;
    word MinorVersion;
    ImageBaseOffset32 Name;
    dword Base;
    dword NumberOfFunctions;
    dword NumberOfNames;
    ImageBaseOffset32 AddressOfFunctions;
    ImageBaseOffset32 AddressOfNames;
    ImageBaseOffset32 AddressOfNameOrdinals;
};

typedef struct IMAGE_FILE_HEADER IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

struct IMAGE_FILE_HEADER {
    word Machine; // 332
    word NumberOfSections;
    dword TimeDateStamp;
    dword PointerToSymbolTable;
    dword NumberOfSymbols;
    word SizeOfOptionalHeader;
    word Characteristics;
};

typedef struct IMAGE_NT_HEADERS32 IMAGE_NT_HEADERS32, *PIMAGE_NT_HEADERS32;

struct IMAGE_NT_HEADERS32 {
    char Signature[4];
    struct IMAGE_FILE_HEADER FileHeader;
    struct IMAGE_OPTIONAL_HEADER32 OptionalHeader;
};

typedef struct IMAGE_RESOURCE_DIR_STRING_U_12 IMAGE_RESOURCE_DIR_STRING_U_12, *PIMAGE_RESOURCE_DIR_STRING_U_12;

struct IMAGE_RESOURCE_DIR_STRING_U_12 {
    word Length;
    wchar16 NameString[6];
};

typedef BOOL (*PHANDLER_ROUTINE)(DWORD);

typedef LONG LSTATUS;

typedef ACCESS_MASK REGSAM;

typedef char *va_list;

typedef struct IUnknown *LPUNKNOWN;



int DAT_00428193;
undefined DAT_00428197;
int DAT_00428205;
char DAT_0042819f;
undefined DAT_0042819b;
char DAT_004281f4;
undefined *DAT_004316b8;
string s_Nonshared_DATA_segment_required_004281a0;
string s_Cannot_run_multiple_instances_of_004281c0;
undefined DAT_004281f5;
DWORD DAT_00428193;
undefined4 *DAT_00430834;
undefined *PTR_LAB_0040be20;
int *DAT_00430834;
undefined4 DAT_0042821c;
unicode u_ShowWindow_0042822c;
IID *riid_0042d7b0;
undefined DAT_00428242;
undefined DAT_00428244;
undefined DAT_004285ac;
undefined4 DAT_004284b4;
undefined4 DAT_00428530;
undefined DAT_00430838;
undefined LAB_0040b61c;
undefined LAB_0040b623;
undefined DAT_0040b69d;
undefined DAT_0040b6b1;
undefined DAT_0043083c;
undefined LAB_0040b7fe;
undefined LAB_004184a8;
pointer PTR_s_False_0042d3d0;
pointer PTR_DAT_0042d3d8;
undefined *PTR_PTR_00430680;
undefined DAT_0040bcb0;
undefined LAB_0040c600;
undefined LAB_0040c607;
int DAT_00430850;
int *DAT_00430854;
undefined LAB_0040c6a9;
pointer PTR_LAB_0040cf35+3_0040bedc;
undefined LAB_0040c775;
undefined LAB_0040c77c;
undefined LAB_0040c840;
undefined LAB_0040c847;
int DAT_00430858;
undefined LAB_0040c8c0;
undefined *PTR_PTR_00430684;
undefined DAT_0040bd0c;
undefined LAB_0040ca24;
undefined LAB_0040ca2b;
undefined *PTR_PTR_00430688;
undefined *PTR_PTR_0043068c;
undefined *PTR_LAB_004306b8;
undefined LAB_0040cde4;
undefined LAB_0040cdeb;
undefined *PTR_PTR_0043066c;
undefined LAB_0040cf1e;
undefined LAB_0040cf25;
undefined DAT_00413bb4;
undefined LAB_0040cfd9;
undefined LAB_0040d159;
undefined LAB_0040d160;
undefined LAB_0040d252;
pointer PTR_LAB_0040cf35+3_0040c358;
undefined LAB_0040d34d;
undefined LAB_0040d354;
undefined *PTR_PTR_004306b4;
undefined LAB_0040d4e6;
undefined LAB_0040d4ed;
undefined LAB_0040d503;
pointer PTR_LAB_0040cf35+3_0040c00c;
undefined LAB_0040d56d;
undefined FUN_0040dc54;
undefined FUN_0040df04;
undefined LAB_0040d6e9;
undefined LAB_0040d6f0;
undefined DAT_0040bd64;
undefined LAB_0040d780;
undefined LAB_0040d787;
undefined LAB_0040d846;
undefined LAB_0040d84d;
undefined LAB_0040d868;
undefined LAB_0040d8c9;
undefined LAB_0040d8d0;
undefined LAB_0040d9af;
undefined LAB_0040d9b6;
undefined LAB_0040da2d;
undefined LAB_0040da34;
undefined LAB_0040c2dc;
undefined LAB_0040dab8;
undefined LAB_0040db45;
undefined LAB_0040db4c;
undefined LAB_0040db62;
undefined LAB_0040dbf4;
undefined LAB_0040dc11;
undefined LAB_0040dc18;
undefined LAB_0040dcd3;
undefined LAB_0040dcda;
undefined LAB_0040dcf8;
undefined LAB_0040dd4f;
undefined LAB_0040dda9;
undefined LAB_0040ddb0;
undefined LAB_0040de33;
undefined LAB_0040de3a;
undefined LAB_0040ded7;
undefined LAB_0040dede;
undefined LAB_0040def4;
undefined LAB_0040df7a;
undefined LAB_0040df81;
undefined *PTR_PTR_00430694;
undefined *PTR_PTR_00430690;
undefined *PTR_PTR_0043067c;
undefined DAT_0040bc58;
undefined *PTR_PTR_00430670;
undefined *PTR_PTR_00430674;
undefined DAT_0040baf4;
undefined DAT_0040bb50;
undefined LAB_0040e88a;
undefined LAB_0040e891;
int DAT_0043085c;
undefined LAB_0040e947;
undefined LAB_0040e9cf;
undefined DAT_0040bc00;
undefined *PTR_PTR_004306a0;
undefined *PTR_PTR_004306a8;
undefined LAB_0040eacc;
undefined LAB_0040ead3;
undefined LAB_0040ebcf;
undefined LAB_0040ebec;
undefined LAB_0040ebf3;
undefined *PTR_PTR_00430678;
undefined LAB_0040ed4e;
undefined LAB_0040ed55;
undefined LAB_0040ee00;
undefined LAB_0040ee07;
undefined DAT_004135b0;
undefined LAB_0040ef46;
undefined LAB_0040ef72;
undefined *DAT_00430848;
undefined LAB_0040f012;
undefined LAB_0040f0b7;
undefined LAB_0040f241;
undefined LAB_0040f283;
undefined LAB_0040f2a5;
undefined LAB_0040f2ac;
undefined LAB_0040f313;
undefined LAB_0040f3ea;
undefined DAT_0040f5e0;
undefined DAT_0040f5f0;
undefined DAT_0040f5fc;
undefined *PTR_PTR_00430668;
undefined *PTR_PTR_004306b0;
undefined LAB_0040f7b5;
undefined LAB_0040f7bc;
undefined *PTR_PTR_004306a4;
undefined LAB_0040f932;
undefined LAB_0040f959;
undefined LAB_0040f9a4;
undefined LAB_0040f9ab;
undefined DAT_0040e71c;
undefined *PTR_DAT_00430660;
undefined LAB_0040fc27;
undefined LAB_0040fc2e;
undefined DAT_0042d3e8;
undefined *PTR_PTR_004306ac;
undefined LAB_0040fec0;
undefined LAB_0040fec7;
undefined DAT_0042d3e0;
undefined LAB_0040ff49;
undefined LAB_0040ff76;
undefined LAB_0040ff7d;
undefined LAB_004100bc;
undefined LAB_004100c3;
undefined LAB_00410329;
undefined LAB_00410330;
undefined LAB_00410371;
undefined LAB_00410378;
undefined LAB_00410414;
undefined LAB_0041041b;
HWND DAT_00430878;
undefined LAB_00410710;
undefined LAB_0041071c;
undefined DAT_004109b0;
undefined DAT_004109c0;
undefined FUN_004106e8;
undefined FUN_004106fc;
undefined *PTR_PTR_00430698;
undefined DAT_0040bdc4;
undefined *PTR_PTR_0043069c;
undefined LAB_00411001;
undefined LAB_00411008;
undefined DAT_0043084c;
LPCRITICAL_SECTION lpCriticalSection_00430860;
PVOID DAT_004309d0;
int *DAT_00430844;
int *DAT_00430850;
int *DAT_0043085c;
undefined LAB_004110cc;
undefined DAT_00430880;
undefined DAT_00430884;
undefined DAT_00430888;
undefined *PTR_PTR_00430828;
undefined LAB_00411cd3;
undefined LAB_00411cda;
undefined LAB_00411d6e;
undefined LAB_00411d75;
undefined DAT_00411b24;
undefined *PTR_DAT_004306c0;
undefined DAT_0041148c;
undefined LAB_00412295;
undefined LAB_004122b8;
undefined *PTR_PTR_0043082c;
undefined DAT_00411acc;
undefined *PTR_LAB_00430830;
undefined DAT_00411b80;
undefined LAB_00412539;
undefined LAB_00412540;
undefined DAT_0041267d;
undefined DAT_00430890;
undefined1 DAT_00430894;
int *DAT_00430898;
undefined *PTR_DAT_004306c4;
undefined *PTR_PTR_004306c8;
undefined *PTR_PTR_00430664;
char DAT_004308a0;
undefined LAB_00412816;
undefined DAT_0042d464;
undefined DAT_00412ca8;
undefined DAT_00412d4f;
undefined1 DAT_004308af;
char DAT_004308ae;
undefined DAT_00412c9c;
undefined4 DAT_00412c88;
byte DAT_004308af;
undefined LAB_00414096;
undefined LAB_0041409d;
undefined DAT_00414234;
undefined *PTR_PTR_004306d0;
undefined DAT_0042d4e8;
undefined DAT_0042d4f4;
undefined DAT_00414410;
pointer PTR_PTR_0042d508;
undefined DAT_004147e6;
undefined DAT_0042d510;
uint DAT_0042d514;
undefined *PTR_PTR_004306e0;
undefined LAB_00414e0c;
undefined LAB_00414e13;
undefined DAT_0042d518;
short DAT_0043096c;
short DAT_0043096e;
undefined LAB_00415266;
undefined LAB_0041526d;
undefined LAB_0041535e;
undefined LAB_00415365;
byte *DAT_004308c8;
undefined4 DAT_004308cc;
byte *DAT_004308b8;
byte *DAT_004308b4;
undefined4 *DAT_004308c0;
undefined4 *DAT_004308c4;
undefined LAB_00415ac9;
undefined LAB_00415ad0;
undefined DAT_00415af8;
undefined DAT_0042d4c4;
undefined1 DAT_004308b1;
undefined1 DAT_004308bc;
undefined DAT_004308fc;
undefined DAT_0043092c;
undefined DAT_00430948;
undefined DAT_00415b54;
undefined LAB_00415cb5;
undefined LAB_00415cbc;
undefined4 DAT_00430974;
undefined4 DAT_00430990;
char *DAT_004308b4;
char DAT_004308b1;
char DAT_00430970;
char *DAT_004308c8;
undefined *DAT_004308c0;
undefined *DAT_004308c4;
ushort DAT_0042d490;
int DAT_00430990;
undefined LAB_0041614a;
undefined LAB_00416151;
undefined DAT_00416164;
char DAT_004308bc;
char DAT_004308af;
undefined DAT_00416358;
undefined DAT_00416364;
undefined *PTR_PTR_004306d4;
undefined *PTR_PTR_004306d8;
undefined *PTR_PTR_004306dc;
undefined LAB_004166db;
undefined LAB_004166e2;
undefined DAT_004308d0;
undefined DAT_00430900;
undefined DAT_00430930;
undefined DAT_0043094c;
undefined1 DAT_00430970;
undefined LAB_0041678d;
undefined LAB_00416794;
undefined LAB_00416837;
undefined LAB_0041683e;
undefined DAT_0041684c;
undefined lpCalInfoEnumProc_004166ec;
undefined lpCalInfoEnumProc_00416728;
undefined LAB_00416a12;
undefined LAB_00416a19;
undefined DAT_00416a28;
undefined DAT_00416a38;
undefined DAT_00416a4c;
undefined DAT_00416a60;
undefined DAT_00416a6c;
HMODULE DAT_004309d0;
undefined *PTR_PTR_0043073c;
undefined DAT_00416c14;
undefined LAB_00416c18;
undefined LAB_00416d2a;
undefined LAB_00416d31;
undefined LAB_00416df8;
undefined LAB_00416dff;
undefined *PTR_PTR_004306e8;
undefined DAT_004136d0;
undefined4 DAT_0042d5b4;
undefined4 DAT_0042d5b8;
undefined *PTR_PTR_0043077c;
undefined *PTR_PTR_00430770;
undefined DAT_00413dd8;
undefined LAB_00416ff4;
undefined LAB_00416ffb;
undefined *PTR_PTR_00430758;
undefined *PTR_PTR_00430784;
undefined *PTR_PTR_0043072c;
undefined *PTR_PTR_0043075c;
undefined DAT_00413c10;
undefined LAB_004172ab;
undefined LAB_004172b2;
undefined *PTR_PTR_0043076c;
undefined DAT_00413728;
undefined DAT_00413780;
undefined LAB_0041738f;
undefined LAB_00417396;
undefined DAT_0042d5d4;
undefined DAT_0042d5d8;
undefined *PTR_PTR_004306e4;
int *DAT_004309b4;
undefined *PTR_PTR_00430724;
int *DAT_004309b8;
undefined *PTR_DAT_0043063c;
undefined *PTR_DAT_00430638;
undefined *PTR_DAT_00430648;
undefined *PTR_DAT_00430640;
undefined *PTR_DAT_00430644;
undefined *PTR_DAT_0043064c;
undefined *PTR_DAT_00430650;
undefined DAT_00413674;
undefined DAT_00413afc;
undefined LAB_00416f14;
undefined LAB_00417030;
undefined LAB_004170dc;
undefined LAB_004173b8;
undefined LAB_00417489;
undefined LAB_00417490;
undefined FUN_00417014;
undefined FUN_004172bc;
undefined DAT_0042d47c;
undefined DAT_0042d480;
undefined DAT_0042d484;
undefined DAT_0042d488;
undefined DAT_0042d48c;
LCID DAT_00430968;
ushort DAT_0043096c;
undefined2 DAT_0043096e;
undefined1 DAT_00430971;
undefined1 DAT_004308ac;
undefined1 DAT_004308ad;
undefined1 DAT_004308ae;
undefined1 DAT_004308b0;
undefined1 DAT_004309ac;
undefined LAB_00417c7f;
undefined LAB_00417c86;
undefined DAT_00417c94;
undefined DAT_00417cc8;
undefined DAT_00417cd4;
undefined DAT_00417d18;
undefined DAT_004308a8;
undefined4 DAT_004308b4;
undefined4 DAT_004308b8;
undefined4 DAT_004308c0;
undefined4 DAT_004308c4;
undefined4 DAT_004308c8;
undefined *PTR_PTR_00430788;
undefined *PTR_LAB_0043078c;
undefined DAT_00413ef0;
undefined LAB_00417db6;
undefined LAB_00417dbd;
undefined4 *DAT_0042d69c;
FARPROC DAT_0042d474;
undefined LAB_00414414;
undefined DAT_004309b0;
char DAT_004309c6;
undefined DAT_00417f90;
undefined LAB_0041807c;
undefined LAB_00418408;
undefined LAB_0041841c;
pointer PTR_DAT_0042d4e4;
undefined LAB_00418598;
int DAT_00430a38;
undefined DAT_00430a34;
undefined DAT_00430c00;
int UNK_00430c04;
undefined DAT_0042d6a0;
undefined DAT_0042d6e0;
undefined4 *DAT_00430e34;
int *DAT_00430e38;
undefined4 DAT_00430e3c;
undefined4 DAT_00430e18;
undefined4 DAT_00430e4c;
char DAT_00430a2d;
HLOCAL DAT_00430e74;
LPCRITICAL_SECTION lpCriticalSection_00430e1c;
undefined4 *DAT_00430e68;
undefined1 DAT_00430e14;
undefined4 *DAT_00430e60;
undefined4 DAT_00430e5c;
char DAT_00430e14;
int *DAT_00430e68;
int DAT_00430e74;
undefined4 DAT_00430e78;
undefined DAT_00430e08;
undefined DAT_00430e0c;
uint *DAT_00430e68;
uint DAT_00430e6c;
uint *DAT_00430e70;
uint DAT_00430e70;
uint *DAT_00430e60;
int DAT_00430e6c;
undefined LAB_0041a574;
undefined LAB_0041a71a;
byte *DAT_00430e70;
undefined *PTR_FUN_0042d704;
undefined *PTR_FUN_0042d708;
undefined *PTR_FUN_0042d70c;
undefined *DAT_004309e4;
undefined *DAT_004309fc;
ushort DAT_0042d6e4;
undefined LAB_0041af76;
undefined LAB_0041af7d;
byte DAT_0042d6e8;
char DAT_0042d6e8;
int *DAT_00430e8c;
int DAT_00430e90;
int DAT_00430e94;
undefined LAB_0041b62e;
uint DAT_00430a1c;
LPCVOID DAT_00430a24;
string s_Runtime_error_at_00000000_0042d710;
string s_0123456789ABCDEF_0042d738;
int DAT_00430a1c;
undefined4 DAT_00430ea4;
undefined4 DAT_00430ea8;
undefined4 DAT_00430e88;
int DAT_00430a24;
char DAT_00430a2c;
char DAT_0042d6f0;
byte DAT_00430eac;
undefined *DAT_00430a20;
undefined4 DAT_00430e94;
undefined4 *DAT_00430e98;
undefined *DAT_00430eb0;
UINT DAT_00430a1c;
string s_Error_0042d730;
undefined4 DAT_00430a1c;
undefined4 DAT_00430a24;
undefined DAT_0041bc79;
undefined *DAT_00430dcc;
undefined DAT_00430dd4;
undefined *DAT_00430dd0;
undefined LAB_0041c668;
undefined LAB_0041c66f;
undefined LAB_0041c7fd;
undefined LAB_0041c862;
undefined DAT_0042d79c;
undefined LAB_0041cbc4;
undefined LAB_0041cbcb;
undefined LAB_0041cc47;
undefined LAB_0041cc4e;
undefined UNK_0041ccc3;
undefined DAT_0041cd22;
undefined DAT_0041ce3b;
undefined4 *DAT_0042d6fc;
int *DAT_0042d700;
undefined4 *DAT_0042d700;
undefined LAB_0041d233;
undefined LAB_0041d2b8;
undefined DAT_00430e10;
undefined LAB_0041d4ee;
undefined DAT_00430dd8;
undefined DAT_00430de8;
undefined DAT_00430df8;
undefined DAT_00430a28;
undefined1 DAT_00430a2e;
undefined1 DAT_00430a2f;
undefined1 DAT_00430a30;
undefined DAT_00430e00;
undefined DAT_00430a14;
undefined thunk_FUN_0041aa4c;
undefined DAT_00430a10;
undefined DAT_00430a08;
undefined LAB_0041d5e0;
undefined4 DAT_00430eb4;
undefined *DAT_00430eb4;
undefined DAT_0041d6b4;
undefined LAB_0041d70c;
undefined *PTR_FUN_0042d828;
undefined *PTR_s_bad_alloc_exception_thrown_0042d9a8;
undefined DAT_00430eb8;
int DAT_0042d8a4;
int DAT_004316ac;
string s_borlndmm_0042d8e1;
string s_@Borlndmm@SysGetMem$qqri_0042d8ea;
string s_@Borlndmm@SysFreeMem$qqrpv_0042d903;
string s_@Borlndmm@SysReallocMem$qqrpvi_0042d91e;
undefined *PTR_FUN_0042d954;
undefined *PTR_FUN_0042d950;
undefined FUN_0041e6d4;
undefined *PTR_FUN_0042d958;
undefined FUN_0041e10c;
undefined *PTR_FUN_0042d95c;
undefined DAT_0042d94c;
undefined DAT_0042d948;
undefined FUN_0041e27c;
undefined FUN_0041e494;
int DAT_0042d940;
int DAT_0042d970;
int DAT_0042d984;
uint *DAT_0042d97c;
uint *DAT_0042d984;
uint DAT_0042d970;
undefined *PTR_DAT_0042d994;
undefined DAT_0042d980;
uint DAT_0042d998;
int DAT_0042d99c;
int DAT_0042d968;
uint DAT_0042d960;
int DAT_0042d964;
uint DAT_0042d99c;
uint DAT_0042d974;
uint DAT_0042d978;
undefined *PTR_DAT_0042d98c;
undefined4 DAT_0042d988;
int *DAT_0042d97c;
undefined *PTR_FUN_0042d814;
undefined LAB_0041eafb;
string s_IS_STRUC(base->tpMask)_0042daab;
string s_xxtype.cpp_0042dac2;
string s_IS_STRUC(derv->tpMask)_0042dacd;
string s_xxtype.cpp_0042dae4;
string s_derv->tpClass.tpcFlags_&_CF_HAS__0042daef;
string s_xxtype.cpp_0042db15;
undefined4 DAT_00430edc;
string s_topTypPtr_!=_0_&&_IS_STRUC(topTy_0042db5b;
string s_xxtype.cpp_0042db89;
string s_tgtTypPtr_!=_0_&&_IS_STRUC(tgtTy_0042db94;
string s_xxtype.cpp_0042dbc2;
string s_srcTypPtr_==_0_||_IS_STRUC(srcTy_0042dbcd;
string s_xxtype.cpp_0042dbfb;
string s___isSameTypeID(srcTypPtr,_tgtTyp_0042dc06;
string s_xxtype.cpp_0042dc30;
string s_tgtTypPtr_!=_0_&&___isSameTypeID_0042dc3b;
string s_xxtype.cpp_0042dc77;
string s_srcTypPtr_0042dc82;
string s_xxtype.cpp_0042dc8c;
undefined DAT_0042dcc9;
string s_xxtype.cpp_0042dcce;
string s_<notype>_0042da40;
string s_id->tpName_0042da49;
string s_xxtype.cpp_0042da54;
undefined DAT_0042da5f;
string s_xxtype.cpp_0042da63;
undefined DAT_0042da6e;
string s_xxtype.cpp_0042da72;
string s_tp1->tpName_0042da7d;
string s_xxtype.cpp_0042da89;
string s_tp2->tpName_0042da94;
string s_xxtype.cpp_0042daa0;
string s_!"Can't_adjust_class_address_(no_0042dd10;
string s_xxtype.cpp_0042dd4a;
undefined *DAT_0042dd58;
undefined1 *DAT_0042dd5c;
undefined LAB_0041f2f0;
undefined FUN_0041f2a0;
int *DAT_004312e8;
undefined *DAT_0042dd5c;
undefined DAT_0042dda8;
undefined DAT_0042dd94;
int DAT_0042de78;
undefined *DAT_004312f0;
undefined *DAT_004312ec;
int *DAT_00431708;
string s_Stack_Overflow!_0042de7c;
undefined FUN_0041f470;
undefined4 DAT_004312f0;
uint DAT_0042e344;
undefined DAT_004313bc;
undefined4 DAT_0042e348;
int DAT_0042e344;
undefined4 DAT_0042de94;
undefined *PTR_FUN_0042f230;
undefined LAB_00420980;
undefined *PTR_DAT_0042e414;
undefined DAT_0042de90;
undefined4 DAT_0042e344;
undefined FUN_0041fb28;
undefined *PTR_FUN_0042de8c;
HANDLE DAT_004313c0;
undefined DAT_004313c4;
int DAT_0042f2f4;
undefined DAT_00431484;
undefined DAT_0042f1d0;
int DAT_0042e608;
undefined DAT_0042e418;
undefined FUN_0041fd98;
string s_(null)_0042e9c0;
undefined1 DAT_0042e9c8;
undefined DAT_0042e9d6;
undefined1 DAT_00431489;
undefined *PTR_DAT_0042ed78;
undefined DAT_0042ea3a;
undefined DAT_0042eeec;
undefined DAT_0042eef8;
undefined DAT_0042ef02;
undefined DAT_0042ef0c;
undefined DAT_0042ef16;
undefined DAT_0042ef20;
undefined DAT_0042ef2a;
undefined DAT_0042ef34;
undefined DAT_0042ef3e;
undefined DAT_0042ef48;
undefined4 DAT_0042f02c;
undefined DAT_0042ef5c;
undefined *PTR_FUN_0042ef60;
string s_printf_:_floating_point_formats_n_0042ef6c;
undefined1 DAT_0042f0b4;
undefined1 DAT_0042f0b9;
undefined1 DAT_0042f0be;
undefined1 DAT_0042f0c3;
unicode u_-INF_0042f0c8;
unicode u_+INF_0042f0d2;
unicode u_-NAN_0042f0dc;
unicode u_+NAN_0042f0e6;
undefined4 DAT_0042f10e;
undefined4 DAT_0042f112;
undefined2 DAT_0042f116;
undefined4 DAT_0042f104;
undefined4 DAT_0042f108;
undefined2 DAT_0042f10c;
undefined4 DAT_0042f0fa;
undefined4 DAT_0042f0fe;
undefined2 DAT_0042f102;
undefined4 DAT_0042f0f0;
undefined4 DAT_0042f0f4;
undefined2 DAT_0042f0f8;
undefined4 DAT_0042f038;
undefined4 DAT_0042f034;
undefined DAT_0042f0f0;
undefined4 DAT_0042f136;
undefined4 DAT_0042f13a;
undefined2 DAT_0042f13e;
undefined4 DAT_0042f12c;
undefined4 DAT_0042f130;
undefined2 DAT_0042f134;
undefined4 DAT_0042f122;
undefined4 DAT_0042f126;
undefined2 DAT_0042f12a;
undefined4 DAT_0042f118;
undefined4 DAT_0042f11c;
undefined2 DAT_0042f120;
undefined DAT_0042f118;
byte DAT_0042f140;
byte DAT_0042f141;
byte DAT_0042f142;
byte DAT_0042f143;
undefined DAT_0043158c;
string s_Error:_system_code_page_access_f_0042f144;
undefined1 DAT_00431488;
undefined1 DAT_0043148a;
undefined DAT_00431590;
string s_%02d/%02d/%04d_%02d:%02d:%02d.%0_0042f1ac;
LPSTR param_1_00431690;
undefined lpfn_0042319c;
LPCSTR DAT_0042f1d8;
undefined *DAT_0042f1d4;
LPCVOID lpBuffer_0042f1dc;
string s_Abnormal_program_termination_0042f20c;
undefined *DAT_0042f22c;
undefined *PTR_FUN_0042f234;
undefined *PTR_FUN_0042f238;
undefined1 *DAT_004316b0;
undefined *PTR_FUN_0042f28c;
string s_No_space_for_copy_of_command_lin_0042f244;
int DAT_0042f240;
undefined2 *DAT_004316b4;
undefined *PTR_FUN_0042f290;
string s_No_space_for_copy_of_command_lin_0042f268;
undefined4 DAT_0042f2c0;
undefined DAT_0042f298;
char DAT_0042f294;
undefined FUN_004238e8;
undefined HandlerRoutine_00423ac8;
undefined DAT_0042f2e8;
int DAT_0042f2f8;
undefined DAT_004316d4;
undefined DAT_004316f0;
int DAT_004316fc;
LPCH DAT_004316cc;
int DAT_0042f23c;
int DAT_0042f2fc;
HMODULE DAT_00431700;
undefined4 DAT_0042f300;
string s_An_exception_(%08X)_occurred_dur_0042f401;
undefined4 DAT_00431700;
undefined4 DAT_0042f2fc;
undefined FUN_00423e14;
string s_(ctorMask_&_0x0100)_!=_0_||_(cto_0042f528;
string s_xx.cpp_0042f55d;
string s_(ctorMask_&_0x0080)_==_0_0042f564;
string s_xx.cpp_0042f57d;
string s_!"what?"_0042f58a;
string s_xx.cpp_0042f593;
string s_!"what?"_0042f5a0;
string s_xx.cpp_0042f5a9;
string s_(dtorMask_&_0x0080)_==_0_0042f5b0;
string s_xx.cpp_0042f5c9;
string s_!"what?"_0042f5d6;
string s_xx.cpp_0042f5df;
string s_(mfnMask_&_0x0080)_==_0_0042f5e6;
string s_xx.cpp_0042f5fe;
string s_!"what?"_0042f60b;
string s_xx.cpp_0042f614;
undefined4 DAT_00431704;
string s_cctrAddr_0042f61b;
string s_xx.cpp_0042f624;
undefined FUN_00423fe0;
string s_dtorAddr_0042f62b;
string s_xx.cpp_0042f634;
string s_argType_0042f63b;
string s_xx.cpp_0042f643;
string s___CPPexceptionList_0042f64a;
string s_xx.cpp_0042f65d;
undefined DAT_0042f664;
string s_xx.cpp_0042f667;
string s_xdrPtr->xdERRaddr_==_xl_0042f66e;
string s_xx.cpp_0042f686;
string s_dscPtr->xdERRaddr_==_errPtr_0042f68d;
string s_xx.cpp_0042f6a9;
string s_dscPtr->xdHtabAdr_==_hdtPtr_0042f6b0;
string s_xx.cpp_0042f6cc;
string s_dscPtr->xdArgCopy_==_0_0042f6d3;
string s_xx.cpp_0042f6ea;
string s_(dscPtr->xdMask_&_TM_IS_PTR)_==_0_0042f6f1;
string s_xx.cpp_0042f713;
string s_mask_&_TM_IS_PTR_0042f71a;
string s_xx.cpp_0042f72b;
string s_dscPtr->xdMask_&_TM_IS_PTR_0042f732;
string s_xx.cpp_0042f74d;
string s_dscPtr->xdTypeID_==_dscPtr->xdBa_0042f754;
string s_xx.cpp_0042f777;
string s_hdtPtr->HDcctrAddr_0042f77e;
string s_xx.cpp_0042f791;
string s_dscPtr->xdSize_==_size_0042f798;
string s_xx.cpp_0042f7af;
undefined DAT_0043170c;
string s_xdrPtr_&&_xdrPtr_==_*xdrLPP_0042f7b6;
string s_xx.cpp_0042f7d2;
string s_!"bogus_context_in_Local_unwind(_0042f7f9;
string s_xx.cpp_0042f81c;
undefined DAT_00431710;
string s_!"bogus_context_in__ExceptionHan_0042f848;
string s_xx.cpp_0042f870;
string s_varType->tpClass.tpcFlags_&_CF_H_0042f877;
string s_xx.cpp_0042f89f;
string s_varType->tpClass.tpcDtorAddr_0042f8a6;
string s_xx.cpp_0042f8c3;
string s_(errPtr->ERRcInitDtc_>=_varType-_0042f8ca;
string s_xx.cpp_0042f90a;
string s_varType->tpClass.tpcFlags_&_CF_H_0042f911;
string s_xx.cpp_0042f939;
string s_dtorCnt_<_varCount_0042f940;
string s_xx.cpp_0042f953;
string s_IS_STRUC(blType->tpMask)_0042f95a;
string s_xx.cpp_0042f973;
string s_IS_STRUC(blType->tpMask)_0042f97a;
string s_xx.cpp_0042f993;
string s_memType_0042f99a;
string s_xx.cpp_0042f9a2;
string s_memType->tpClass.tpcFlags_&_CF_H_0042f9a9;
string s_xx.cpp_0042f9d1;
string s_varType->tpMask_&_TM_IS_ARRAY_0042f9d8;
string s_xx.cpp_0042f9f6;
string s_varType->tpArr.tpaElemType->tpCl_0042f9fd;
string s_xx.cpp_0042fa38;
string s_vdtCount_0042fa3f;
string s_xx.cpp_0042fa48;
string s_etdCount_<=_elemCount_||_elemCou_0042fa4f;
string s_xx.cpp_0042fa77;
string s_dtrCount_<=_vdtCount_0042fa7e;
string s_xx.cpp_0042fa93;
string s_IS_CLASS(varType->tpMask)_0042fa9a;
string s_xx.cpp_0042fab4;
string s_((unsigned___far_*)vftAddr)[-1]_=_0042fabb;
string s_xx.cpp_0042fae0;
string s_dttPtr->dttFlags_&_(DTCVF_PTRVAL_0042fae7;
string s_xx.cpp_0042fb16;
string s_dttPtr->dttType->tpMask_&_TM_IS__0042fb1d;
string s_xx.cpp_0042fb41;
string s_dttPtr->dttType->tpPtr.tppBaseTy_0042fb48;
string s_xx.cpp_0042fb8b;
string s_IS_CLASS(dttPtr->dttType->tpMask_0042fb92;
string s_xx.cpp_0042fbe9;
string s_dtvtPtr->dttType->tpMask_&_TM_IS_0042fbf0;
string s_xx.cpp_0042fc17;
string s_varType->tpClass.tpcFlags_&_CF_H_0042fc1e;
string s_xx.cpp_0042fc46;
string s_elemType->tpClass.tpcFlags_&_CF__0042fc4d;
string s_xx.cpp_0042fc76;
string s_varType->tpMask_&_TM_IS_ARRAY_0042fc7d;
string s_xx.cpp_0042fc9b;
string s_varType->tpMask_&_TM_IS_PTR_0042fca2;
string s_xx.cpp_0042fcbe;
undefined DAT_0042fcc5;
string s_xx.cpp_0042fcc8;
string s_**BCCxh1_0042fccf;
undefined DAT_00413890;
undefined DAT_004262dc;
undefined4 DAT_004309d0;
undefined FUN_00425f6c;
undefined DAT_0042fcd8;
undefined DAT_0042fce3;
char DAT_00430109;
char DAT_00430108;
undefined DAT_00431714;
undefined DAT_00431724;
string s_%s_@_%s/%d_00430173;
string s__ASSERTE:_0043017e;
string s_%s_failed_-_%s/%d_00430189;
undefined param_2_0043019b;
string s_%s_Press_[Y]es_to_terminate,_[N]_0043019e;
undefined DAT_00426800;
undefined LAB_00426870;
string s_vt_==_rhs.vt_00430205;
string s_VARIANT.CPP_00430212;

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 __stdcall entry(undefined4 param_1,int param_2,int param_3)

{
  DWORD DVar1;
  undefined4 uVar2;
  char *pcVar3;
  int in_stack_00000000;
  
  _DAT_00428197 = DAT_00428193 << 2;
  DAT_00428205 = param_2;
  (**(code **)(&DAT_004281f5 + param_2 * 4))();
  if (DAT_00428205 == 1) {
    if (DAT_0042819f != '\0') {
      DVar1 = GetVersion();
      pcVar3 = s_Nonshared_DATA_segment_required_004281a0;
      if ((DVar1 & 0x80000000) != 0) {
        pcVar3 = s_Cannot_run_multiple_instances_of_004281c0;
      }
      FUN_004231f4((uint *)pcVar3);
      uVar2 = 0;
      goto LAB_004010f4;
    }
    DAT_0042819f = '\x01';
    FUN_00423f0c();
    FUN_0041e788();
    FUN_0041db24();
    FUN_0041e790();
    FUN_0041f3f0((undefined4 *)0x0);
  }
  _DAT_0042819b = param_1;
  if ((DAT_00428205 == 1) || (DAT_00428205 == 2)) {
    DAT_004281f4 = '\x01';
    uVar2 = param_1;
  }
  else {
    uVar2 = FUN_00423d14(0x428148,in_stack_00000000,param_1,param_2);
    DAT_004281f4 = '\0';
  }
  if (DAT_004316b8 != (code *)0x0) {
    uVar2 = (*DAT_004316b8)();
  }
  if (DAT_004281f4 != '\0') {
    uVar2 = FUN_00423d14(in_stack_00000000,param_1,param_2,param_3);
  }
LAB_004010f4:
  if (DAT_00428205 == 0) {
    FUN_0041db14();
    FUN_0041e78c();
    FUN_00423f10();
    uVar2 = FUN_004011e8();
  }
  return uVar2;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 __stdcall FUN_00401012(undefined4 param_1,int param_2,int param_3)

{
  DWORD DVar1;
  undefined4 uVar2;
  char *pcVar3;
  int in_stack_00000000;
  
  _DAT_00428197 = DAT_00428193 << 2;
  DAT_00428205 = param_2;
  (**(code **)(&DAT_004281f5 + param_2 * 4))();
  if (DAT_00428205 == 1) {
    if (DAT_0042819f != '\0') {
      DVar1 = GetVersion();
      pcVar3 = s_Nonshared_DATA_segment_required_004281a0;
      if ((DVar1 & 0x80000000) != 0) {
        pcVar3 = s_Cannot_run_multiple_instances_of_004281c0;
      }
      FUN_004231f4((uint *)pcVar3);
      uVar2 = 0;
      goto LAB_004010f4;
    }
    DAT_0042819f = '\x01';
    FUN_00423f0c();
    FUN_0041e788();
    FUN_0041db24();
    FUN_0041e790();
    FUN_0041f3f0((undefined4 *)0x0);
  }
  _DAT_0042819b = param_1;
  if ((DAT_00428205 == 1) || (DAT_00428205 == 2)) {
    DAT_004281f4 = '\x01';
    uVar2 = param_1;
  }
  else {
    uVar2 = FUN_00423d14(0x428148,in_stack_00000000,param_1,param_2);
    DAT_004281f4 = '\0';
  }
  if (DAT_004316b8 != (code *)0x0) {
    uVar2 = (*DAT_004316b8)();
  }
  if (DAT_004281f4 != '\0') {
    uVar2 = FUN_00423d14(in_stack_00000000,param_1,param_2,param_3);
  }
LAB_004010f4:
  if (DAT_00428205 == 0) {
    FUN_0041db14();
    FUN_0041e78c();
    FUN_00423f10();
    uVar2 = FUN_004011e8();
  }
  return uVar2;
}



void __stdcall FUN_0040112a(void)

{
  return;
}



// WARNING: Removing unreachable block (ram,0x00401149)

void __stdcall FUN_00401137(void)

{
  HANDLE hHeap;
  LPVOID pvVar1;
  DWORD dwFlags;
  SIZE_T dwBytes;
  
  dwBytes = 0xac;
  dwFlags = 8;
  hHeap = GetProcessHeap();
  pvVar1 = HeapAlloc(hHeap,dwFlags,dwBytes);
  if (pvVar1 == (LPVOID)0x0) {
    pvVar1 = (LPVOID)FUN_0040112a();
  }
  FUN_00423f3c(DAT_00428193,pvVar1);
  FUN_00423f50();
  return;
}



void __stdcall FUN_004011b5(void)

{
  LPVOID lpMem;
  HANDLE hHeap;
  DWORD dwFlags;
  
  lpMem = (LPVOID)FUN_00423f2c(DAT_00428193);
  if (lpMem != (LPVOID)0x0) {
    dwFlags = 8;
    hHeap = GetProcessHeap();
    HeapFree(hHeap,dwFlags,lpMem);
    FUN_00423f58();
  }
  return;
}



void __stdcall FUN_004011e8(void)

{
  FUN_004011b5();
  FUN_00423f1c(DAT_00428193);
  return;
}



void __stdcall FUN_00401202(void)

{
  int iVar1;
  
  if (DAT_00428193 == 0) {
    DAT_00428193 = FUN_00423f14();
  }
  iVar1 = FUN_00423f2c(DAT_00428193);
  if (iVar1 != 0) {
    return;
  }
  FUN_00401137();
  FUN_00423f2c(DAT_00428193);
  return;
}



undefined4 __stdcall FUN_00401238(undefined4 param_1,int param_2)

{
  int *piVar1;
  undefined4 *in_FS_OFFSET;
  int local_2c;
  undefined4 local_28;
  
  BCB_InitExceptionHandler(0x428248);
  if (param_2 == 0) {
    for (local_2c = 0; local_2c < (int)DAT_00430834[2]; local_2c = local_2c + 1) {
      piVar1 = (int *)GetFieldDescriptor(DAT_00430834,local_2c);
      if (piVar1 != (int *)0x0) {
        (**(code **)(*piVar1 + 8))();
      }
    }
    OleUninitialize();
  }
  else if (param_2 == 1) {
    OleInitialize((LPVOID)0x0);
    DAT_00430834 = (undefined4 *)FUN_004012c8((int *)&PTR_LAB_0040be20,'\x01');
  }
  *in_FS_OFFSET = local_28;
  return 1;
}



void FUN_004012c8(int *param_1,char param_2)

{
  undefined4 extraout_ECX;
  char extraout_DL;
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  
  if ('\0' < param_2) {
    param_1 = (int *)FUN_00426f74((int)param_1);
    param_2 = extraout_DL;
  }
  BCB_InitExceptionHandler(0x428490);
  FUN_0041ae24(param_1,'\0',extraout_ECX);
  *in_FS_OFFSET = local_2c;
  if (param_2 != '\0') {
    FUN_00426f81(param_1);
  }
  return;
}



void __cdecl _Form_View(undefined4 param_1,undefined4 *param_2,ULONG param_3)

{
  HRESULT HVar1;
  int iVar2;
  ulong *puVar3;
  IID *pIVar4;
  _union_2683 local_40;
  IID local_30;
  undefined4 *local_20;
  SAFEARRAY *local_1c;
  wchar16 *local_18;
  IID *local_10;
  IUnknown local_c;
  int local_8;
  
                    // 0x131c  1  _Form_View
  puVar3 = &DAT_0042821c;
  pIVar4 = &local_30;
  for (iVar2 = 4; iVar2 != 0; iVar2 = iVar2 + -1) {
    pIVar4->Data1 = *puVar3;
    puVar3 = puVar3 + 1;
    pIVar4 = (IID *)&pIVar4->Data2;
  }
  local_c.lpVtbl = (IUnknownVtbl *)0x0;
  HVar1 = GetActiveObject(&local_30,(void *)0x0,(IUnknown **)&local_10);
  if (HVar1 == 0) {
    local_8 = (**(void **)local_10->Data1)(&local_c,local_10,(void **)local_10->Data1);
  }
  else {
    local_8 = CoCreateInstance(&local_30,(LPUNKNOWN)0x0,4,(IID *)&riid_0042d7b0,&local_c.lpVtbl);
  }
  if (-1 < local_8) {
    local_18 = u_ShowWindow_0042822c;
    GetUserDefaultLCID();
    (**(code **)((local_c.lpVtbl)->QueryInterface + 0x14))();
    local_1c = SafeArrayCreateVector(0x11,0,param_3);
    local_8 = SafeArrayAccessData(local_1c,&local_20);
    if (-1 < local_8) {
      BCB_memcpy(local_20,param_2,param_3);
    }
    local_8 = SafeArrayUnaccessData(local_1c);
    VariantInit((VARIANTARG *)&local_40.n2);
    local_40.n2.vt = 0x2011;
    local_40._8_4_ = local_1c;
    GetUserDefaultLCID();
    (**(code **)((local_c.lpVtbl)->QueryInterface + 0x18))();
    FUN_0040c8f4(DAT_00430834,(int)local_c.lpVtbl);
    VariantClear((VARIANTARG *)&local_40.n2);
  }
  return;
}



int * __cdecl _SortBufData(byte *param_1,uint *param_2)

{
  uint uVar1;
  int *piVar2;
  int iVar3;
  uint uVar4;
  int *piVar5;
  int iVar6;
  byte *pbVar7;
  undefined4 *in_FS_OFFSET;
  int local_a4;
  int *local_90;
  undefined4 local_54;
  int local_24;
  uint local_20;
  int local_1c;
  uint local_18;
  int local_14;
  int local_10;
  int local_c;
  int local_8;
  
                    // 0x1454  2  _SortBufData
  BCB_InitExceptionHandler(0x4282c0);
  local_8 = 0;
  local_90 = (int *)0x0;
  uVar1 = *param_2;
  pbVar7 = param_1 + 0x10;
  if (uVar1 != 0) {
    local_10 = 0;
    local_18 = (uint)param_1[0x10] + (uint)param_1[0x11] * 0x100;
    local_20 = local_18 / 1000;
    local_14 = (uint)param_1[4] * 100 + (uint)param_1[5];
    local_24 = 0;
    FormatSweepId(&local_24,(uint)param_1[4],(uint)param_1[5]);
    AnsiString_Assign(&local_10,&local_24);
    AnsiString_Free(&local_24,2);
    local_1c = (uint)*param_1 + (uint)param_1[1] * 0x100;
    local_c = (uint)param_1[0x16] * 0x100 + (uint)param_1[0x15];
    piVar2 = GetFormatByCategory((int *)&local_20);
    iVar3 = CalcHeaderLength(piVar2);
    iVar6 = piVar2[2] * 4;
    uVar4 = uVar1 + iVar3 + iVar6 + 2;
    local_90 = AllocBuffer(uVar4);
    if (local_90 != (int *)0x0) {
      *param_2 = uVar4;
      BCB_memcpy((undefined4 *)((int)local_90 + iVar6 + iVar3 + 2),(undefined4 *)param_1,uVar1);
      iVar3 = (int)local_90 + iVar6 + 2;
      switch(local_20) {
      case 1:
        *(undefined1 *)local_90 = 2;
        DecodeAscan((int *)&local_20,(int)pbVar7,iVar3,piVar2);
        break;
      case 4:
      case 6:
        *(undefined1 *)local_90 = 1;
        DecodeGeneric((int *)&local_20,(int)pbVar7,iVar3,piVar2);
        break;
      case 5:
        *(undefined1 *)local_90 = 1;
        DecodeCalibration((int *)&local_20,(int)pbVar7,iVar3,piVar2);
        break;
      case 10:
      case 0xb:
      case 0xc:
      case 0xd:
      case 0xe:
      case 0xf:
      case 0x10:
      case 0x11:
      case 0x12:
      case 0x13:
        *(undefined1 *)local_90 = 3;
        DecodeBscan((int *)&local_20,(int)pbVar7,iVar3,piVar2);
        break;
      case 0x14:
      case 0x15:
      case 0x16:
      case 0x17:
      case 0x18:
      case 0x19:
      case 0x1a:
      case 0x1b:
      case 0x1c:
      case 0x1d:
        *(undefined1 *)local_90 = 4;
        DecodeShortProtocol((int *)&local_20,(int)pbVar7,iVar3,piVar2);
      }
      FillFieldOffsets((int)local_90 + 2,piVar2);
      *(char *)((int)local_90 + 1) = (char)piVar2[2];
      for (local_a4 = 0; local_a4 < piVar2[2]; local_a4 = local_a4 + 1) {
        piVar5 = (int *)GetFieldDescriptor(piVar2,local_a4);
        if (piVar5 != (int *)0x0) {
          AnsiString_Free(piVar5 + 1,2);
          AnsiString_Free(piVar5,2);
          BCB_FreeObject();
        }
      }
      if (piVar2 != (int *)0x0) {
        (**(code **)(*piVar2 + -4))(piVar2,3);
      }
    }
    AnsiString_Free(&local_10,2);
  }
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_54;
  return local_90;
}



void __cdecl _FreeBuffer(int param_1)

{
                    // 0x193c  3  _FreeBuffer
  if (param_1 != 0) {
    FreeAllocBuffer(param_1);
  }
  return;
}



int * __cdecl FormatSweepId(int *param_1,int param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_54;
  int local_30 [3];
  int local_24 [8];
  
  BCB_InitExceptionHandler(0x42840c);
  local_24[7] = 0;
  local_24[6] = 0;
  if (param_2 / 10 == 0) {
    local_24[4] = 0;
    FUN_00414208(param_2,local_24 + 4);
    local_24[3] = 0;
    FUN_00425e38(&DAT_00428242,local_24 + 4,local_24 + 3);
    AnsiString_Assign(local_24 + 7,local_24 + 3);
    AnsiString_Free(local_24 + 3,2);
    AnsiString_Free(local_24 + 4,2);
  }
  else {
    local_24[5] = 0;
    FUN_00414208(param_2,local_24 + 5);
    AnsiString_Assign(local_24 + 7,local_24 + 5);
    AnsiString_Free(local_24 + 5,2);
  }
  if (param_3 / 10 == 0) {
    local_24[1] = 0;
    FUN_00414208(param_3,local_24 + 1);
    local_24[0] = 0;
    FUN_00425e38(&DAT_00428244,local_24 + 1,local_24);
    AnsiString_Assign(local_24 + 6,local_24);
    AnsiString_Free(local_24,2);
    AnsiString_Free(local_24 + 1,2);
  }
  else {
    local_24[2] = 0;
    FUN_00414208(param_3,local_24 + 2);
    AnsiString_Assign(local_24 + 6,local_24 + 2);
    AnsiString_Free(local_24 + 2,2);
  }
  FUN_00425bac(local_30 + 2);
  local_30[1] = 0;
  FUN_00425d08(local_24 + 7,local_30 + 2,local_30 + 1);
  local_30[0] = 0;
  FUN_00425d08(local_30 + 1,local_24 + 6,local_30);
  AnsiString_Assign(param_1,local_30);
  AnsiString_Free(local_30,2);
  AnsiString_Free(local_30 + 1,2);
  AnsiString_Free(local_30 + 2,2);
  AnsiString_Free(local_24 + 6,2);
  AnsiString_Free(local_24 + 7,2);
  *in_FS_OFFSET = local_54;
  return param_1;
}



void __cdecl FUN_00401cb4(int *param_1,byte param_2)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_28;
  
  BCB_InitExceptionHandler(0x4284ac);
  if (param_1 != (int *)0x0) {
    AnsiString_Free(param_1 + 1,2);
    AnsiString_Free(param_1,2);
    if ((param_2 & 1) != 0) {
      BCB_FreeObject();
    }
  }
  *in_FS_OFFSET = local_28;
  return;
}



int FUN_00401d58(undefined4 *param_1,undefined4 *param_2)

{
  byte bVar1;
  int iVar2;
  int local_14;
  
  local_14 = 0;
  while( true ) {
    if ((int)param_1[2] <= local_14) {
      return -1;
    }
    iVar2 = GetFieldDescriptor(param_1,local_14);
    bVar1 = FUN_00425d94((undefined4 *)(iVar2 + 4),param_2);
    if (bVar1 != 0) break;
    local_14 = local_14 + 1;
  }
  return local_14;
}



void FUN_00401db0(int *param_1,undefined4 *param_2,undefined4 *param_3,int param_4,int param_5,
                 int param_6)

{
  int *piVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  
  BCB_InitExceptionHandler(0x428908);
  piVar1 = (int *)FUN_0041d624();
  if (piVar1 != (int *)0x0) {
    *piVar1 = 0;
    piVar1[1] = 0;
  }
  AnsiString_Assign(piVar1,param_2);
  AnsiString_Assign(piVar1 + 1,param_3);
  piVar1[2] = param_6;
  piVar1[3] = param_5;
  piVar1[4] = param_4;
  FUN_0040c8f4(param_1,(int)piVar1);
  *in_FS_OFFSET = local_2c;
  return;
}



int CalcHeaderLength(undefined4 *param_1)

{
  int iVar1;
  int local_10;
  int local_c;
  
  local_c = 0;
  if (param_1 != (undefined4 *)0x0) {
    for (local_10 = 0; local_10 < (int)param_1[2]; local_10 = local_10 + 1) {
      iVar1 = GetFieldDescriptor(param_1,local_10);
      local_c = local_c + *(int *)(iVar1 + 0x10);
    }
  }
  return local_c;
}



int FUN_00401ea4(undefined4 *param_1,int param_2)

{
  byte bVar1;
  int iVar2;
  undefined4 *in_FS_OFFSET;
  int local_38;
  int local_34;
  undefined4 local_2c;
  int local_8;
  
  local_8 = param_2;
  BCB_InitExceptionHandler(0x428938);
  FUN_00425be4(&local_8);
  local_34 = -1;
  if ((param_1 != (undefined4 *)0x0) && (param_1[2] != 0)) {
    local_34 = 0;
    for (local_38 = 0; local_38 < (int)param_1[2]; local_38 = local_38 + 1) {
      iVar2 = GetFieldDescriptor(param_1,local_38);
      bVar1 = FUN_00425d94((undefined4 *)(iVar2 + 4),&local_8);
      if (bVar1 != 0) break;
      iVar2 = GetFieldDescriptor(param_1,local_38);
      local_34 = local_34 + *(int *)(iVar2 + 0x10);
    }
  }
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return local_34;
}



void FUN_00401f54(int param_1,undefined4 *param_2,undefined4 *param_3,int *param_4)

{
  int *piVar1;
  undefined4 uVar2;
  int extraout_ECX;
  int iVar3;
  undefined1 *extraout_ECX_00;
  undefined1 *extraout_ECX_01;
  undefined1 *extraout_ECX_02;
  undefined4 *puVar4;
  undefined4 *in_FS_OFFSET;
  undefined4 local_84 [2];
  undefined4 local_7c [2];
  undefined4 local_74 [2];
  int local_6c;
  int local_68;
  int local_64;
  int local_60;
  int local_5c;
  uint local_58;
  undefined4 *local_54;
  undefined4 *local_50;
  int local_4c;
  undefined4 local_48;
  undefined2 local_38;
  int local_2c;
  byte *local_24;
  int local_20;
  byte *local_1c;
  int local_18;
  byte *local_14;
  int local_10;
  int local_c;
  int local_8;
  
  local_54 = param_3;
  local_50 = param_2;
  local_4c = param_1;
  BCB_InitExceptionHandler(0x4289c0);
  local_58 = 0;
  local_5c = 0;
  local_60 = FUN_00401d58(local_50,local_54);
  if (local_60 != -1) {
    local_38 = 0x14;
    piVar1 = FUN_00425be4(&local_8);
    local_2c = local_2c + 1;
    local_64 = FUN_00401ea4(local_50,*piVar1);
    local_2c = local_2c + -1;
    AnsiString_Free(&local_8,2);
    local_38 = 8;
    if (-1 < local_64) {
      local_68 = GetFieldDescriptor(local_50,local_60);
      iVar3 = *(int *)(local_68 + 8);
      if (iVar3 == 1) {
        local_38 = 0x20;
        piVar1 = FUN_00425be4(&local_c);
        local_2c = local_2c + 1;
        local_5c = FUN_00414274((byte *)*piVar1,0,extraout_ECX);
        local_2c = local_2c + -1;
        AnsiString_Free(&local_c,2);
        for (local_6c = 0; local_6c < *(int *)(local_68 + 0x10); local_6c = local_6c + 1) {
          *(undefined1 *)(local_4c + -1 + ((local_64 + *(int *)(local_68 + 0x10)) - local_6c)) =
               (undefined1)local_5c;
          local_5c = local_5c >> 8;
        }
      }
      else if (iVar3 == 2) {
        FUN_0041e880((undefined4 *)(local_4c + local_64),0,*(uint *)(local_68 + 0x10));
        if (*param_4 == 0) {
          iVar3 = 0;
        }
        else {
          iVar3 = *(int *)(*param_4 + -4);
        }
        if (iVar3 < *(int *)(local_68 + 0x10)) {
          if (*param_4 == 0) {
            local_58 = 0;
          }
          else {
            local_58 = *(uint *)(*param_4 + -4);
          }
        }
        else {
          local_58 = *(uint *)(local_68 + 0x10);
        }
        if (*param_4 == 0) {
          puVar4 = (undefined4 *)&DAT_004285ac;
        }
        else {
          puVar4 = (undefined4 *)*param_4;
        }
        FUN_004144a0((undefined4 *)(local_4c + local_64),puVar4,local_58);
      }
      else if (iVar3 == 3) {
        local_38 = 0x2c;
        FUN_00426ea0(local_74,param_4,'\x02');
        piVar1 = FUN_00425bac(&local_10);
        local_14 = (byte *)0x0;
        local_2c = local_2c + 2;
        FUN_00415b70((byte *)*piVar1,(int *)&local_14,extraout_ECX_00);
        uVar2 = FUN_00414238(local_14);
        *(char *)(local_4c + local_64) = (char)uVar2;
        local_2c = local_2c + -1;
        AnsiString_Free((int *)&local_14,2);
        local_2c = local_2c + -1;
        AnsiString_Free(&local_10,2);
        local_38 = 0x38;
        FUN_00426ea0(local_7c,param_4,'\x02');
        piVar1 = FUN_00425bac(&local_18);
        local_1c = (byte *)0x0;
        local_2c = local_2c + 2;
        FUN_00415b70((byte *)*piVar1,(int *)&local_1c,extraout_ECX_01);
        uVar2 = FUN_00414238(local_1c);
        *(char *)(local_4c + 1 + local_64) = (char)uVar2;
        local_2c = local_2c + -1;
        AnsiString_Free((int *)&local_1c,2);
        local_2c = local_2c + -1;
        AnsiString_Free(&local_18,2);
        local_38 = 0x44;
        FUN_00426ea0(local_84,param_4,'\x02');
        piVar1 = FUN_00425bac(&local_20);
        local_24 = (byte *)0x0;
        local_2c = local_2c + 2;
        FUN_00415b70((byte *)*piVar1,(int *)&local_24,extraout_ECX_02);
        uVar2 = FUN_00414238(local_24);
        *(char *)(local_4c + 2 + local_64) = (char)uVar2;
        local_2c = local_2c + -1;
        AnsiString_Free((int *)&local_24,2);
        local_2c = local_2c + -1;
        AnsiString_Free(&local_20,2);
      }
    }
  }
  *in_FS_OFFSET = local_48;
  return;
}



void FillFieldOffsets(int param_1,undefined4 *param_2)

{
  int iVar1;
  int local_14;
  short local_10;
  short local_e;
  undefined4 *local_c;
  int local_8;
  
  local_e = *(short *)(param_2 + 2) << 2;
  local_10 = 0;
  local_c = param_2;
  local_8 = param_1;
  for (local_14 = 0; local_14 < (int)local_c[2]; local_14 = local_14 + 1) {
    iVar1 = GetFieldDescriptor(local_c,local_14);
    BCB_memcpy((undefined4 *)(local_14 * 4 + local_8),(undefined4 *)&local_e,2);
    local_10 = *(short *)(iVar1 + 0x10);
    BCB_memcpy((undefined4 *)(local_14 * 4 + local_8 + 2),(undefined4 *)&local_10,2);
    local_e = local_e + local_10;
  }
  return;
}



void __cdecl DispatchByTypeVar(int *param_1,int param_2,int param_3,undefined4 *param_4)

{
  int iVar1;
  
  iVar1 = param_1[3];
  if (iVar1 < 0x282) {
    if (iVar1 < 0x186) {
      if (iVar1 - 0x96U < 2) {
        DecodeTypeVar_Ascan(param_1,param_2,param_3,param_4);
        return;
      }
      if ((1 < iVar1 - 0x16aU) && (1 < iVar1 - 0x17cU)) {
        return;
      }
    }
    else if (1 < iVar1 - 0x186U) {
      if ((1 < iVar1 - 0x1baU) && (1 < iVar1 - 0x1e0U)) {
        return;
      }
      goto LAB_00402382;
    }
  }
  else if (iVar1 < 0x2da) {
    if (iVar1 - 0x282U < 2) {
LAB_00402382:
      DecodeTypeVar_Composite(param_1,param_2,param_3,param_4);
      return;
    }
    if ((3 < iVar1 - 0x2a8U) && (1 < iVar1 - 0x2c8U)) {
      return;
    }
  }
  else if (((1 < iVar1 - 0x2daU) && (1 < iVar1 - 0x2e4U)) && (1 < iVar1 - 0x30cU)) {
    if (1 < iVar1 - 0x34aU) {
      return;
    }
    goto LAB_00402382;
  }
  DecodeTypeVar_Default(param_1,param_2,param_3,param_4);
  return;
}



void __cdecl DecodeDate(int param_1,int param_2,undefined4 *param_3,int param_4)

{
  byte bVar1;
  byte bVar2;
  char cVar3;
  undefined2 extraout_var;
  uint uVar4;
  undefined4 extraout_EDX;
  undefined4 extraout_EDX_00;
  undefined4 *in_FS_OFFSET;
  int *piVar5;
  ushort local_44;
  ushort local_40;
  undefined2 local_3c;
  undefined4 local_38;
  int local_14 [4];
  
  BCB_InitExceptionHandler(0x428a60);
  local_14[3] = 0;
  uVar4 = *(byte *)(param_1 + 2 + param_4) + 2000;
  bVar1 = *(byte *)(param_1 + 1 + param_4);
  bVar2 = *(byte *)(param_1 + param_4);
  cVar3 = FUN_00402874(uVar4,(uint)bVar1,(uint)bVar2);
  if (cVar3 == '\0') {
    FUN_00414dac(CONCAT22(extraout_var,2000),1,1);
    local_14[1] = 0;
    FUN_00415b58(local_14 + 1,extraout_EDX_00,(undefined1 *)0x0);
    AnsiString_Assign(local_14 + 3,local_14 + 1);
    AnsiString_Free(local_14 + 1,2);
  }
  else {
    local_44 = (ushort)bVar2;
    local_40 = (ushort)bVar1;
    local_3c = (undefined2)uVar4;
    FUN_00414dac(CONCAT22(extraout_var,local_3c),local_40,local_44);
    local_14[2] = 0;
    FUN_00415b58(local_14 + 2,extraout_EDX,(undefined1 *)0x0);
    AnsiString_Assign(local_14 + 3,local_14 + 2);
    AnsiString_Free(local_14 + 2,2);
  }
  piVar5 = local_14 + 3;
  FUN_00425bac(local_14);
  FUN_00401f54(param_2,param_3,local_14,piVar5);
  AnsiString_Free(local_14,2);
  AnsiString_Free(local_14 + 3,2);
  *in_FS_OFFSET = local_38;
  return;
}



void __cdecl DecodeTime(int param_1,int param_2,undefined4 *param_3,int param_4)

{
  char cVar1;
  undefined4 *in_FS_OFFSET;
  char **ppcVar2;
  undefined4 local_34;
  int local_10;
  int local_c;
  char *local_8;
  
  BCB_InitExceptionHandler(0x428ae4);
  local_8 = (char *)0x0;
  cVar1 = FUN_00402954((uint)*(byte *)(param_1 + param_4),(uint)*(byte *)(param_1 + 1 + param_4));
  if (cVar1 == '\0') {
    FUN_00425bac(&local_c);
    AnsiString_Assign((int *)&local_8,&local_c);
    AnsiString_Free(&local_c,2);
  }
  else {
    FUN_00425e18((int *)&local_8,5);
    FUN_00425b28((int *)&local_8,1);
    FUN_00425e04((int *)&local_8);
    *local_8 = (char)(((ulonglong)*(byte *)(param_1 + param_4) % 100) / 10) + '0';
    FUN_00425b28((int *)&local_8,2);
    FUN_00425e04((int *)&local_8);
    local_8[1] = (char)(((ulonglong)*(byte *)(param_1 + param_4) % 100) % 10) + '0';
    FUN_00425b28((int *)&local_8,3);
    FUN_00425e04((int *)&local_8);
    local_8[2] = ':';
    FUN_00425b28((int *)&local_8,4);
    FUN_00425e04((int *)&local_8);
    local_8[3] = (char)(((ulonglong)*(byte *)(param_1 + 1 + param_4) % 100) / 10) + '0';
    FUN_00425b28((int *)&local_8,5);
    FUN_00425e04((int *)&local_8);
    local_8[4] = (char)(((ulonglong)*(byte *)(param_1 + 1 + param_4) % 100) % 10) + '0';
  }
  ppcVar2 = &local_8;
  FUN_00425bac(&local_10);
  FUN_00401f54(param_2,param_3,&local_10,(int *)ppcVar2);
  AnsiString_Free(&local_10,2);
  AnsiString_Free((int *)&local_8,2);
  *in_FS_OFFSET = local_34;
  return;
}



int * __cdecl FUN_00402708(int *param_1,int param_2,int param_3)

{
  bool bVar1;
  int *piVar2;
  int iVar3;
  undefined4 *puVar4;
  char *pcVar5;
  undefined4 *puVar6;
  undefined4 *in_FS_OFFSET;
  byte bVar7;
  int local_134;
  int local_130;
  undefined4 local_128 [31];
  char local_ac [124];
  undefined4 local_30;
  undefined2 local_20;
  int local_14;
  int local_c [2];
  
  bVar7 = 0;
  BCB_InitExceptionHandler(0x428b6c);
  puVar4 = &DAT_004284b4;
  pcVar5 = local_ac;
  for (iVar3 = 0x1f; iVar3 != 0; iVar3 = iVar3 + -1) {
    *(undefined4 *)pcVar5 = *puVar4;
    puVar4 = puVar4 + (uint)bVar7 * -2 + 1;
    pcVar5 = pcVar5 + ((uint)bVar7 * -2 + 1) * 4;
  }
  puVar4 = &DAT_00428530;
  puVar6 = local_128;
  for (iVar3 = 0x1f; iVar3 != 0; iVar3 = iVar3 + -1) {
    *puVar6 = *puVar4;
    puVar4 = puVar4 + (uint)bVar7 * -2 + 1;
    puVar6 = puVar6 + (uint)bVar7 * -2 + 1;
  }
  local_c[1] = 0;
  local_14 = local_14 + 1;
  bVar1 = false;
  local_130 = 0;
  if (0 < param_3) {
    do {
      local_134 = 0;
      do {
        if (*(char *)(param_2 + -1 + (param_3 - local_130)) == local_ac[local_134]) {
          if ((local_ac[local_134] != '\0') || (bVar1)) {
            bVar1 = true;
            local_20 = 0x20;
            piVar2 = FUN_00425c20(local_c);
            local_14 = local_14 + 1;
            FUN_00425cf4(local_c + 1,piVar2);
            local_14 = local_14 + -1;
            AnsiString_Free(local_c,2);
          }
          break;
        }
        local_134 = local_134 + 1;
      } while (local_134 < 0x7c);
      local_130 = local_130 + 1;
    } while (local_130 < param_3);
  }
  local_20 = 0x2c;
  AnsiString_Assign(param_1,local_c + 1);
  local_20 = 0x38;
  local_14 = local_14 + -1;
  AnsiString_Free(local_c + 1,2);
  *in_FS_OFFSET = local_30;
  return param_1;
}



undefined1 __cdecl FUN_00402874(uint param_1,int param_2,int param_3)

{
  uint uVar1;
  undefined1 local_5;
  
  local_5 = 0;
  if ((2000 < (int)param_1) && (param_2 < 0xd)) {
    switch(param_2) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 0xc:
      if ((0 < param_3) && (param_3 < 0x20)) {
        local_5 = 1;
      }
      break;
    case 4:
    case 6:
    case 9:
    case 0xb:
      if ((0 < param_3) && (param_3 < 0x1f)) {
        local_5 = 1;
      }
    case 2:
      uVar1 = param_1 & 0x80000003;
      if ((int)uVar1 < 0) {
        uVar1 = (uVar1 - 1 | 0xfffffffc) + 1;
      }
      if (uVar1 == 0) {
        if ((int)param_1 % 100 == 0) {
          if ((0 < param_3) && (param_3 < 0x1e)) {
            local_5 = 1;
          }
        }
        else if ((0 < param_3) && (param_3 < 0x1d)) {
          local_5 = 1;
        }
      }
      else if ((0 < param_3) && (param_3 < 0x1d)) {
        local_5 = 1;
      }
    }
  }
  return local_5;
}



undefined1 __cdecl FUN_00402954(int param_1,int param_2)

{
  undefined1 local_5;
  
  local_5 = 0;
  if ((((-1 < param_1) && (param_1 < 0x18)) && (-1 < param_2)) && (param_2 < 0x3c)) {
    local_5 = 1;
  }
  return local_5;
}



void __cdecl DecodeAscan(int *param_1,int param_2,int param_3,undefined4 *param_4)

{
  int iVar1;
  undefined4 *in_FS_OFFSET;
  int *piVar2;
  undefined4 local_64;
  int local_40;
  int local_3c [3];
  int local_30;
  int local_2c;
  int local_28 [3];
  int local_1c [3];
  int local_10;
  int local_c;
  int local_8;
  
  BCB_InitExceptionHandler(0x428c98);
  FUN_00425bac(&local_10);
  piVar2 = &local_10;
  FUN_00425bac(&local_c);
  FUN_00401f54(param_3,param_4,&local_c,piVar2);
  AnsiString_Free(&local_10,2);
  AnsiString_Free(&local_c,2);
  local_1c[1] = 0;
  FUN_00414208(param_1[2] % 1000,local_1c + 1);
  piVar2 = local_1c + 1;
  FUN_00425bac(local_1c + 2);
  FUN_00401f54(param_3,param_4,local_1c + 2,piVar2);
  AnsiString_Free(local_1c + 1,2);
  AnsiString_Free(local_1c + 2,2);
  FUN_00425bac(&local_8);
  piVar2 = &local_8;
  FUN_00425bac(local_1c);
  FUN_00401f54(param_3,param_4,local_1c,piVar2);
  AnsiString_Free(local_1c,2);
  DispatchByTypeVar(param_1,param_2,param_3,param_4);
  local_28[1] = 0;
  FUN_00414208(param_1[5],local_28 + 1);
  piVar2 = local_28 + 1;
  FUN_00425bac(local_28 + 2);
  FUN_00401f54(param_3,param_4,local_28 + 2,piVar2);
  AnsiString_Free(local_28 + 1,2);
  AnsiString_Free(local_28 + 2,2);
  FUN_00425bac(&local_2c);
  piVar2 = &local_2c;
  FUN_00425bac(local_28);
  FUN_00401f54(param_3,param_4,local_28,piVar2);
  AnsiString_Free(&local_2c,2);
  AnsiString_Free(local_28,2);
  piVar2 = param_1 + 4;
  FUN_00425bac(&local_30);
  FUN_00401f54(param_3,param_4,&local_30,piVar2);
  AnsiString_Free(&local_30,2);
  iVar1 = FUN_0040381c((byte *)(param_2 + 0xe));
  local_3c[1] = 0;
  FUN_00414208(iVar1,local_3c + 1);
  piVar2 = local_3c + 1;
  FUN_00425bac(local_3c + 2);
  FUN_00401f54(param_3,param_4,local_3c + 2,piVar2);
  AnsiString_Free(local_3c + 1,2);
  AnsiString_Free(local_3c + 2,2);
  local_40 = 0;
  FUN_00414208(param_1[1],&local_40);
  piVar2 = &local_40;
  FUN_00425bac(local_3c);
  FUN_00401f54(param_3,param_4,local_3c,piVar2);
  AnsiString_Free(&local_40,2);
  AnsiString_Free(local_3c,2);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_64;
  return;
}



void __cdecl DecodeGeneric(int *param_1,int param_2,int param_3,undefined4 *param_4)

{
  undefined4 *in_FS_OFFSET;
  int *piVar1;
  undefined4 local_5c;
  int local_38;
  int local_34;
  int local_30;
  int local_2c;
  int local_28 [3];
  int local_1c [3];
  int local_10;
  int local_c;
  int local_8;
  
  BCB_InitExceptionHandler(0x428de4);
  FUN_00425bac(&local_10);
  piVar1 = &local_10;
  FUN_00425bac(&local_c);
  FUN_00401f54(param_3,param_4,&local_c,piVar1);
  AnsiString_Free(&local_10,2);
  AnsiString_Free(&local_c,2);
  local_1c[1] = 0;
  FUN_00414208(param_1[2] % 1000,local_1c + 1);
  piVar1 = local_1c + 1;
  FUN_00425bac(local_1c + 2);
  FUN_00401f54(param_3,param_4,local_1c + 2,piVar1);
  AnsiString_Free(local_1c + 1,2);
  AnsiString_Free(local_1c + 2,2);
  FUN_00425bac(&local_8);
  piVar1 = &local_8;
  FUN_00425bac(local_1c);
  FUN_00401f54(param_3,param_4,local_1c,piVar1);
  AnsiString_Free(local_1c,2);
  DispatchByTypeVar(param_1,param_2,param_3,param_4);
  local_28[1] = 0;
  FUN_00414208(param_1[5],local_28 + 1);
  piVar1 = local_28 + 1;
  FUN_00425bac(local_28 + 2);
  FUN_00401f54(param_3,param_4,local_28 + 2,piVar1);
  AnsiString_Free(local_28 + 1,2);
  AnsiString_Free(local_28 + 2,2);
  FUN_00425bac(&local_2c);
  piVar1 = &local_2c;
  FUN_00425bac(local_28);
  FUN_00401f54(param_3,param_4,local_28,piVar1);
  AnsiString_Free(&local_2c,2);
  AnsiString_Free(local_28,2);
  piVar1 = param_1 + 4;
  FUN_00425bac(&local_30);
  FUN_00401f54(param_3,param_4,&local_30,piVar1);
  AnsiString_Free(&local_30,2);
  local_38 = 0;
  FUN_00414208(param_1[1],&local_38);
  piVar1 = &local_38;
  FUN_00425bac(&local_34);
  FUN_00401f54(param_3,param_4,&local_34,piVar1);
  AnsiString_Free(&local_38,2);
  AnsiString_Free(&local_34,2);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_5c;
  return;
}



void __cdecl DecodeCalibration(int *param_1,int param_2,int param_3,undefined4 *param_4)

{
  undefined4 *in_FS_OFFSET;
  int *piVar1;
  undefined4 local_5c;
  int local_38;
  int local_34;
  int local_30;
  int local_2c;
  int local_28 [3];
  int local_1c [3];
  int local_10;
  int local_c;
  int local_8;
  
  BCB_InitExceptionHandler(0x428f24);
  FUN_00425bac(&local_10);
  piVar1 = &local_10;
  FUN_00425bac(&local_c);
  FUN_00401f54(param_3,param_4,&local_c,piVar1);
  AnsiString_Free(&local_10,2);
  AnsiString_Free(&local_c,2);
  local_1c[1] = 0;
  FUN_00414208(param_1[2] % 1000,local_1c + 1);
  piVar1 = local_1c + 1;
  FUN_00425bac(local_1c + 2);
  FUN_00401f54(param_3,param_4,local_1c + 2,piVar1);
  AnsiString_Free(local_1c + 1,2);
  AnsiString_Free(local_1c + 2,2);
  FUN_00425bac(&local_8);
  piVar1 = &local_8;
  FUN_00425bac(local_1c);
  FUN_00401f54(param_3,param_4,local_1c,piVar1);
  AnsiString_Free(local_1c,2);
  local_28[1] = 0;
  FUN_00414208(param_1[5],local_28 + 1);
  piVar1 = local_28 + 1;
  FUN_00425bac(local_28 + 2);
  FUN_00401f54(param_3,param_4,local_28 + 2,piVar1);
  AnsiString_Free(local_28 + 1,2);
  AnsiString_Free(local_28 + 2,2);
  FUN_00425bac(&local_2c);
  piVar1 = &local_2c;
  FUN_00425bac(local_28);
  FUN_00401f54(param_3,param_4,local_28,piVar1);
  AnsiString_Free(&local_2c,2);
  AnsiString_Free(local_28,2);
  DispatchByTypeVar(param_1,param_2,param_3,param_4);
  piVar1 = param_1 + 4;
  FUN_00425bac(&local_30);
  FUN_00401f54(param_3,param_4,&local_30,piVar1);
  AnsiString_Free(&local_30,2);
  local_38 = 0;
  FUN_00414208(param_1[1],&local_38);
  piVar1 = &local_38;
  FUN_00425bac(&local_34);
  FUN_00401f54(param_3,param_4,&local_34,piVar1);
  AnsiString_Free(&local_38,2);
  AnsiString_Free(&local_34,2);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_5c;
  return;
}



void __cdecl DecodeBscan(int *param_1,int param_2,int param_3,undefined4 *param_4)

{
  undefined4 *in_FS_OFFSET;
  int *piVar1;
  undefined4 local_54;
  int local_30;
  int local_2c;
  int local_28;
  int local_24;
  int local_20 [3];
  int local_14;
  int local_10;
  int local_c;
  int local_8;
  
  BCB_InitExceptionHandler(0x429034);
  FUN_00425bac(&local_c);
  piVar1 = &local_c;
  FUN_00425bac(&local_8);
  FUN_00401f54(param_3,param_4,&local_8,piVar1);
  AnsiString_Free(&local_c,2);
  AnsiString_Free(&local_8,2);
  local_14 = 0;
  FUN_00414208(param_1[2] % 10000,&local_14);
  piVar1 = &local_14;
  FUN_00425bac(&local_10);
  FUN_00401f54(param_3,param_4,&local_10,piVar1);
  AnsiString_Free(&local_14,2);
  AnsiString_Free(&local_10,2);
  DispatchByTypeVar(param_1,param_2,param_3,param_4);
  local_20[1] = 0;
  FUN_00414208(param_1[5],local_20 + 1);
  piVar1 = local_20 + 1;
  FUN_00425bac(local_20 + 2);
  FUN_00401f54(param_3,param_4,local_20 + 2,piVar1);
  AnsiString_Free(local_20 + 1,2);
  AnsiString_Free(local_20 + 2,2);
  FUN_00425bac(&local_24);
  piVar1 = &local_24;
  FUN_00425bac(local_20);
  FUN_00401f54(param_3,param_4,local_20,piVar1);
  AnsiString_Free(&local_24,2);
  AnsiString_Free(local_20,2);
  piVar1 = param_1 + 4;
  FUN_00425bac(&local_28);
  FUN_00401f54(param_3,param_4,&local_28,piVar1);
  AnsiString_Free(&local_28,2);
  local_30 = 0;
  FUN_00414208(param_1[1],&local_30);
  piVar1 = &local_30;
  FUN_00425bac(&local_2c);
  FUN_00401f54(param_3,param_4,&local_2c,piVar1);
  AnsiString_Free(&local_30,2);
  AnsiString_Free(&local_2c,2);
  *in_FS_OFFSET = local_54;
  return;
}



void __cdecl DecodeShortProtocol(int *param_1,int param_2,int param_3,undefined4 *param_4)

{
  VARIANTARG *pVVar1;
  undefined4 *in_FS_OFFSET;
  int *piVar2;
  undefined4 local_80;
  int local_5c;
  int local_58 [3];
  int local_4c;
  int local_48;
  int local_44 [3];
  int local_38;
  VARIANTARG local_34;
  VARIANTARG local_24;
  int local_14;
  int local_10;
  int local_c;
  int local_8;
  
  BCB_InitExceptionHandler(0x429160);
  FUN_00425bac(&local_c);
  piVar2 = &local_c;
  FUN_00425bac(&local_8);
  FUN_00401f54(param_3,param_4,&local_8,piVar2);
  AnsiString_Free(&local_c,2);
  AnsiString_Free(&local_8,2);
  local_14 = 0;
  FUN_00414208(param_1[2] % 10000,&local_14);
  piVar2 = &local_14;
  pVVar1 = FUN_004268ec(&local_34);
  FUN_00426970(&local_24,100);
  FUN_00403734(piVar2,(uint *)&local_24,pVVar1);
  local_38 = 0;
  FUN_00426b00(&local_34,&local_38);
  piVar2 = &local_38;
  FUN_00425bac(&local_10);
  FUN_00401f54(param_3,param_4,&local_10,piVar2);
  AnsiString_Free(&local_38,2);
  FUN_00426a08(&local_34,2);
  FUN_00426a08(&local_24,2);
  AnsiString_Free(&local_14,2);
  AnsiString_Free(&local_10,2);
  DispatchByTypeVar(param_1,param_2,param_3,param_4);
  local_44[1] = 0;
  FUN_00414208(param_1[5],local_44 + 1);
  piVar2 = local_44 + 1;
  FUN_00425bac(local_44 + 2);
  FUN_00401f54(param_3,param_4,local_44 + 2,piVar2);
  AnsiString_Free(local_44 + 1,2);
  AnsiString_Free(local_44 + 2,2);
  FUN_00425bac(&local_48);
  piVar2 = &local_48;
  FUN_00425bac(local_44);
  FUN_00401f54(param_3,param_4,local_44,piVar2);
  AnsiString_Free(&local_48,2);
  AnsiString_Free(local_44,2);
  piVar2 = param_1 + 4;
  FUN_00425bac(&local_4c);
  FUN_00401f54(param_3,param_4,&local_4c,piVar2);
  AnsiString_Free(&local_4c,2);
  local_58[1] = 0;
  FUN_00414208(param_1[1],local_58 + 1);
  piVar2 = local_58 + 1;
  FUN_00425bac(local_58 + 2);
  FUN_00401f54(param_3,param_4,local_58 + 2,piVar2);
  AnsiString_Free(local_58 + 1,2);
  AnsiString_Free(local_58 + 2,2);
  local_5c = 0;
  FUN_00402708(&local_5c,param_2 + 0x11,6);
  piVar2 = &local_5c;
  FUN_00425bac(local_58);
  FUN_00401f54(param_3,param_4,local_58,piVar2);
  AnsiString_Free(&local_5c,2);
  AnsiString_Free(local_58,2);
  *in_FS_OFFSET = local_80;
  return;
}



VARIANTARG * FUN_00403734(undefined4 *param_1,uint *param_2,VARIANTARG *param_3)

{
  VARIANTARG *pVVar1;
  VARIANTARG *pVVar2;
  undefined4 *in_FS_OFFSET;
  undefined4 local_4c;
  VARIANTARG local_24;
  VARIANTARG local_14;
  
  BCB_InitExceptionHandler(0x42b65c);
  pVVar1 = FUN_004268ec(&local_24);
  pVVar2 = FUN_004269b4(&local_14,param_1);
  FUN_00426a8c((undefined4 *)pVVar2,param_2,pVVar1);
  FUN_00426a30(param_3,&local_24);
  FUN_00426a08(&local_24,2);
  FUN_00426a08(&local_14,2);
  *in_FS_OFFSET = local_4c;
  return param_3;
}



int __cdecl FUN_0040381c(byte *param_1)

{
  return (uint)*param_1 + (uint)param_1[1] * 0x100;
}



void __cdecl DecodeTypeVar_Default(int *param_1,int param_2,int param_3,undefined4 *param_4)

{
  undefined4 *in_FS_OFFSET;
  int *piVar1;
  undefined4 local_b4;
  int local_90 [3];
  int local_84;
  int local_80 [3];
  int local_74;
  int local_70 [3];
  int local_64;
  int local_60 [3];
  int local_54;
  int local_50 [3];
  int local_44;
  int local_40 [3];
  int local_34;
  int local_30 [3];
  int local_24;
  int local_20 [3];
  int local_14;
  int local_10 [3];
  
  BCB_InitExceptionHandler(0x4293fc);
  DecodeDate(param_2,param_3,param_4,7);
  DecodeTime(param_2,param_3,param_4,10);
  local_10[2] = 0;
  if (*param_1 - 4U < 3) {
    local_10[1] = 0;
    FUN_00405cf8(local_10 + 1,param_1,param_2);
    AnsiString_Assign(local_10 + 2,local_10 + 1);
    AnsiString_Free(local_10 + 1,2);
    piVar1 = local_10 + 2;
    FUN_00425bac(local_10);
    FUN_00401f54(param_3,param_4,local_10,piVar1);
    AnsiString_Free(local_10,2);
    local_14 = 0;
    FUN_00406318(&local_14,param_1,param_2);
    AnsiString_Assign(local_10 + 2,&local_14);
    AnsiString_Free(&local_14,2);
    piVar1 = local_10 + 2;
    FUN_00425bac(local_20 + 2);
    FUN_00401f54(param_3,param_4,local_20 + 2,piVar1);
    AnsiString_Free(local_20 + 2,2);
    local_20[1] = 0;
    FUN_004055ac(local_20 + 1,param_1,param_2);
    AnsiString_Assign(local_10 + 2,local_20 + 1);
    AnsiString_Free(local_20 + 1,2);
    piVar1 = local_10 + 2;
    FUN_00425bac(local_20);
    FUN_00401f54(param_3,param_4,local_20,piVar1);
    AnsiString_Free(local_20,2);
    local_24 = 0;
    FUN_004056b4(&local_24,param_1,param_2);
    AnsiString_Assign(local_10 + 2,&local_24);
    AnsiString_Free(&local_24,2);
    piVar1 = local_10 + 2;
    FUN_00425bac(local_30 + 2);
    FUN_00401f54(param_3,param_4,local_30 + 2,piVar1);
    AnsiString_Free(local_30 + 2,2);
    local_30[1] = 0;
    FUN_00406204(local_30 + 1,(int)param_1,param_2);
    AnsiString_Assign(local_10 + 2,local_30 + 1);
    AnsiString_Free(local_30 + 1,2);
    piVar1 = local_10 + 2;
    FUN_00425bac(local_30);
    FUN_00401f54(param_3,param_4,local_30,piVar1);
    AnsiString_Free(local_30,2);
    local_34 = 0;
    FUN_00405e84(&local_34,(int)param_1,param_2);
    AnsiString_Assign(local_10 + 2,&local_34);
    AnsiString_Free(&local_34,2);
    piVar1 = local_10 + 2;
    FUN_00425bac(local_40 + 2);
    FUN_00401f54(param_3,param_4,local_40 + 2,piVar1);
    AnsiString_Free(local_40 + 2,2);
    local_40[1] = 0;
    FUN_0040609c(local_40 + 1,(int)param_1,param_2);
    AnsiString_Assign(local_10 + 2,local_40 + 1);
    AnsiString_Free(local_40 + 1,2);
    piVar1 = local_10 + 2;
    FUN_00425bac(local_40);
    FUN_00401f54(param_3,param_4,local_40,piVar1);
    AnsiString_Free(local_40,2);
    local_44 = 0;
    FUN_004057bc(&local_44,param_1,param_2);
    AnsiString_Assign(local_10 + 2,&local_44);
    AnsiString_Free(&local_44,2);
    piVar1 = local_10 + 2;
    FUN_00425bac(local_50 + 2);
    FUN_00401f54(param_3,param_4,local_50 + 2,piVar1);
    AnsiString_Free(local_50 + 2,2);
    local_50[1] = 0;
    FUN_00405a50(local_50 + 1,param_1,param_2);
    AnsiString_Assign(local_10 + 2,local_50 + 1);
    AnsiString_Free(local_50 + 1,2);
    piVar1 = local_10 + 2;
    FUN_00425bac(local_50);
    FUN_00401f54(param_3,param_4,local_50,piVar1);
    AnsiString_Free(local_50,2);
    local_54 = 0;
    FUN_00405bdc(&local_54,(int)param_1,param_2);
    AnsiString_Assign(local_10 + 2,&local_54);
    AnsiString_Free(&local_54,2);
    piVar1 = local_10 + 2;
    FUN_00425bac(local_60 + 2);
    FUN_00401f54(param_3,param_4,local_60 + 2,piVar1);
    AnsiString_Free(local_60 + 2,2);
  }
  else if (*param_1 - 10U < 10) {
    local_60[1] = 0;
    FUN_00406618(local_60 + 1,param_1,param_2);
    AnsiString_Assign(local_10 + 2,local_60 + 1);
    AnsiString_Free(local_60 + 1,2);
    piVar1 = local_10 + 2;
    FUN_00425bac(local_60);
    FUN_00401f54(param_3,param_4,local_60,piVar1);
    AnsiString_Free(local_60,2);
    local_64 = 0;
    FUN_00406498(&local_64,param_1,param_2);
    AnsiString_Assign(local_10 + 2,&local_64);
    AnsiString_Free(&local_64,2);
    piVar1 = local_10 + 2;
    FUN_00425bac(local_70 + 2);
    FUN_00401f54(param_3,param_4,local_70 + 2,piVar1);
    AnsiString_Free(local_70 + 2,2);
    local_70[1] = 0;
    FUN_004069c4(local_70 + 1,param_1,param_2);
    AnsiString_Assign(local_10 + 2,local_70 + 1);
    AnsiString_Free(local_70 + 1,2);
    piVar1 = local_10 + 2;
    FUN_00425bac(local_70);
    FUN_00401f54(param_3,param_4,local_70,piVar1);
    AnsiString_Free(local_70,2);
    local_74 = 0;
    FUN_00406acc(&local_74,param_1,param_2);
    AnsiString_Assign(local_10 + 2,&local_74);
    AnsiString_Free(&local_74,2);
    piVar1 = local_10 + 2;
    FUN_00425bac(local_80 + 2);
    FUN_00401f54(param_3,param_4,local_80 + 2,piVar1);
    AnsiString_Free(local_80 + 2,2);
    local_80[1] = 0;
    FUN_00406bd4(local_80 + 1,param_1,param_2);
    AnsiString_Assign(local_10 + 2,local_80 + 1);
    AnsiString_Free(local_80 + 1,2);
    piVar1 = local_10 + 2;
    FUN_00425bac(local_80);
    FUN_00401f54(param_3,param_4,local_80,piVar1);
    AnsiString_Free(local_80,2);
    local_84 = 0;
    FUN_00406e90(&local_84,param_1,param_2);
    AnsiString_Assign(local_10 + 2,&local_84);
    AnsiString_Free(&local_84,2);
    piVar1 = local_10 + 2;
    FUN_00425bac(local_90 + 2);
    FUN_00401f54(param_3,param_4,local_90 + 2,piVar1);
    AnsiString_Free(local_90 + 2,2);
    local_90[1] = 0;
    FUN_00406c5c(local_90 + 1,param_1,param_2);
    AnsiString_Assign(local_10 + 2,local_90 + 1);
    AnsiString_Free(local_90 + 1,2);
    piVar1 = local_10 + 2;
    FUN_00425bac(local_90);
    FUN_00401f54(param_3,param_4,local_90,piVar1);
    AnsiString_Free(local_90,2);
  }
  AnsiString_Free(local_10 + 2,2);
  *in_FS_OFFSET = local_b4;
  return;
}



void __cdecl DecodeTypeVar_Composite(int *param_1,int param_2,int param_3,undefined4 *param_4)

{
  undefined4 *in_FS_OFFSET;
  int *piVar1;
  undefined4 local_84;
  int local_60 [3];
  int local_54;
  int local_50 [3];
  int local_44;
  int local_40 [3];
  int local_34;
  int local_30 [3];
  int local_24;
  int local_20 [3];
  int local_14;
  int local_10 [3];
  
  BCB_InitExceptionHandler(0x429734);
  DecodeDate(param_2,param_3,param_4,7);
  DecodeTime(param_2,param_3,param_4,10);
  local_10[2] = 0;
  if (*param_1 - 4U < 3) {
    local_10[1] = 0;
    FUN_00405d7c(local_10 + 1,param_1,param_2);
    AnsiString_Assign(local_10 + 2,local_10 + 1);
    AnsiString_Free(local_10 + 1,2);
    piVar1 = local_10 + 2;
    FUN_00425bac(local_10);
    FUN_00401f54(param_3,param_4,local_10,piVar1);
    AnsiString_Free(local_10,2);
    local_14 = 0;
    FUN_00406398(&local_14,param_1,param_2);
    AnsiString_Assign(local_10 + 2,&local_14);
    AnsiString_Free(&local_14,2);
    piVar1 = local_10 + 2;
    FUN_00425bac(local_20 + 2);
    FUN_00401f54(param_3,param_4,local_20 + 2,piVar1);
    AnsiString_Free(local_20 + 2,2);
    local_20[1] = 0;
    FUN_00405f44(local_20 + 1,param_1,param_2);
    AnsiString_Assign(local_10 + 2,local_20 + 1);
    AnsiString_Free(local_20 + 1,2);
    piVar1 = local_10 + 2;
    FUN_00425bac(local_20);
    FUN_00401f54(param_3,param_4,local_20,piVar1);
    AnsiString_Free(local_20,2);
    local_24 = 0;
    FUN_00406158(&local_24,param_1,param_2);
    AnsiString_Assign(local_10 + 2,&local_24);
    AnsiString_Free(&local_24,2);
    piVar1 = local_10 + 2;
    FUN_00425bac(local_30 + 2);
    FUN_00401f54(param_3,param_4,local_30 + 2,piVar1);
    AnsiString_Free(local_30 + 2,2);
    local_30[1] = 0;
    FUN_00405ff0(local_30 + 1,param_1,param_2);
    AnsiString_Assign(local_10 + 2,local_30 + 1);
    AnsiString_Free(local_30 + 1,2);
    piVar1 = local_10 + 2;
    FUN_00425bac(local_30);
    FUN_00401f54(param_3,param_4,local_30,piVar1);
    AnsiString_Free(local_30,2);
    local_34 = 0;
    FUN_00405898(&local_34,param_1,param_2);
    AnsiString_Assign(local_10 + 2,&local_34);
    AnsiString_Free(&local_34,2);
    piVar1 = local_10 + 2;
    FUN_00425bac(local_40 + 2);
    FUN_00401f54(param_3,param_4,local_40 + 2,piVar1);
    AnsiString_Free(local_40 + 2,2);
    local_40[1] = 0;
    FUN_00405ad4(local_40 + 1,param_1,param_2);
    AnsiString_Assign(local_10 + 2,local_40 + 1);
    AnsiString_Free(local_40 + 1,2);
    piVar1 = local_10 + 2;
    FUN_00425bac(local_40);
    FUN_00401f54(param_3,param_4,local_40,piVar1);
    AnsiString_Free(local_40,2);
  }
  else if (*param_1 - 10U < 10) {
    local_44 = 0;
    FUN_00406688(&local_44,param_1,param_2);
    AnsiString_Assign(local_10 + 2,&local_44);
    AnsiString_Free(&local_44,2);
    piVar1 = local_10 + 2;
    FUN_00425bac(local_50 + 2);
    FUN_00401f54(param_3,param_4,local_50 + 2,piVar1);
    AnsiString_Free(local_50 + 2,2);
    local_50[1] = 0;
    FUN_00406518(local_50 + 1,param_1,param_2);
    AnsiString_Assign(local_10 + 2,local_50 + 1);
    AnsiString_Free(local_50 + 1,2);
    piVar1 = local_10 + 2;
    FUN_00425bac(local_50);
    FUN_00401f54(param_3,param_4,local_50,piVar1);
    AnsiString_Free(local_50,2);
    local_54 = 0;
    FUN_00406f38(&local_54,param_1,param_2);
    AnsiString_Assign(local_10 + 2,&local_54);
    AnsiString_Free(&local_54,2);
    piVar1 = local_10 + 2;
    FUN_00425bac(local_60 + 2);
    FUN_00401f54(param_3,param_4,local_60 + 2,piVar1);
    AnsiString_Free(local_60 + 2,2);
    local_60[1] = 0;
    FUN_00406ce0(local_60 + 1,param_1,param_2);
    AnsiString_Assign(local_10 + 2,local_60 + 1);
    AnsiString_Free(local_60 + 1,2);
    piVar1 = local_10 + 2;
    FUN_00425bac(local_60);
    FUN_00401f54(param_3,param_4,local_60,piVar1);
    AnsiString_Free(local_60,2);
  }
  AnsiString_Free(local_10 + 2,2);
  *in_FS_OFFSET = local_84;
  return;
}



void __cdecl DecodeTypeVar_Ascan(int *param_1,int param_2,int param_3,undefined4 *param_4)

{
  undefined4 *in_FS_OFFSET;
  int *piVar1;
  undefined4 local_ec;
  int local_c8 [3];
  int local_bc;
  int local_b8 [3];
  int local_ac;
  int local_a8 [3];
  int local_9c;
  int local_98 [3];
  int local_8c;
  int local_88 [3];
  int local_7c;
  int local_78 [3];
  int local_6c;
  int local_68 [3];
  int local_5c;
  int local_58 [3];
  int local_4c;
  int local_48 [3];
  int local_3c;
  int local_38 [3];
  int local_2c;
  int local_28 [3];
  int local_1c;
  int local_18 [3];
  int local_c [2];
  
  BCB_InitExceptionHandler(0x429b7c);
  DecodeDate(param_2,param_3,param_4,7);
  DecodeTime(param_2,param_3,param_4,10);
  local_c[1] = 0;
  if (*param_1 - 4U < 3) {
    local_c[0] = 0;
    FUN_00405e00(local_c,param_1,param_2);
    AnsiString_Assign(local_c + 1,local_c);
    AnsiString_Free(local_c,2);
    piVar1 = local_c + 1;
    FUN_00425bac(local_18 + 2);
    FUN_00401f54(param_3,param_4,local_18 + 2,piVar1);
    AnsiString_Free(local_18 + 2,2);
    local_18[1] = 0;
    FUN_00406418(local_18 + 1,param_1,param_2);
    AnsiString_Assign(local_c + 1,local_18 + 1);
    AnsiString_Free(local_18 + 1,2);
    piVar1 = local_c + 1;
    FUN_00425bac(local_18);
    FUN_00401f54(param_3,param_4,local_18,piVar1);
    AnsiString_Free(local_18,2);
    local_1c = 0;
    FUN_00405528(&local_1c,param_1,param_2);
    AnsiString_Assign(local_c + 1,&local_1c);
    AnsiString_Free(&local_1c,2);
    piVar1 = local_c + 1;
    FUN_00425bac(local_28 + 2);
    FUN_00401f54(param_3,param_4,local_28 + 2,piVar1);
    AnsiString_Free(local_28 + 2,2);
    local_28[1] = 0;
    FUN_00405630(local_28 + 1,param_1,param_2);
    AnsiString_Assign(local_c + 1,local_28 + 1);
    AnsiString_Free(local_28 + 1,2);
    piVar1 = local_c + 1;
    FUN_00425bac(local_28);
    FUN_00401f54(param_3,param_4,local_28,piVar1);
    AnsiString_Free(local_28,2);
    local_2c = 0;
    FUN_00405738(&local_2c,param_1,param_2);
    AnsiString_Assign(local_c + 1,&local_2c);
    AnsiString_Free(&local_2c,2);
    piVar1 = local_c + 1;
    FUN_00425bac(local_38 + 2);
    FUN_00401f54(param_3,param_4,local_38 + 2,piVar1);
    AnsiString_Free(local_38 + 2,2);
    local_38[1] = 0;
    FUN_00406fe0(local_38 + 1,param_1,param_2);
    AnsiString_Assign(local_c + 1,local_38 + 1);
    AnsiString_Free(local_38 + 1,2);
    piVar1 = local_c + 1;
    FUN_00425bac(local_38);
    FUN_00401f54(param_3,param_4,local_38,piVar1);
    AnsiString_Free(local_38,2);
    local_3c = 0;
    FUN_00407060(&local_3c,param_1,param_2);
    AnsiString_Assign(local_c + 1,&local_3c);
    AnsiString_Free(&local_3c,2);
    piVar1 = local_c + 1;
    FUN_00425bac(local_48 + 2);
    FUN_00401f54(param_3,param_4,local_48 + 2,piVar1);
    AnsiString_Free(local_48 + 2,2);
    local_48[1] = 0;
    FUN_00407160(local_48 + 1,param_1,param_2);
    AnsiString_Assign(local_c + 1,local_48 + 1);
    AnsiString_Free(local_48 + 1,2);
    piVar1 = local_c + 1;
    FUN_00425bac(local_48);
    FUN_00401f54(param_3,param_4,local_48,piVar1);
    AnsiString_Free(local_48,2);
    local_4c = 0;
    FUN_004070e0(&local_4c,param_1,param_2);
    AnsiString_Assign(local_c + 1,&local_4c);
    AnsiString_Free(&local_4c,2);
    piVar1 = local_c + 1;
    FUN_00425bac(local_58 + 2);
    FUN_00401f54(param_3,param_4,local_58 + 2,piVar1);
    AnsiString_Free(local_58 + 2,2);
    local_58[1] = 0;
    FUN_00405974(local_58 + 1,param_1,param_2);
    AnsiString_Assign(local_c + 1,local_58 + 1);
    AnsiString_Free(local_58 + 1,2);
    piVar1 = local_c + 1;
    FUN_00425bac(local_58);
    FUN_00401f54(param_3,param_4,local_58,piVar1);
    AnsiString_Free(local_58,2);
    local_5c = 0;
    FUN_00405b58(&local_5c,param_1,param_2);
    AnsiString_Assign(local_c + 1,&local_5c);
    AnsiString_Free(&local_5c,2);
    piVar1 = local_c + 1;
    FUN_00425bac(local_68 + 2);
    FUN_00401f54(param_3,param_4,local_68 + 2,piVar1);
    AnsiString_Free(local_68 + 2,2);
    local_68[1] = 0;
    FUN_00405c78(local_68 + 1,param_1,param_2);
    AnsiString_Assign(local_c + 1,local_68 + 1);
    AnsiString_Free(local_68 + 1,2);
    piVar1 = local_c + 1;
    FUN_00425bac(local_68);
    FUN_00401f54(param_3,param_4,local_68,piVar1);
    AnsiString_Free(local_68,2);
  }
  else if (*param_1 - 10U < 10) {
    local_6c = 0;
    FUN_004066f8(&local_6c,param_1,param_2);
    AnsiString_Assign(local_c + 1,&local_6c);
    AnsiString_Free(&local_6c,2);
    piVar1 = local_c + 1;
    FUN_00425bac(local_78 + 2);
    FUN_00401f54(param_3,param_4,local_78 + 2,piVar1);
    AnsiString_Free(local_78 + 2,2);
    local_78[1] = 0;
    FUN_00406598(local_78 + 1,param_1,param_2);
    AnsiString_Assign(local_c + 1,local_78 + 1);
    AnsiString_Free(local_78 + 1,2);
    piVar1 = local_c + 1;
    FUN_00425bac(local_78);
    FUN_00401f54(param_3,param_4,local_78,piVar1);
    AnsiString_Free(local_78,2);
    local_7c = 0;
    FUN_00406940(&local_7c,param_1,param_2);
    AnsiString_Assign(local_c + 1,&local_7c);
    AnsiString_Free(&local_7c,2);
    piVar1 = local_c + 1;
    FUN_00425bac(local_88 + 2);
    FUN_00401f54(param_3,param_4,local_88 + 2,piVar1);
    AnsiString_Free(local_88 + 2,2);
    local_88[1] = 0;
    FUN_00406a48(local_88 + 1,param_1,param_2);
    AnsiString_Assign(local_c + 1,local_88 + 1);
    AnsiString_Free(local_88 + 1,2);
    piVar1 = local_c + 1;
    FUN_00425bac(local_88);
    FUN_00401f54(param_3,param_4,local_88,piVar1);
    AnsiString_Free(local_88,2);
    local_8c = 0;
    FUN_00406b50(&local_8c,param_1,param_2);
    AnsiString_Assign(local_c + 1,&local_8c);
    AnsiString_Free(&local_8c,2);
    piVar1 = local_c + 1;
    FUN_00425bac(local_98 + 2);
    FUN_00401f54(param_3,param_4,local_98 + 2,piVar1);
    AnsiString_Free(local_98 + 2,2);
    local_98[1] = 0;
    FUN_00406768(local_98 + 1,param_1,param_2);
    AnsiString_Assign(local_c + 1,local_98 + 1);
    AnsiString_Free(local_98 + 1,2);
    piVar1 = local_c + 1;
    FUN_00425bac(local_98);
    FUN_00401f54(param_3,param_4,local_98,piVar1);
    AnsiString_Free(local_98,2);
    local_9c = 0;
    FUN_00406fe0(&local_9c,param_1,param_2);
    AnsiString_Assign(local_c + 1,&local_9c);
    AnsiString_Free(&local_9c,2);
    piVar1 = local_c + 1;
    FUN_00425bac(local_a8 + 2);
    FUN_00401f54(param_3,param_4,local_a8 + 2,piVar1);
    AnsiString_Free(local_a8 + 2,2);
    local_a8[1] = 0;
    FUN_00407060(local_a8 + 1,param_1,param_2);
    AnsiString_Assign(local_c + 1,local_a8 + 1);
    AnsiString_Free(local_a8 + 1,2);
    piVar1 = local_c + 1;
    FUN_00425bac(local_a8);
    FUN_00401f54(param_3,param_4,local_a8,piVar1);
    AnsiString_Free(local_a8,2);
    local_ac = 0;
    FUN_00407160(&local_ac,param_1,param_2);
    AnsiString_Assign(local_c + 1,&local_ac);
    AnsiString_Free(&local_ac,2);
    piVar1 = local_c + 1;
    FUN_00425bac(local_b8 + 2);
    FUN_00401f54(param_3,param_4,local_b8 + 2,piVar1);
    AnsiString_Free(local_b8 + 2,2);
    local_b8[1] = 0;
    FUN_004070e0(local_b8 + 1,param_1,param_2);
    AnsiString_Assign(local_c + 1,local_b8 + 1);
    AnsiString_Free(local_b8 + 1,2);
    piVar1 = local_c + 1;
    FUN_00425bac(local_b8);
    FUN_00401f54(param_3,param_4,local_b8,piVar1);
    AnsiString_Free(local_b8,2);
    local_bc = 0;
    FUN_00406de8(&local_bc,param_1,param_2);
    AnsiString_Assign(local_c + 1,&local_bc);
    AnsiString_Free(&local_bc,2);
    piVar1 = local_c + 1;
    FUN_00425bac(local_c8 + 2);
    FUN_00401f54(param_3,param_4,local_c8 + 2,piVar1);
    AnsiString_Free(local_c8 + 2,2);
    local_c8[1] = 0;
    FUN_00406d64(local_c8 + 1,param_1,param_2);
    AnsiString_Assign(local_c + 1,local_c8 + 1);
    AnsiString_Free(local_c8 + 1,2);
    piVar1 = local_c + 1;
    FUN_00425bac(local_c8);
    FUN_00401f54(param_3,param_4,local_c8,piVar1);
    AnsiString_Free(local_c8,2);
  }
  AnsiString_Free(local_c + 1,2);
  *in_FS_OFFSET = local_ec;
  return;
}



int * __cdecl FUN_00405528(int *param_1,undefined4 param_2,int param_3)

{
  int iVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x429e1c);
  iVar1 = FUN_0040381c((byte *)(param_3 + 0x38));
  local_8 = 0;
  FUN_00414208(iVar1,&local_8);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_004055ac(int *param_1,undefined4 param_2,int param_3)

{
  int iVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x429e94);
  iVar1 = FUN_0040381c((byte *)(param_3 + 0x35));
  local_8 = 0;
  FUN_00414208(iVar1,&local_8);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_00405630(int *param_1,undefined4 param_2,int param_3)

{
  int iVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x429f0c);
  iVar1 = FUN_0040381c((byte *)(param_3 + 0x3a));
  local_8 = 0;
  FUN_00414208(iVar1,&local_8);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_004056b4(int *param_1,undefined4 param_2,int param_3)

{
  int iVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x429f84);
  iVar1 = FUN_0040381c((byte *)(param_3 + 0x37));
  local_8 = 0;
  FUN_00414208(iVar1,&local_8);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_00405738(int *param_1,undefined4 param_2,int param_3)

{
  int iVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x429ffc);
  iVar1 = FUN_0040381c((byte *)(param_3 + 0x3c));
  local_8 = 0;
  FUN_00414208(iVar1,&local_8);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_004057bc(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_34;
  int local_10;
  int local_c [2];
  
  BCB_InitExceptionHandler(0x42a094);
  local_c[1] = 0;
  if (*(char *)(param_3 + 0xc) == '\0') {
    FUN_00425bac(&local_10);
    AnsiString_Assign(local_c + 1,&local_10);
    AnsiString_Free(&local_10,2);
  }
  else {
    FUN_00425bac(local_c);
    AnsiString_Assign(local_c + 1,local_c);
    AnsiString_Free(local_c,2);
  }
  AnsiString_Assign(param_1,local_c + 1);
  AnsiString_Free(local_c + 1,2);
  *in_FS_OFFSET = local_34;
  return param_1;
}



int * __cdecl FUN_00405898(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_34;
  int local_10;
  int local_c [2];
  
  BCB_InitExceptionHandler(0x42a144);
  local_c[1] = 0;
  if (*(char *)(param_3 + 0xc) == '\0') {
    FUN_00425bac(&local_10);
    AnsiString_Assign(local_c + 1,&local_10);
    AnsiString_Free(&local_10,2);
  }
  else {
    FUN_00425bac(local_c);
    AnsiString_Assign(local_c + 1,local_c);
    AnsiString_Free(local_c,2);
  }
  AnsiString_Assign(param_1,local_c + 1);
  AnsiString_Free(local_c + 1,2);
  *in_FS_OFFSET = local_34;
  return param_1;
}



int * __cdecl FUN_00405974(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_34;
  int local_10;
  int local_c [2];
  
  BCB_InitExceptionHandler(0x42a1f4);
  local_c[1] = 0;
  if (*(char *)(param_3 + 0xc) == '\0') {
    FUN_00425bac(&local_10);
    AnsiString_Assign(local_c + 1,&local_10);
    AnsiString_Free(&local_10,2);
  }
  else {
    FUN_00425bac(local_c);
    AnsiString_Assign(local_c + 1,local_c);
    AnsiString_Free(local_c,2);
  }
  AnsiString_Assign(param_1,local_c + 1);
  AnsiString_Free(local_c + 1,2);
  *in_FS_OFFSET = local_34;
  return param_1;
}



int * __cdecl FUN_00405a50(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42a284);
  local_8 = 0;
  FUN_00402708(&local_8,param_3 + 0x21,7);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_00405ad4(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42a2fc);
  local_8 = 0;
  FUN_00402708(&local_8,param_3 + 0x21,7);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_00405b58(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42a374);
  local_8 = 0;
  FUN_00402708(&local_8,param_3 + 0x21,7);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_00405bdc(int *param_1,int param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  int local_30;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42a3ec);
  local_30 = 0x3b;
  if (*(int *)(param_2 + 0xc) - 0x2a8U < 4) {
    local_30 = 0x3e;
  }
  FUN_0040381c((byte *)(param_3 + local_30));
  FUN_00425c6c(&local_8);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_00405c78(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42a464);
  FUN_0040381c((byte *)(param_3 + 0x45));
  FUN_00425c6c(&local_8);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_00405cf8(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42a4dc);
  local_8 = 0;
  FUN_00402708(&local_8,param_3 + 0x11,0xb);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_00405d7c(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42a554);
  local_8 = 0;
  FUN_00402708(&local_8,param_3 + 0x11,0xb);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_00405e00(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42a5cc);
  local_8 = 0;
  FUN_00402708(&local_8,param_3 + 0x11,0xb);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_00405e84(int *param_1,int param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_30;
  int local_c [2];
  
  BCB_InitExceptionHandler(0x42a654);
  local_c[1] = 0;
  if (*(int *)(param_2 + 0xc) - 0x2a8U < 4) {
    local_c[0] = 0;
    FUN_00402708(local_c,param_3 + 0x41,7);
    AnsiString_Assign(local_c + 1,local_c);
    AnsiString_Free(local_c,2);
  }
  AnsiString_Assign(param_1,local_c + 1);
  AnsiString_Free(local_c + 1,2);
  *in_FS_OFFSET = local_30;
  return param_1;
}



int * __cdecl FUN_00405f44(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_30;
  int local_c [2];
  
  BCB_InitExceptionHandler(0x42a6e8);
  local_c[1] = 0;
  local_c[0] = 0;
  FUN_00402708(local_c,param_3 + 0x35,7);
  AnsiString_Assign(local_c + 1,local_c);
  AnsiString_Free(local_c,2);
  AnsiString_Assign(param_1,local_c + 1);
  AnsiString_Free(local_c + 1,2);
  *in_FS_OFFSET = local_30;
  return param_1;
}



int * __cdecl FUN_00405ff0(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_30;
  int local_c [2];
  
  BCB_InitExceptionHandler(0x42a77c);
  local_c[1] = 0;
  FUN_0040381c((byte *)(param_3 + 0x3c));
  FUN_00425c6c(local_c);
  AnsiString_Assign(local_c + 1,local_c);
  AnsiString_Free(local_c,2);
  AnsiString_Assign(param_1,local_c + 1);
  AnsiString_Free(local_c + 1,2);
  *in_FS_OFFSET = local_30;
  return param_1;
}



int * __cdecl FUN_0040609c(int *param_1,int param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_30;
  int local_c [2];
  
  BCB_InitExceptionHandler(0x42a810);
  local_c[1] = 0;
  if (*(int *)(param_2 + 0xc) - 0x2a8U < 4) {
    FUN_0040381c((byte *)(param_3 + 0x3c));
    FUN_00425c6c(local_c);
    AnsiString_Assign(local_c + 1,local_c);
    AnsiString_Free(local_c,2);
  }
  AnsiString_Assign(param_1,local_c + 1);
  AnsiString_Free(local_c + 1,2);
  *in_FS_OFFSET = local_30;
  return param_1;
}



int * __cdecl FUN_00406158(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_30;
  int local_c [2];
  
  BCB_InitExceptionHandler(0x42a8a4);
  local_c[1] = 0;
  FUN_0040381c((byte *)(param_3 + 0x3e));
  FUN_00425c6c(local_c);
  AnsiString_Assign(local_c + 1,local_c);
  AnsiString_Free(local_c,2);
  AnsiString_Assign(param_1,local_c + 1);
  AnsiString_Free(local_c + 1,2);
  *in_FS_OFFSET = local_30;
  return param_1;
}



int * __cdecl FUN_00406204(int *param_1,int param_2,int param_3)

{
  int iVar1;
  uint uVar2;
  undefined4 *in_FS_OFFSET;
  undefined4 local_30;
  int local_c [2];
  
  BCB_InitExceptionHandler(0x42a938);
  local_c[1] = 0;
  iVar1 = *(int *)(param_2 + 0xc);
  if (iVar1 < 0x2c8) {
    if ((((1 < iVar1 - 0x16aU) && (1 < iVar1 - 0x17cU)) && (1 < iVar1 - 0x186U)) &&
       (3 < iVar1 - 0x2a8U)) goto LAB_004062d3;
  }
  else if (iVar1 < 0x2e5) {
    if ((iVar1 != 0x2e4) && (1 < iVar1 - 0x2c8U)) {
      uVar2 = iVar1 - 0x2da;
joined_r0x0040628c:
      if (1 < uVar2) goto LAB_004062d3;
    }
  }
  else if (iVar1 != 0x2e6) {
    uVar2 = iVar1 - 0x30c;
    goto joined_r0x0040628c;
  }
  FUN_0040381c((byte *)(param_3 + 0x39));
  FUN_00425c6c(local_c);
  AnsiString_Assign(local_c + 1,local_c);
  AnsiString_Free(local_c,2);
LAB_004062d3:
  AnsiString_Assign(param_1,local_c + 1);
  AnsiString_Free(local_c + 1,2);
  *in_FS_OFFSET = local_30;
  return param_1;
}



int * __cdecl FUN_00406318(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42a9bc);
  FUN_0040381c((byte *)(param_3 + 0x5e));
  FUN_00425c6c(&local_8);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_00406398(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42aa34);
  FUN_0040381c((byte *)(param_3 + 0x5e));
  FUN_00425c6c(&local_8);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_00406418(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42aaac);
  FUN_0040381c((byte *)(param_3 + 0x5e));
  FUN_00425c6c(&local_8);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_00406498(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42ab24);
  FUN_0040381c((byte *)(param_3 + 0x29));
  FUN_00425c6c(&local_8);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_00406518(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42ab9c);
  FUN_0040381c((byte *)(param_3 + 0x29));
  FUN_00425c6c(&local_8);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_00406598(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42ac14);
  FUN_0040381c((byte *)(param_3 + 0x29));
  FUN_00425c6c(&local_8);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_00406618(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42ac78);
  local_8 = 0;
  FUN_00402708(&local_8,param_3 + 0x11,0xb);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_00406688(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42acc4);
  local_8 = 0;
  FUN_00402708(&local_8,param_3 + 0x11,0xb);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_004066f8(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42ad10);
  local_8 = 0;
  FUN_00402708(&local_8,param_3 + 0x11,0xb);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_00406768(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_44;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  int local_c [2];
  
  BCB_InitExceptionHandler(0x42add0);
  local_c[1] = 0;
  switch(*(undefined1 *)(param_3 + 0x35)) {
  case 0:
    FUN_00425bac(local_c);
    AnsiString_Assign(local_c + 1,local_c);
    AnsiString_Free(local_c,2);
    break;
  case 1:
    FUN_00425bac(&local_10);
    AnsiString_Assign(local_c + 1,&local_10);
    AnsiString_Free(&local_10,2);
    break;
  case 2:
    FUN_00425bac(&local_14);
    AnsiString_Assign(local_c + 1,&local_14);
    AnsiString_Free(&local_14,2);
    break;
  case 3:
    FUN_00425bac(&local_18);
    AnsiString_Assign(local_c + 1,&local_18);
    AnsiString_Free(&local_18,2);
    break;
  case 4:
    FUN_00425bac(&local_1c);
    AnsiString_Assign(local_c + 1,&local_1c);
    AnsiString_Free(&local_1c,2);
    break;
  case 5:
    FUN_00425bac(&local_20);
    AnsiString_Assign(local_c + 1,&local_20);
    AnsiString_Free(&local_20,2);
  }
  AnsiString_Assign(param_1,local_c + 1);
  AnsiString_Free(local_c + 1,2);
  *in_FS_OFFSET = local_44;
  return param_1;
}



int * __cdecl FUN_00406940(int *param_1,undefined4 param_2,int param_3)

{
  int iVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42ae90);
  iVar1 = FUN_0040381c((byte *)(param_3 + 0x38));
  local_8 = 0;
  FUN_00414208(iVar1,&local_8);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_004069c4(int *param_1,undefined4 param_2,int param_3)

{
  int iVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42af08);
  iVar1 = FUN_0040381c((byte *)(param_3 + 0x35));
  local_8 = 0;
  FUN_00414208(iVar1,&local_8);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_00406a48(int *param_1,undefined4 param_2,int param_3)

{
  int iVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42af80);
  iVar1 = FUN_0040381c((byte *)(param_3 + 0x3a));
  local_8 = 0;
  FUN_00414208(iVar1,&local_8);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_00406acc(int *param_1,undefined4 param_2,int param_3)

{
  int iVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42aff8);
  iVar1 = FUN_0040381c((byte *)(param_3 + 0x37));
  local_8 = 0;
  FUN_00414208(iVar1,&local_8);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_00406b50(int *param_1,undefined4 param_2,int param_3)

{
  int iVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42b070);
  iVar1 = FUN_0040381c((byte *)(param_3 + 0x3c));
  local_8 = 0;
  FUN_00414208(iVar1,&local_8);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_00406bd4(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42b0e8);
  FUN_0040381c((byte *)(param_3 + 0x39));
  FUN_00425c6c(&local_8);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_00406c5c(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42b160);
  local_8 = 0;
  FUN_00402708(&local_8,param_3 + 0x21,7);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_00406ce0(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42b1d8);
  local_8 = 0;
  FUN_00402708(&local_8,param_3 + 0x21,7);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_00406d64(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42b250);
  local_8 = 0;
  FUN_00402708(&local_8,param_3 + 0x21,7);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_00406de8(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_30;
  int local_c [2];
  
  BCB_InitExceptionHandler(0x42b2d8);
  local_c[1] = 0;
  local_c[0] = 0;
  FUN_00414208((uint)*(byte *)(param_3 + 0xc),local_c);
  AnsiString_Assign(local_c + 1,local_c);
  AnsiString_Free(local_c,2);
  AnsiString_Assign(param_1,local_c + 1);
  AnsiString_Free(local_c + 1,2);
  *in_FS_OFFSET = local_30;
  return param_1;
}



int * __cdecl FUN_00406e90(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_30;
  int local_c [2];
  
  BCB_InitExceptionHandler(0x42b36c);
  local_c[1] = 0;
  local_c[0] = 0;
  FUN_00414208((uint)*(byte *)(param_3 + 0xc),local_c);
  AnsiString_Assign(local_c + 1,local_c);
  AnsiString_Free(local_c,2);
  AnsiString_Assign(param_1,local_c + 1);
  AnsiString_Free(local_c + 1,2);
  *in_FS_OFFSET = local_30;
  return param_1;
}



int * __cdecl FUN_00406f38(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_30;
  int local_c [2];
  
  BCB_InitExceptionHandler(0x42b400);
  local_c[1] = 0;
  local_c[0] = 0;
  FUN_00414208((uint)*(byte *)(param_3 + 0xc),local_c);
  AnsiString_Assign(local_c + 1,local_c);
  AnsiString_Free(local_c,2);
  AnsiString_Assign(param_1,local_c + 1);
  AnsiString_Free(local_c + 1,2);
  *in_FS_OFFSET = local_30;
  return param_1;
}



int * __cdecl FUN_00406fe0(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42b484);
  local_8 = 0;
  FUN_00414208((uint)*(byte *)(param_3 + 0x36),&local_8);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_00407060(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42b4fc);
  local_8 = 0;
  FUN_00414208((uint)*(byte *)(param_3 + 0x40),&local_8);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_004070e0(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42b574);
  local_8 = 0;
  FUN_00414208((uint)*(byte *)(param_3 + 0x3f),&local_8);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * __cdecl FUN_00407160(int *param_1,undefined4 param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42b5ec);
  local_8 = 0;
  FUN_00414208((uint)*(byte *)(param_3 + 0x3e),&local_8);
  AnsiString_Assign(param_1,&local_8);
  AnsiString_Free(&local_8,2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



int * GetFormatByCategory(int *param_1)

{
  int *piVar1;
  undefined4 *in_FS_OFFSET;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 local_d0;
  int local_ac;
  int local_a8;
  int local_a4;
  int local_a0;
  int local_9c;
  int local_98;
  int local_94;
  int local_90;
  int local_8c;
  int local_88;
  int local_84;
  int local_80;
  int local_7c;
  int local_78;
  int local_74;
  int local_70;
  int local_6c;
  int local_68;
  int local_64;
  int local_60;
  int local_5c;
  int local_58;
  int local_54;
  int local_50;
  int local_4c;
  int local_48;
  int local_44;
  int local_40;
  int local_3c;
  int local_38;
  int local_34;
  int local_30;
  int local_2c;
  int local_28;
  int local_24;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42c1b4);
  piVar1 = (int *)FUN_004012c8((int *)&PTR_LAB_0040be20,'\x01');
  iVar2 = *param_1;
  if (iVar2 == 1) {
    iVar4 = 1;
    iVar3 = 0;
    iVar2 = 1;
    FUN_00425bac(&local_c);
    FUN_00425bac(&local_8);
    FUN_00401db0(piVar1,&local_8,&local_c,iVar2,iVar3,iVar4);
    AnsiString_Free(&local_c,2);
    AnsiString_Free(&local_8,2);
    iVar4 = 1;
    iVar3 = 0;
    iVar2 = 4;
    FUN_00425bac(&local_14);
    FUN_00425bac(&local_10);
    FUN_00401db0(piVar1,&local_10,&local_14,iVar2,iVar3,iVar4);
    AnsiString_Free(&local_14,2);
    AnsiString_Free(&local_10,2);
    iVar4 = 2;
    iVar3 = 0x14;
    iVar2 = 0x14;
    FUN_00425bac(&local_1c);
    FUN_00425bac(&local_18);
    FUN_00401db0(piVar1,&local_18,&local_1c,iVar2,iVar3,iVar4);
    AnsiString_Free(&local_1c,2);
    AnsiString_Free(&local_18,2);
    iVar4 = 3;
    iVar3 = 0;
    iVar2 = 3;
    FUN_00425bac(&local_24);
    FUN_00425bac(&local_20);
    FUN_00401db0(piVar1,&local_20,&local_24,iVar2,iVar3,iVar4);
    AnsiString_Free(&local_24,2);
    AnsiString_Free(&local_20,2);
    iVar4 = 2;
    iVar3 = 5;
    iVar2 = 5;
    FUN_00425bac(&local_2c);
    FUN_00425bac(&local_28);
    FUN_00401db0(piVar1,&local_28,&local_2c,iVar2,iVar3,iVar4);
    AnsiString_Free(&local_2c,2);
    AnsiString_Free(&local_28,2);
    iVar4 = 1;
    iVar3 = 0;
    iVar2 = 2;
    FUN_00425bac(&local_34);
    FUN_00425bac(&local_30);
    FUN_00401db0(piVar1,&local_30,&local_34,iVar2,iVar3,iVar4);
    AnsiString_Free(&local_34,2);
    AnsiString_Free(&local_30,2);
    iVar4 = 2;
    iVar3 = 0x1e;
    iVar2 = 1;
    FUN_00425bac(&local_3c);
    FUN_00425bac(&local_38);
    FUN_00401db0(piVar1,&local_38,&local_3c,iVar2,iVar3,iVar4);
    AnsiString_Free(&local_3c,2);
    AnsiString_Free(&local_38,2);
    iVar4 = 2;
    iVar3 = 10;
    iVar2 = 10;
    FUN_00425bac(&local_44);
    FUN_00425bac(&local_40);
    FUN_00401db0(piVar1,&local_40,&local_44,iVar2,iVar3,iVar4);
    AnsiString_Free(&local_44,2);
    AnsiString_Free(&local_40,2);
    iVar4 = 1;
    iVar3 = 0;
    iVar2 = 2;
    FUN_00425bac(&local_4c);
    FUN_00425bac(&local_48);
    FUN_00401db0(piVar1,&local_48,&local_4c,iVar2,iVar3,iVar4);
    AnsiString_Free(&local_4c,2);
    AnsiString_Free(&local_48,2);
    iVar4 = 1;
    iVar3 = 0;
    iVar2 = 2;
    FUN_00425bac(&local_54);
    FUN_00425bac(&local_50);
    FUN_00401db0(piVar1,&local_50,&local_54,iVar2,iVar3,iVar4);
    AnsiString_Free(&local_54,2);
    AnsiString_Free(&local_50,2);
    iVar4 = 1;
    iVar3 = 0;
    iVar2 = 0;
    FUN_00425bac(&local_5c);
    FUN_00425bac(&local_58);
    FUN_00401db0(piVar1,&local_58,&local_5c,iVar2,iVar3,iVar4);
    AnsiString_Free(&local_5c,2);
    AnsiString_Free(&local_58,2);
  }
  else if (iVar2 - 4U < 3) {
    FUN_00407c20((int)param_1,piVar1);
  }
  else if (iVar2 - 10U < 10) {
    FUN_00409984((int)param_1,piVar1);
  }
  else if (iVar2 - 0x14U < 10) {
    iVar4 = 1;
    iVar3 = 0;
    iVar2 = 1;
    FUN_00425bac(&local_64);
    FUN_00425bac(&local_60);
    FUN_00401db0(piVar1,&local_60,&local_64,iVar2,iVar3,iVar4);
    AnsiString_Free(&local_64,2);
    AnsiString_Free(&local_60,2);
    iVar4 = 1;
    iVar3 = 0;
    iVar2 = 4;
    FUN_00425bac(&local_6c);
    FUN_00425bac(&local_68);
    FUN_00401db0(piVar1,&local_68,&local_6c,iVar2,iVar3,iVar4);
    AnsiString_Free(&local_6c,2);
    AnsiString_Free(&local_68,2);
    iVar4 = 3;
    iVar3 = 0;
    iVar2 = 3;
    FUN_00425bac(&local_74);
    FUN_00425bac(&local_70);
    FUN_00401db0(piVar1,&local_70,&local_74,iVar2,iVar3,iVar4);
    AnsiString_Free(&local_74,2);
    AnsiString_Free(&local_70,2);
    iVar4 = 2;
    iVar3 = 5;
    iVar2 = 5;
    FUN_00425bac(&local_7c);
    FUN_00425bac(&local_78);
    FUN_00401db0(piVar1,&local_78,&local_7c,iVar2,iVar3,iVar4);
    AnsiString_Free(&local_7c,2);
    AnsiString_Free(&local_78,2);
    iVar4 = 1;
    iVar3 = 0;
    iVar2 = 2;
    FUN_00425bac(&local_84);
    FUN_00425bac(&local_80);
    FUN_00401db0(piVar1,&local_80,&local_84,iVar2,iVar3,iVar4);
    AnsiString_Free(&local_84,2);
    AnsiString_Free(&local_80,2);
    iVar4 = 2;
    iVar3 = 0x1e;
    iVar2 = 1;
    FUN_00425bac(&local_8c);
    FUN_00425bac(&local_88);
    FUN_00401db0(piVar1,&local_88,&local_8c,iVar2,iVar3,iVar4);
    AnsiString_Free(&local_8c,2);
    AnsiString_Free(&local_88,2);
    iVar4 = 2;
    iVar3 = 10;
    iVar2 = 10;
    FUN_00425bac(&local_94);
    FUN_00425bac(&local_90);
    FUN_00401db0(piVar1,&local_90,&local_94,iVar2,iVar3,iVar4);
    AnsiString_Free(&local_94,2);
    AnsiString_Free(&local_90,2);
    iVar4 = 1;
    iVar3 = 0;
    iVar2 = 2;
    FUN_00425bac(&local_9c);
    FUN_00425bac(&local_98);
    FUN_00401db0(piVar1,&local_98,&local_9c,iVar2,iVar3,iVar4);
    AnsiString_Free(&local_9c,2);
    AnsiString_Free(&local_98,2);
    iVar4 = 2;
    iVar3 = 10;
    iVar2 = 10;
    FUN_00425bac(&local_a4);
    FUN_00425bac(&local_a0);
    FUN_00401db0(piVar1,&local_a0,&local_a4,iVar2,iVar3,iVar4);
    AnsiString_Free(&local_a4,2);
    AnsiString_Free(&local_a0,2);
    iVar4 = 1;
    iVar3 = 0;
    iVar2 = 0;
    FUN_00425bac(&local_ac);
    FUN_00425bac(&local_a8);
    FUN_00401db0(piVar1,&local_a8,&local_ac,iVar2,iVar3,iVar4);
    AnsiString_Free(&local_ac,2);
    AnsiString_Free(&local_a8,2);
  }
  *in_FS_OFFSET = local_d0;
  return piVar1;
}



void FUN_00407c20(int param_1,int *param_2)

{
  uint uVar1;
  undefined4 *in_FS_OFFSET;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 local_200;
  int local_1dc;
  int local_1d8;
  int local_1d4;
  int local_1d0;
  int local_1cc;
  int local_1c8;
  int local_1c4;
  int local_1c0;
  int local_1bc;
  int local_1b8;
  int local_1b4;
  int local_1b0;
  int local_1ac;
  int local_1a8;
  int local_1a4;
  int local_1a0;
  int local_19c;
  int local_198;
  int local_194;
  int local_190;
  int local_18c;
  int local_188;
  int local_184;
  int local_180;
  int local_17c;
  int local_178;
  int local_174;
  int local_170;
  int local_16c;
  int local_168;
  int local_164;
  int local_160;
  int local_15c;
  int local_158;
  int local_154;
  int local_150;
  int local_14c;
  int local_148;
  int local_144;
  int local_140;
  int local_13c;
  int local_138;
  int local_134;
  int local_130;
  int local_12c;
  int local_128;
  int local_124;
  int local_120;
  int local_11c;
  int local_118;
  int local_114;
  int local_110;
  int local_10c;
  int local_108;
  int local_104;
  int local_100;
  int local_fc;
  int local_f8;
  int local_f4;
  int local_f0;
  int local_ec;
  int local_e8;
  int local_e4;
  int local_e0;
  int local_dc;
  int local_d8;
  int local_d4;
  int local_d0;
  int local_cc;
  int local_c8;
  int local_c4;
  int local_c0;
  int local_bc;
  int local_b8;
  int local_b4;
  int local_b0;
  int local_ac;
  int local_a8;
  int local_a4;
  int local_a0;
  int local_9c;
  int local_98;
  int local_94;
  int local_90;
  int local_8c;
  int local_88;
  int local_84;
  int local_80;
  int local_7c;
  int local_78;
  int local_74;
  int local_70;
  int local_6c;
  int local_68;
  int local_64;
  int local_60;
  int local_5c;
  int local_58;
  int local_54;
  int local_50;
  int local_4c;
  int local_48;
  int local_44;
  int local_40;
  int local_3c;
  int local_38;
  int local_34;
  int local_30;
  int local_2c;
  int local_28;
  int local_24;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42c92c);
  iVar2 = *(int *)(param_1 + 0xc);
  if (iVar2 < 0x2a8) {
    if (iVar2 < 0x186) {
      if (iVar2 - 0x96U < 2) {
        iVar4 = 1;
        iVar3 = 0;
        iVar2 = 1;
        FUN_00425bac(&local_134);
        FUN_00425bac(&local_130);
        FUN_00401db0(param_2,&local_130,&local_134,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_134,2);
        AnsiString_Free(&local_130,2);
        iVar4 = 2;
        iVar3 = 0x14;
        iVar2 = 0x14;
        FUN_00425bac(&local_13c);
        FUN_00425bac(&local_138);
        FUN_00401db0(param_2,&local_138,&local_13c,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_13c,2);
        AnsiString_Free(&local_138,2);
        iVar4 = 1;
        iVar3 = 0;
        iVar2 = 4;
        FUN_00425bac(&local_144);
        FUN_00425bac(&local_140);
        FUN_00401db0(param_2,&local_140,&local_144,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_144,2);
        AnsiString_Free(&local_140,2);
        iVar4 = 3;
        iVar3 = 0;
        iVar2 = 3;
        FUN_00425bac(&local_14c);
        FUN_00425bac(&local_148);
        FUN_00401db0(param_2,&local_148,&local_14c,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_14c,2);
        AnsiString_Free(&local_148,2);
        iVar4 = 2;
        iVar3 = 5;
        iVar2 = 5;
        FUN_00425bac(&local_154);
        FUN_00425bac(&local_150);
        FUN_00401db0(param_2,&local_150,&local_154,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_154,2);
        AnsiString_Free(&local_150,2);
        iVar4 = 1;
        iVar3 = 0;
        iVar2 = 2;
        FUN_00425bac(&local_15c);
        FUN_00425bac(&local_158);
        FUN_00401db0(param_2,&local_158,&local_15c,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_15c,2);
        AnsiString_Free(&local_158,2);
        iVar4 = 2;
        iVar3 = 0x1e;
        iVar2 = 1;
        FUN_00425bac(&local_164);
        FUN_00425bac(&local_160);
        FUN_00401db0(param_2,&local_160,&local_164,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_164,2);
        AnsiString_Free(&local_160,2);
        iVar4 = 2;
        iVar3 = 10;
        iVar2 = 10;
        FUN_00425bac(&local_16c);
        FUN_00425bac(&local_168);
        FUN_00401db0(param_2,&local_168,&local_16c,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_16c,2);
        AnsiString_Free(&local_168,2);
        iVar4 = 1;
        iVar3 = 0;
        iVar2 = 2;
        FUN_00425bac(&local_174);
        FUN_00425bac(&local_170);
        FUN_00401db0(param_2,&local_170,&local_174,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_174,2);
        AnsiString_Free(&local_170,2);
        iVar4 = 1;
        iVar3 = 0;
        iVar2 = 2;
        FUN_00425bac(&local_17c);
        FUN_00425bac(&local_178);
        FUN_00401db0(param_2,&local_178,&local_17c,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_17c,2);
        AnsiString_Free(&local_178,2);
        iVar4 = 2;
        iVar3 = 0xe;
        iVar2 = 0xe;
        FUN_00425bac(&local_184);
        FUN_00425bac(&local_180);
        FUN_00401db0(param_2,&local_180,&local_184,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_184,2);
        AnsiString_Free(&local_180,2);
        iVar4 = 1;
        iVar3 = 0;
        iVar2 = 4;
        FUN_00425bac(&local_18c);
        FUN_00425bac(&local_188);
        FUN_00401db0(param_2,&local_188,&local_18c,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_18c,2);
        AnsiString_Free(&local_188,2);
        iVar4 = 1;
        iVar3 = 0;
        iVar2 = 4;
        FUN_00425bac(&local_194);
        FUN_00425bac(&local_190);
        FUN_00401db0(param_2,&local_190,&local_194,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_194,2);
        AnsiString_Free(&local_190,2);
        iVar4 = 1;
        iVar3 = 0;
        iVar2 = 4;
        FUN_00425bac(&local_19c);
        FUN_00425bac(&local_198);
        FUN_00401db0(param_2,&local_198,&local_19c,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_19c,2);
        AnsiString_Free(&local_198,2);
        iVar4 = 2;
        iVar3 = 0x14;
        iVar2 = 0x14;
        FUN_00425bac(&local_1a4);
        FUN_00425bac(&local_1a0);
        FUN_00401db0(param_2,&local_1a0,&local_1a4,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_1a4,2);
        AnsiString_Free(&local_1a0,2);
        iVar4 = 2;
        iVar3 = 0x14;
        iVar2 = 0x14;
        FUN_00425bac(&local_1ac);
        FUN_00425bac(&local_1a8);
        FUN_00401db0(param_2,&local_1a8,&local_1ac,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_1ac,2);
        AnsiString_Free(&local_1a8,2);
        iVar4 = 2;
        iVar3 = 0x14;
        iVar2 = 0x14;
        FUN_00425bac(&local_1b4);
        FUN_00425bac(&local_1b0);
        FUN_00401db0(param_2,&local_1b0,&local_1b4,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_1b4,2);
        AnsiString_Free(&local_1b0,2);
        iVar4 = 2;
        iVar3 = 0x14;
        iVar2 = 0x14;
        FUN_00425bac(&local_1bc);
        FUN_00425bac(&local_1b8);
        FUN_00401db0(param_2,&local_1b8,&local_1bc,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_1bc,2);
        AnsiString_Free(&local_1b8,2);
        iVar4 = 2;
        iVar3 = 5;
        iVar2 = 5;
        FUN_00425bac(&local_1c4);
        FUN_00425bac(&local_1c0);
        FUN_00401db0(param_2,&local_1c0,&local_1c4,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_1c4,2);
        AnsiString_Free(&local_1c0,2);
        iVar4 = 2;
        iVar3 = 10;
        iVar2 = 10;
        FUN_00425bac(&local_1cc);
        FUN_00425bac(&local_1c8);
        FUN_00401db0(param_2,&local_1c8,&local_1cc,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_1cc,2);
        AnsiString_Free(&local_1c8,2);
        iVar4 = 2;
        iVar3 = 5;
        iVar2 = 5;
        FUN_00425bac(&local_1d4);
        FUN_00425bac(&local_1d0);
        FUN_00401db0(param_2,&local_1d0,&local_1d4,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_1d4,2);
        AnsiString_Free(&local_1d0,2);
        iVar4 = 1;
        iVar3 = 0;
        iVar2 = 0;
        FUN_00425bac(&local_1dc);
        FUN_00425bac(&local_1d8);
        FUN_00401db0(param_2,&local_1d8,&local_1dc,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_1dc,2);
        AnsiString_Free(&local_1d8,2);
        goto LAB_00409973;
      }
      if (1 < iVar2 - 0x16aU) {
        uVar1 = iVar2 - 0x17c;
        goto joined_r0x00407ce1;
      }
    }
    else if (1 < iVar2 - 0x186U) {
      if ((1 < iVar2 - 0x1baU) && (1 < iVar2 - 0x1e0U)) {
        uVar1 = iVar2 - 0x282;
        goto joined_r0x00407cfe;
      }
LAB_004085ee:
      iVar4 = 1;
      iVar3 = 0;
      iVar2 = 1;
      FUN_00425bac(&local_ac);
      FUN_00425bac(&local_a8);
      FUN_00401db0(param_2,&local_a8,&local_ac,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_ac,2);
      AnsiString_Free(&local_a8,2);
      iVar4 = 2;
      iVar3 = 0x14;
      iVar2 = 0x14;
      FUN_00425bac(&local_b4);
      FUN_00425bac(&local_b0);
      FUN_00401db0(param_2,&local_b0,&local_b4,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_b4,2);
      AnsiString_Free(&local_b0,2);
      iVar4 = 1;
      iVar3 = 0;
      iVar2 = 4;
      FUN_00425bac(&local_bc);
      FUN_00425bac(&local_b8);
      FUN_00401db0(param_2,&local_b8,&local_bc,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_bc,2);
      AnsiString_Free(&local_b8,2);
      iVar4 = 3;
      iVar3 = 0;
      iVar2 = 3;
      FUN_00425bac(&local_c4);
      FUN_00425bac(&local_c0);
      FUN_00401db0(param_2,&local_c0,&local_c4,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_c4,2);
      AnsiString_Free(&local_c0,2);
      iVar4 = 2;
      iVar3 = 5;
      iVar2 = 5;
      FUN_00425bac(&local_cc);
      FUN_00425bac(&local_c8);
      FUN_00401db0(param_2,&local_c8,&local_cc,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_cc,2);
      AnsiString_Free(&local_c8,2);
      iVar4 = 1;
      iVar3 = 0;
      iVar2 = 2;
      FUN_00425bac(&local_d4);
      FUN_00425bac(&local_d0);
      FUN_00401db0(param_2,&local_d0,&local_d4,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_d4,2);
      AnsiString_Free(&local_d0,2);
      iVar4 = 2;
      iVar3 = 0x1e;
      iVar2 = 1;
      FUN_00425bac(&local_dc);
      FUN_00425bac(&local_d8);
      FUN_00401db0(param_2,&local_d8,&local_dc,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_dc,2);
      AnsiString_Free(&local_d8,2);
      iVar4 = 2;
      iVar3 = 10;
      iVar2 = 10;
      FUN_00425bac(&local_e4);
      FUN_00425bac(&local_e0);
      FUN_00401db0(param_2,&local_e0,&local_e4,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_e4,2);
      AnsiString_Free(&local_e0,2);
      iVar4 = 1;
      iVar3 = 0;
      iVar2 = 2;
      FUN_00425bac(&local_ec);
      FUN_00425bac(&local_e8);
      FUN_00401db0(param_2,&local_e8,&local_ec,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_ec,2);
      AnsiString_Free(&local_e8,2);
      iVar4 = 1;
      iVar3 = 0;
      iVar2 = 2;
      FUN_00425bac(&local_f4);
      FUN_00425bac(&local_f0);
      FUN_00401db0(param_2,&local_f0,&local_f4,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_f4,2);
      AnsiString_Free(&local_f0,2);
      iVar4 = 2;
      iVar3 = 0xe;
      iVar2 = 0xe;
      FUN_00425bac(&local_fc);
      FUN_00425bac(&local_f8);
      FUN_00401db0(param_2,&local_f8,&local_fc,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_fc,2);
      AnsiString_Free(&local_f8,2);
      iVar4 = 2;
      iVar3 = 10;
      iVar2 = 10;
      FUN_00425bac(&local_104);
      FUN_00425bac(&local_100);
      FUN_00401db0(param_2,&local_100,&local_104,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_104,2);
      AnsiString_Free(&local_100,2);
      iVar4 = 2;
      iVar3 = 0x14;
      iVar2 = 0x14;
      FUN_00425bac(&local_10c);
      FUN_00425bac(&local_108);
      FUN_00401db0(param_2,&local_108,&local_10c,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_10c,2);
      AnsiString_Free(&local_108,2);
      iVar4 = 1;
      iVar3 = 0;
      iVar2 = 2;
      FUN_00425bac(&local_114);
      FUN_00425bac(&local_110);
      FUN_00401db0(param_2,&local_110,&local_114,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_114,2);
      AnsiString_Free(&local_110,2);
      iVar4 = 2;
      iVar3 = 5;
      iVar2 = 5;
      FUN_00425bac(&local_11c);
      FUN_00425bac(&local_118);
      FUN_00401db0(param_2,&local_118,&local_11c,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_11c,2);
      AnsiString_Free(&local_118,2);
      iVar4 = 2;
      iVar3 = 10;
      iVar2 = 10;
      FUN_00425bac(&local_124);
      FUN_00425bac(&local_120);
      FUN_00401db0(param_2,&local_120,&local_124,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_124,2);
      AnsiString_Free(&local_120,2);
      iVar4 = 1;
      iVar3 = 0;
      iVar2 = 0;
      FUN_00425bac(&local_12c);
      FUN_00425bac(&local_128);
      FUN_00401db0(param_2,&local_128,&local_12c,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_12c,2);
      AnsiString_Free(&local_128,2);
      goto LAB_00409973;
    }
  }
  else if (iVar2 < 0x2e5) {
    if (((iVar2 != 0x2e4) && (3 < iVar2 - 0x2a8U)) && (1 < iVar2 - 0x2c8U)) {
      uVar1 = iVar2 - 0x2da;
joined_r0x00407ce1:
      if (1 < uVar1) goto LAB_00409973;
    }
  }
  else if ((iVar2 != 0x2e6) && (1 < iVar2 - 0x30cU)) {
    uVar1 = iVar2 - 0x34a;
joined_r0x00407cfe:
    if (1 < uVar1) goto LAB_00409973;
    goto LAB_004085ee;
  }
  iVar4 = 1;
  iVar3 = 0;
  iVar2 = 1;
  FUN_00425bac(&local_c);
  FUN_00425bac(&local_8);
  FUN_00401db0(param_2,&local_8,&local_c,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_c,2);
  AnsiString_Free(&local_8,2);
  iVar4 = 2;
  iVar3 = 0x14;
  iVar2 = 0x14;
  FUN_00425bac(&local_14);
  FUN_00425bac(&local_10);
  FUN_00401db0(param_2,&local_10,&local_14,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_14,2);
  AnsiString_Free(&local_10,2);
  iVar4 = 1;
  iVar3 = 0;
  iVar2 = 4;
  FUN_00425bac(&local_1c);
  FUN_00425bac(&local_18);
  FUN_00401db0(param_2,&local_18,&local_1c,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_1c,2);
  AnsiString_Free(&local_18,2);
  iVar4 = 3;
  iVar3 = 0;
  iVar2 = 3;
  FUN_00425bac(&local_24);
  FUN_00425bac(&local_20);
  FUN_00401db0(param_2,&local_20,&local_24,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_24,2);
  AnsiString_Free(&local_20,2);
  iVar4 = 2;
  iVar3 = 5;
  iVar2 = 5;
  FUN_00425bac(&local_2c);
  FUN_00425bac(&local_28);
  FUN_00401db0(param_2,&local_28,&local_2c,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_2c,2);
  AnsiString_Free(&local_28,2);
  iVar4 = 1;
  iVar3 = 0;
  iVar2 = 2;
  FUN_00425bac(&local_34);
  FUN_00425bac(&local_30);
  FUN_00401db0(param_2,&local_30,&local_34,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_34,2);
  AnsiString_Free(&local_30,2);
  iVar4 = 2;
  iVar3 = 0x1e;
  iVar2 = 1;
  FUN_00425bac(&local_3c);
  FUN_00425bac(&local_38);
  FUN_00401db0(param_2,&local_38,&local_3c,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_3c,2);
  AnsiString_Free(&local_38,2);
  iVar4 = 2;
  iVar3 = 10;
  iVar2 = 10;
  FUN_00425bac(&local_44);
  FUN_00425bac(&local_40);
  FUN_00401db0(param_2,&local_40,&local_44,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_44,2);
  AnsiString_Free(&local_40,2);
  iVar4 = 1;
  iVar3 = 0;
  iVar2 = 2;
  FUN_00425bac(&local_4c);
  FUN_00425bac(&local_48);
  FUN_00401db0(param_2,&local_48,&local_4c,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_4c,2);
  AnsiString_Free(&local_48,2);
  iVar4 = 1;
  iVar3 = 0;
  iVar2 = 2;
  FUN_00425bac(&local_54);
  FUN_00425bac(&local_50);
  FUN_00401db0(param_2,&local_50,&local_54,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_54,2);
  AnsiString_Free(&local_50,2);
  iVar4 = 2;
  iVar3 = 0xe;
  iVar2 = 0xe;
  FUN_00425bac(&local_5c);
  FUN_00425bac(&local_58);
  FUN_00401db0(param_2,&local_58,&local_5c,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_5c,2);
  AnsiString_Free(&local_58,2);
  iVar4 = 1;
  iVar3 = 0;
  iVar2 = 4;
  FUN_00425bac(&local_64);
  FUN_00425bac(&local_60);
  FUN_00401db0(param_2,&local_60,&local_64,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_64,2);
  AnsiString_Free(&local_60,2);
  iVar4 = 1;
  iVar3 = 0;
  iVar2 = 4;
  FUN_00425bac(&local_6c);
  FUN_00425bac(&local_68);
  FUN_00401db0(param_2,&local_68,&local_6c,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_6c,2);
  AnsiString_Free(&local_68,2);
  iVar4 = 1;
  iVar3 = 0;
  iVar2 = 4;
  FUN_00425bac(&local_74);
  FUN_00425bac(&local_70);
  FUN_00401db0(param_2,&local_70,&local_74,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_74,2);
  AnsiString_Free(&local_70,2);
  iVar4 = 2;
  iVar3 = 10;
  iVar2 = 10;
  FUN_00425bac(&local_7c);
  FUN_00425bac(&local_78);
  FUN_00401db0(param_2,&local_78,&local_7c,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_7c,2);
  AnsiString_Free(&local_78,2);
  iVar4 = 1;
  iVar3 = 0;
  iVar2 = 2;
  FUN_00425bac(&local_84);
  FUN_00425bac(&local_80);
  FUN_00401db0(param_2,&local_80,&local_84,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_84,2);
  AnsiString_Free(&local_80,2);
  iVar4 = 2;
  iVar3 = 5;
  iVar2 = 5;
  FUN_00425bac(&local_8c);
  FUN_00425bac(&local_88);
  FUN_00401db0(param_2,&local_88,&local_8c,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_8c,2);
  AnsiString_Free(&local_88,2);
  iVar4 = 2;
  iVar3 = 10;
  iVar2 = 10;
  FUN_00425bac(&local_94);
  FUN_00425bac(&local_90);
  FUN_00401db0(param_2,&local_90,&local_94,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_94,2);
  AnsiString_Free(&local_90,2);
  iVar4 = 2;
  iVar3 = 5;
  iVar2 = 5;
  FUN_00425bac(&local_9c);
  FUN_00425bac(&local_98);
  FUN_00401db0(param_2,&local_98,&local_9c,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_9c,2);
  AnsiString_Free(&local_98,2);
  iVar4 = 1;
  iVar3 = 0;
  iVar2 = 0;
  FUN_00425bac(&local_a4);
  FUN_00425bac(&local_a0);
  FUN_00401db0(param_2,&local_a0,&local_a4,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_a4,2);
  AnsiString_Free(&local_a0,2);
LAB_00409973:
  *in_FS_OFFSET = local_200;
  return;
}



void FUN_00409984(int param_1,int *param_2)

{
  uint uVar1;
  undefined4 *in_FS_OFFSET;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 local_1b8;
  int local_194;
  int local_190;
  int local_18c;
  int local_188;
  int local_184;
  int local_180;
  int local_17c;
  int local_178;
  int local_174;
  int local_170;
  int local_16c;
  int local_168;
  int local_164;
  int local_160;
  int local_15c;
  int local_158;
  int local_154;
  int local_150;
  int local_14c;
  int local_148;
  int local_144;
  int local_140;
  int local_13c;
  int local_138;
  int local_134;
  int local_130;
  int local_12c;
  int local_128;
  int local_124;
  int local_120;
  int local_11c;
  int local_118;
  int local_114;
  int local_110;
  int local_10c;
  int local_108;
  int local_104;
  int local_100;
  int local_fc;
  int local_f8;
  int local_f4;
  int local_f0;
  int local_ec;
  int local_e8;
  int local_e4;
  int local_e0;
  int local_dc;
  int local_d8;
  int local_d4;
  int local_d0;
  int local_cc;
  int local_c8;
  int local_c4;
  int local_c0;
  int local_bc;
  int local_b8;
  int local_b4;
  int local_b0;
  int local_ac;
  int local_a8;
  int local_a4;
  int local_a0;
  int local_9c;
  int local_98;
  int local_94;
  int local_90;
  int local_8c;
  int local_88;
  int local_84;
  int local_80;
  int local_7c;
  int local_78;
  int local_74;
  int local_70;
  int local_6c;
  int local_68;
  int local_64;
  int local_60;
  int local_5c;
  int local_58;
  int local_54;
  int local_50;
  int local_4c;
  int local_48;
  int local_44;
  int local_40;
  int local_3c;
  int local_38;
  int local_34;
  int local_30;
  int local_2c;
  int local_28;
  int local_24;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42d170);
  iVar2 = *(int *)(param_1 + 0xc);
  if (iVar2 < 0x2a8) {
    if (iVar2 < 0x186) {
      if (iVar2 - 0x96U < 2) {
        iVar4 = 1;
        iVar3 = 0;
        iVar2 = 1;
        FUN_00425bac(&local_f4);
        FUN_00425bac(&local_f0);
        FUN_00401db0(param_2,&local_f0,&local_f4,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_f4,2);
        AnsiString_Free(&local_f0,2);
        iVar4 = 1;
        iVar3 = 0;
        iVar2 = 4;
        FUN_00425bac(&local_fc);
        FUN_00425bac(&local_f8);
        FUN_00401db0(param_2,&local_f8,&local_fc,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_fc,2);
        AnsiString_Free(&local_f8,2);
        iVar4 = 3;
        iVar3 = 0;
        iVar2 = 3;
        FUN_00425bac(&local_104);
        FUN_00425bac(&local_100);
        FUN_00401db0(param_2,&local_100,&local_104,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_104,2);
        AnsiString_Free(&local_100,2);
        iVar4 = 2;
        iVar3 = 5;
        iVar2 = 5;
        FUN_00425bac(&local_10c);
        FUN_00425bac(&local_108);
        FUN_00401db0(param_2,&local_108,&local_10c,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_10c,2);
        AnsiString_Free(&local_108,2);
        iVar4 = 1;
        iVar3 = 0;
        iVar2 = 2;
        FUN_00425bac(&local_114);
        FUN_00425bac(&local_110);
        FUN_00401db0(param_2,&local_110,&local_114,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_114,2);
        AnsiString_Free(&local_110,2);
        iVar4 = 2;
        iVar3 = 0x1e;
        iVar2 = 1;
        FUN_00425bac(&local_11c);
        FUN_00425bac(&local_118);
        FUN_00401db0(param_2,&local_118,&local_11c,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_11c,2);
        AnsiString_Free(&local_118,2);
        iVar4 = 2;
        iVar3 = 10;
        iVar2 = 10;
        FUN_00425bac(&local_124);
        FUN_00425bac(&local_120);
        FUN_00401db0(param_2,&local_120,&local_124,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_124,2);
        AnsiString_Free(&local_120,2);
        iVar4 = 1;
        iVar3 = 0;
        iVar2 = 2;
        FUN_00425bac(&local_12c);
        FUN_00425bac(&local_128);
        FUN_00401db0(param_2,&local_128,&local_12c,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_12c,2);
        AnsiString_Free(&local_128,2);
        iVar4 = 1;
        iVar3 = 0;
        iVar2 = 2;
        FUN_00425bac(&local_134);
        FUN_00425bac(&local_130);
        FUN_00401db0(param_2,&local_130,&local_134,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_134,2);
        AnsiString_Free(&local_130,2);
        iVar4 = 2;
        iVar3 = 0xe;
        iVar2 = 0xe;
        FUN_00425bac(&local_13c);
        FUN_00425bac(&local_138);
        FUN_00401db0(param_2,&local_138,&local_13c,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_13c,2);
        AnsiString_Free(&local_138,2);
        iVar4 = 1;
        iVar3 = 0;
        iVar2 = 4;
        FUN_00425bac(&local_144);
        FUN_00425bac(&local_140);
        FUN_00401db0(param_2,&local_140,&local_144,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_144,2);
        AnsiString_Free(&local_140,2);
        iVar4 = 1;
        iVar3 = 0;
        iVar2 = 4;
        FUN_00425bac(&local_14c);
        FUN_00425bac(&local_148);
        FUN_00401db0(param_2,&local_148,&local_14c,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_14c,2);
        AnsiString_Free(&local_148,2);
        iVar4 = 1;
        iVar3 = 0;
        iVar2 = 4;
        FUN_00425bac(&local_154);
        FUN_00425bac(&local_150);
        FUN_00401db0(param_2,&local_150,&local_154,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_154,2);
        AnsiString_Free(&local_150,2);
        iVar4 = 2;
        iVar3 = 0x14;
        iVar2 = 0x14;
        FUN_00425bac(&local_15c);
        FUN_00425bac(&local_158);
        FUN_00401db0(param_2,&local_158,&local_15c,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_15c,2);
        AnsiString_Free(&local_158,2);
        iVar4 = 2;
        iVar3 = 0x14;
        iVar2 = 0x14;
        FUN_00425bac(&local_164);
        FUN_00425bac(&local_160);
        FUN_00401db0(param_2,&local_160,&local_164,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_164,2);
        AnsiString_Free(&local_160,2);
        iVar4 = 2;
        iVar3 = 0x14;
        iVar2 = 0x14;
        FUN_00425bac(&local_16c);
        FUN_00425bac(&local_168);
        FUN_00401db0(param_2,&local_168,&local_16c,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_16c,2);
        AnsiString_Free(&local_168,2);
        iVar4 = 2;
        iVar3 = 0x14;
        iVar2 = 0x14;
        FUN_00425bac(&local_174);
        FUN_00425bac(&local_170);
        FUN_00401db0(param_2,&local_170,&local_174,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_174,2);
        AnsiString_Free(&local_170,2);
        iVar4 = 2;
        iVar3 = 0x14;
        iVar2 = 0x14;
        FUN_00425bac(&local_17c);
        FUN_00425bac(&local_178);
        FUN_00401db0(param_2,&local_178,&local_17c,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_17c,2);
        AnsiString_Free(&local_178,2);
        iVar4 = 1;
        iVar3 = 0;
        iVar2 = 1;
        FUN_00425bac(&local_184);
        FUN_00425bac(&local_180);
        FUN_00401db0(param_2,&local_180,&local_184,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_184,2);
        AnsiString_Free(&local_180,2);
        iVar4 = 2;
        iVar3 = 0x14;
        iVar2 = 0x14;
        FUN_00425bac(&local_18c);
        FUN_00425bac(&local_188);
        FUN_00401db0(param_2,&local_188,&local_18c,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_18c,2);
        AnsiString_Free(&local_188,2);
        iVar4 = 1;
        iVar3 = 0;
        iVar2 = 0;
        FUN_00425bac(&local_194);
        FUN_00425bac(&local_190);
        FUN_00401db0(param_2,&local_190,&local_194,iVar2,iVar3,iVar4);
        AnsiString_Free(&local_194,2);
        AnsiString_Free(&local_190,2);
        goto LAB_0040b257;
      }
      if (1 < iVar2 - 0x16aU) {
        uVar1 = iVar2 - 0x17c;
        goto joined_r0x00409a45;
      }
    }
    else if (1 < iVar2 - 0x186U) {
      if ((1 < iVar2 - 0x1baU) && (1 < iVar2 - 0x1e0U)) {
        uVar1 = iVar2 - 0x282;
        goto joined_r0x00409a62;
      }
LAB_0040a152:
      iVar4 = 1;
      iVar3 = 0;
      iVar2 = 1;
      FUN_00425bac(&local_8c);
      FUN_00425bac(&local_88);
      FUN_00401db0(param_2,&local_88,&local_8c,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_8c,2);
      AnsiString_Free(&local_88,2);
      iVar4 = 1;
      iVar3 = 0;
      iVar2 = 4;
      FUN_00425bac(&local_94);
      FUN_00425bac(&local_90);
      FUN_00401db0(param_2,&local_90,&local_94,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_94,2);
      AnsiString_Free(&local_90,2);
      iVar4 = 3;
      iVar3 = 0;
      iVar2 = 3;
      FUN_00425bac(&local_9c);
      FUN_00425bac(&local_98);
      FUN_00401db0(param_2,&local_98,&local_9c,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_9c,2);
      AnsiString_Free(&local_98,2);
      iVar4 = 2;
      iVar3 = 5;
      iVar2 = 5;
      FUN_00425bac(&local_a4);
      FUN_00425bac(&local_a0);
      FUN_00401db0(param_2,&local_a0,&local_a4,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_a4,2);
      AnsiString_Free(&local_a0,2);
      iVar4 = 1;
      iVar3 = 0;
      iVar2 = 2;
      FUN_00425bac(&local_ac);
      FUN_00425bac(&local_a8);
      FUN_00401db0(param_2,&local_a8,&local_ac,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_ac,2);
      AnsiString_Free(&local_a8,2);
      iVar4 = 2;
      iVar3 = 0x1e;
      iVar2 = 1;
      FUN_00425bac(&local_b4);
      FUN_00425bac(&local_b0);
      FUN_00401db0(param_2,&local_b0,&local_b4,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_b4,2);
      AnsiString_Free(&local_b0,2);
      iVar4 = 2;
      iVar3 = 10;
      iVar2 = 10;
      FUN_00425bac(&local_bc);
      FUN_00425bac(&local_b8);
      FUN_00401db0(param_2,&local_b8,&local_bc,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_bc,2);
      AnsiString_Free(&local_b8,2);
      iVar4 = 1;
      iVar3 = 0;
      iVar2 = 2;
      FUN_00425bac(&local_c4);
      FUN_00425bac(&local_c0);
      FUN_00401db0(param_2,&local_c0,&local_c4,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_c4,2);
      AnsiString_Free(&local_c0,2);
      iVar4 = 1;
      iVar3 = 0;
      iVar2 = 2;
      FUN_00425bac(&local_cc);
      FUN_00425bac(&local_c8);
      FUN_00401db0(param_2,&local_c8,&local_cc,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_cc,2);
      AnsiString_Free(&local_c8,2);
      iVar4 = 2;
      iVar3 = 0xe;
      iVar2 = 0xe;
      FUN_00425bac(&local_d4);
      FUN_00425bac(&local_d0);
      FUN_00401db0(param_2,&local_d0,&local_d4,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_d4,2);
      AnsiString_Free(&local_d0,2);
      iVar4 = 1;
      iVar3 = 0;
      iVar2 = 1;
      FUN_00425bac(&local_dc);
      FUN_00425bac(&local_d8);
      FUN_00401db0(param_2,&local_d8,&local_dc,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_dc,2);
      AnsiString_Free(&local_d8,2);
      iVar4 = 2;
      iVar3 = 0x14;
      iVar2 = 0x14;
      FUN_00425bac(&local_e4);
      FUN_00425bac(&local_e0);
      FUN_00401db0(param_2,&local_e0,&local_e4,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_e4,2);
      AnsiString_Free(&local_e0,2);
      iVar4 = 1;
      iVar3 = 0;
      iVar2 = 0;
      FUN_00425bac(&local_ec);
      FUN_00425bac(&local_e8);
      FUN_00401db0(param_2,&local_e8,&local_ec,iVar2,iVar3,iVar4);
      AnsiString_Free(&local_ec,2);
      AnsiString_Free(&local_e8,2);
      goto LAB_0040b257;
    }
  }
  else if (iVar2 < 0x2e5) {
    if (((iVar2 != 0x2e4) && (3 < iVar2 - 0x2a8U)) && (1 < iVar2 - 0x2c8U)) {
      uVar1 = iVar2 - 0x2da;
joined_r0x00409a45:
      if (1 < uVar1) goto LAB_0040b257;
    }
  }
  else if ((iVar2 != 0x2e6) && (1 < iVar2 - 0x30cU)) {
    uVar1 = iVar2 - 0x34a;
joined_r0x00409a62:
    if (1 < uVar1) goto LAB_0040b257;
    goto LAB_0040a152;
  }
  iVar4 = 1;
  iVar3 = 0;
  iVar2 = 1;
  FUN_00425bac(&local_c);
  FUN_00425bac(&local_8);
  FUN_00401db0(param_2,&local_8,&local_c,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_c,2);
  AnsiString_Free(&local_8,2);
  iVar4 = 1;
  iVar3 = 0;
  iVar2 = 4;
  FUN_00425bac(&local_14);
  FUN_00425bac(&local_10);
  FUN_00401db0(param_2,&local_10,&local_14,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_14,2);
  AnsiString_Free(&local_10,2);
  iVar4 = 3;
  iVar3 = 0;
  iVar2 = 3;
  FUN_00425bac(&local_1c);
  FUN_00425bac(&local_18);
  FUN_00401db0(param_2,&local_18,&local_1c,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_1c,2);
  AnsiString_Free(&local_18,2);
  iVar4 = 2;
  iVar3 = 5;
  iVar2 = 5;
  FUN_00425bac(&local_24);
  FUN_00425bac(&local_20);
  FUN_00401db0(param_2,&local_20,&local_24,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_24,2);
  AnsiString_Free(&local_20,2);
  iVar4 = 1;
  iVar3 = 0;
  iVar2 = 2;
  FUN_00425bac(&local_2c);
  FUN_00425bac(&local_28);
  FUN_00401db0(param_2,&local_28,&local_2c,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_2c,2);
  AnsiString_Free(&local_28,2);
  iVar4 = 2;
  iVar3 = 0x1e;
  iVar2 = 1;
  FUN_00425bac(&local_34);
  FUN_00425bac(&local_30);
  FUN_00401db0(param_2,&local_30,&local_34,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_34,2);
  AnsiString_Free(&local_30,2);
  iVar4 = 2;
  iVar3 = 10;
  iVar2 = 10;
  FUN_00425bac(&local_3c);
  FUN_00425bac(&local_38);
  FUN_00401db0(param_2,&local_38,&local_3c,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_3c,2);
  AnsiString_Free(&local_38,2);
  iVar4 = 1;
  iVar3 = 0;
  iVar2 = 2;
  FUN_00425bac(&local_44);
  FUN_00425bac(&local_40);
  FUN_00401db0(param_2,&local_40,&local_44,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_44,2);
  AnsiString_Free(&local_40,2);
  iVar4 = 1;
  iVar3 = 0;
  iVar2 = 2;
  FUN_00425bac(&local_4c);
  FUN_00425bac(&local_48);
  FUN_00401db0(param_2,&local_48,&local_4c,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_4c,2);
  AnsiString_Free(&local_48,2);
  iVar4 = 2;
  iVar3 = 0xe;
  iVar2 = 0xe;
  FUN_00425bac(&local_54);
  FUN_00425bac(&local_50);
  FUN_00401db0(param_2,&local_50,&local_54,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_54,2);
  AnsiString_Free(&local_50,2);
  iVar4 = 1;
  iVar3 = 0;
  iVar2 = 4;
  FUN_00425bac(&local_5c);
  FUN_00425bac(&local_58);
  FUN_00401db0(param_2,&local_58,&local_5c,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_5c,2);
  AnsiString_Free(&local_58,2);
  iVar4 = 1;
  iVar3 = 0;
  iVar2 = 4;
  FUN_00425bac(&local_64);
  FUN_00425bac(&local_60);
  FUN_00401db0(param_2,&local_60,&local_64,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_64,2);
  AnsiString_Free(&local_60,2);
  iVar4 = 1;
  iVar3 = 0;
  iVar2 = 4;
  FUN_00425bac(&local_6c);
  FUN_00425bac(&local_68);
  FUN_00401db0(param_2,&local_68,&local_6c,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_6c,2);
  AnsiString_Free(&local_68,2);
  iVar4 = 1;
  iVar3 = 0;
  iVar2 = 1;
  FUN_00425bac(&local_74);
  FUN_00425bac(&local_70);
  FUN_00401db0(param_2,&local_70,&local_74,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_74,2);
  AnsiString_Free(&local_70,2);
  iVar4 = 2;
  iVar3 = 0x14;
  iVar2 = 0x14;
  FUN_00425bac(&local_7c);
  FUN_00425bac(&local_78);
  FUN_00401db0(param_2,&local_78,&local_7c,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_7c,2);
  AnsiString_Free(&local_78,2);
  iVar4 = 1;
  iVar3 = 0;
  iVar2 = 0;
  FUN_00425bac(&local_84);
  FUN_00425bac(&local_80);
  FUN_00401db0(param_2,&local_80,&local_84,iVar2,iVar3,iVar4);
  AnsiString_Free(&local_84,2);
  AnsiString_Free(&local_80,2);
LAB_0040b257:
  *in_FS_OFFSET = local_1b8;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __stdcall FUN_0040b298(void)

{
  undefined4 uVar1;
  undefined4 *in_FS_OFFSET;
  undefined1 auStack_10 [12];
  
  uVar1 = *in_FS_OFFSET;
  *in_FS_OFFSET = auStack_10;
  _DAT_00430838 = _DAT_00430838 + 1;
  *in_FS_OFFSET = uVar1;
  return;
}



int FUN_0040b2fc(int param_1)

{
  return param_1 + 2 + (uint)*(byte *)(param_1 + 1);
}



int FUN_0040b308(int param_1,int param_2)

{
  int *piVar1;
  int iVar2;
  uint uVar3;
  undefined1 *puVar4;
  
  if (param_2 != 0) {
    iVar2 = **(int **)(param_1 + 0xb + (uint)*(byte *)(param_1 + 1));
    uVar3 = (uint)*(byte *)(iVar2 + 1);
    puVar4 = (undefined1 *)(iVar2 + 0xf + uVar3);
    piVar1 = (int *)(iVar2 + 7 + uVar3);
    iVar2 = 0;
    do {
      uVar3 = CONCAT31((int3)(uVar3 >> 8),*puVar4);
      if (uVar3 == *(uint *)(param_2 + -4)) {
        while (((*(byte *)(param_2 + -1 + uVar3) ^ puVar4[uVar3]) & 0xdf) == 0) {
          uVar3 = uVar3 - 1;
          if (uVar3 == 0) {
            return iVar2;
          }
        }
        uVar3 = CONCAT31((int3)(uVar3 >> 8),*puVar4);
      }
      iVar2 = iVar2 + 1;
      puVar4 = puVar4 + uVar3 + 1;
    } while (iVar2 <= *piVar1);
  }
  return -1;
}



void FUN_0040b354(char *param_1,byte *param_2)

{
  if (*param_1 == '\x01') {
    FUN_00414238(param_2);
    return;
  }
  FUN_0040b308((int)param_1,(int)param_2);
  return;
}



void FUN_0040b368(int param_1,undefined1 *param_2)

{
  int iVar1;
  int iVar2;
  undefined1 uVar3;
  byte bVar5;
  undefined1 *puVar4;
  undefined2 uVar6;
  uint uVar7;
  undefined1 *puVar8;
  int iVar9;
  uint uVar10;
  
  puVar4 = param_2;
  if (param_2 != (undefined1 *)0x0) {
    puVar4 = (undefined1 *)(CONCAT11(*param_2,param_2[-4]) & 0xdfff);
  }
  while( true ) {
    iVar1 = param_1 + 2 + (uint)*(byte *)(param_1 + 1);
    uVar7 = (uint)*(byte *)(iVar1 + 10);
    uVar10 = (uint)*(ushort *)(iVar1 + 0xb + uVar7);
    if (uVar10 != 0) {
      iVar2 = iVar1 + 0xd + uVar7;
      do {
        puVar8 = (undefined1 *)
                 (CONCAT22((short)(uVar7 >> 0x10),*(undefined2 *)(iVar2 + 0x1a)) & 0xffffdfff);
        if (puVar8 == puVar4) {
          iVar9 = CONCAT22((short)((uint)puVar8 >> 0x10),(ushort)(byte)puVar8);
          while( true ) {
            uVar6 = (undefined2)((uint)puVar4 >> 0x10);
            uVar3 = SUB41(puVar4,0);
            bVar5 = param_2[iVar9 + -1] ^ *(byte *)(iVar2 + 0x1a + iVar9);
            puVar4 = (undefined1 *)CONCAT22(uVar6,CONCAT11(bVar5,uVar3));
            if ((bVar5 & 0xdf) != 0) break;
            iVar9 = iVar9 + -1;
            if (iVar9 == 0) {
              return;
            }
          }
          puVar4 = (undefined1 *)(CONCAT22(uVar6,CONCAT11(*param_2,uVar3)) & 0xffffdfff);
          puVar8 = (undefined1 *)CONCAT31((int3)((uint)iVar9 >> 8),*(undefined1 *)(iVar2 + 0x1a));
        }
        uVar7 = CONCAT22((short)((uint)puVar8 >> 0x10),(ushort)(byte)puVar8);
        uVar10 = uVar10 - 1;
        iVar2 = iVar2 + 0x1b + uVar7;
      } while (uVar10 != 0);
    }
    if (*(int **)(iVar1 + 4) == (int *)0x0) break;
    param_1 = **(int **)(iVar1 + 4);
  }
  return;
}



uint FUN_0040b3d4(int *param_1,undefined4 *param_2)

{
  char *pcVar1;
  code *pcVar2;
  uint uVar3;
  uint uVar4;
  uint *puVar5;
  byte bVar6;
  
  pcVar1 = *(char **)*param_2;
  bVar6 = 4;
  if (*pcVar1 != '\a') {
    bVar6 = pcVar1[(byte)pcVar1[1] + 2];
  }
  pcVar2 = (code *)param_2[1];
  if (*(byte *)((int)param_2 + 7) < 0xfe) {
    uVar3 = (*pcVar2)(param_1,param_2[4]);
  }
  else if (*(byte *)((int)param_2 + 7) == 0xff) {
    puVar5 = (uint *)(((uint)pcVar2 & 0xffffff) + (int)param_1);
    uVar3 = CONCAT31((int3)((uint)param_1 >> 8),(char)*puVar5);
    if ((1 < bVar6) && (uVar3 = CONCAT22((short)((uint)param_1 >> 0x10),(short)*puVar5), 3 < bVar6))
    {
      uVar3 = *puVar5;
    }
  }
  else {
    uVar3 = (**(code **)((int)(short)pcVar2 + *param_1))();
  }
  uVar4 = uVar3;
  if (bVar6 < 4) {
    if (bVar6 < 2) {
      uVar4 = (uint)(char)uVar3;
      if (bVar6 != 0) {
        return uVar3 & 0xff;
      }
    }
    else {
      uVar4 = (uint)(short)uVar3;
      if (bVar6 != 2) {
        uVar4 = uVar3 & 0xffff;
      }
    }
  }
  return uVar4;
}



void FUN_0040b448(int *param_1,undefined4 *param_2,int param_3)

{
  char *pcVar1;
  code *pcVar2;
  int *piVar3;
  int iVar4;
  byte bVar5;
  
  pcVar1 = *(char **)*param_2;
  bVar5 = 4;
  if (*pcVar1 != '\a') {
    bVar5 = pcVar1[(byte)pcVar1[1] + 2];
  }
  iVar4 = param_2[4];
  if (param_2[4] == -0x80000000) {
    iVar4 = param_3;
  }
  pcVar2 = (code *)param_2[2];
  if (*(byte *)((int)param_2 + 0xb) == 0xff) {
    piVar3 = (int *)((int)param_1 + ((uint)pcVar2 & 0xffffff));
    *(char *)piVar3 = (char)param_3;
    if ((1 < bVar5) && (*(short *)piVar3 = (short)param_3, 3 < bVar5)) {
      *piVar3 = param_3;
    }
  }
  else if (*(byte *)((int)param_2 + 0xb) < 0xfe) {
    (*pcVar2)(param_1,iVar4);
  }
  else {
    (**(code **)((int)(short)pcVar2 + *param_1))();
  }
  return;
}



void FUN_0040b4a4(int *param_1,undefined4 *param_2,byte *param_3)

{
  byte bVar1;
  code *pcVar2;
  byte bVar3;
  uint uVar4;
  byte *pbVar5;
  
  pbVar5 = (byte *)param_2[4];
  if ((byte *)param_2[4] == (byte *)0x80000000) {
    pbVar5 = param_3;
  }
  pcVar2 = (code *)param_2[2];
  if (*(byte *)((int)param_2 + 0xb) != 0xff) {
    if (0xfd < *(byte *)((int)param_2 + 0xb)) {
      (**(code **)((int)(short)pcVar2 + *param_1))();
      return;
    }
    (*pcVar2)(param_1,pbVar5);
    return;
  }
  pbVar5 = (byte *)(((uint)pcVar2 & 0xffffff) + (int)param_1);
  bVar1 = *(byte *)(*(int *)*param_2 + 2 + (uint)*(byte *)(*(int *)*param_2 + 1));
  bVar3 = *param_3;
  if (bVar1 <= *param_3) {
    bVar3 = bVar1;
  }
  *pbVar5 = bVar3;
  uVar4 = (uint)bVar3;
  while( true ) {
    pbVar5 = pbVar5 + 1;
    param_3 = param_3 + 1;
    if (uVar4 == 0) break;
    uVar4 = uVar4 - 1;
    *pbVar5 = *param_3;
  }
  return;
}



void FUN_0040b4f8(int *param_1,undefined4 *param_2,undefined4 *param_3)

{
  byte abStack_10c [256];
  
  FUN_0041ba9c(abStack_10c,param_3,0xff);
  FUN_0040b4a4(param_1,param_2,abStack_10c);
  return;
}



void FUN_0040b52c(int *param_1,undefined4 *param_2)

{
  FUN_0041b894(param_1,param_2);
  return;
}



void FUN_0040b540(int *param_1,int param_2,undefined4 *param_3)

{
  code *pcVar1;
  undefined4 *puVar2;
  
  puVar2 = *(undefined4 **)(param_2 + 0x10);
  if (*(undefined4 **)(param_2 + 0x10) == (undefined4 *)0x80000000) {
    puVar2 = param_3;
  }
  pcVar1 = *(code **)(param_2 + 8);
  if (*(byte *)(param_2 + 0xb) != 0xff) {
    if (0xfd < *(byte *)(param_2 + 0xb)) {
      (**(code **)((int)(short)pcVar1 + *param_1))();
      return;
    }
    (*pcVar1)(param_1,puVar2);
    return;
  }
  FUN_0040b52c((int *)((int)param_1 + ((uint)pcVar1 & 0xffffff)),param_3);
  return;
}



void FUN_0040b580(BSTR *param_1,OLECHAR *param_2)

{
  FUN_0041be40(param_1,param_2);
  return;
}



void FUN_0040b594(int *param_1,int param_2,OLECHAR *param_3)

{
  code *pcVar1;
  OLECHAR *pOVar2;
  
  pOVar2 = *(OLECHAR **)(param_2 + 0x10);
  if (*(OLECHAR **)(param_2 + 0x10) == (OLECHAR *)0x80000000) {
    pOVar2 = param_3;
  }
  pcVar1 = *(code **)(param_2 + 8);
  if (*(byte *)(param_2 + 0xb) != 0xff) {
    if (0xfd < *(byte *)(param_2 + 0xb)) {
      (**(code **)((int)(short)pcVar1 + *param_1))();
      return;
    }
    (*pcVar1)(param_1,pOVar2);
    return;
  }
  FUN_0040b580((BSTR *)((int)param_1 + ((uint)pcVar1 & 0xffffff)),param_3);
  return;
}



void FUN_0040b5d4(int *param_1,int param_2,LPCSTR param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_20;
  undefined1 *puStack_1c;
  undefined1 *puStack_18;
  OLECHAR *local_8;
  
  puStack_18 = &stack0xfffffffc;
  local_8 = (OLECHAR *)0x0;
  puStack_1c = &LAB_0040b61c;
  uStack_20 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_20;
  FUN_0041bf0c((BSTR)&local_8,param_3);
  FUN_0040b594(param_1,param_2,local_8);
  *in_FS_OFFSET = uStack_20;
  puStack_18 = &LAB_0040b623;
  puStack_1c = (undefined1 *)0x40b61b;
  FUN_0041be04(&local_8);
  return;
}



void FUN_0040b62c(int *param_1,undefined4 *param_2,LPCSTR param_3)

{
  char cVar1;
  
  cVar1 = **(char **)*param_2;
  if (cVar1 == '\x05') {
    FUN_0040b4f8(param_1,param_2,(undefined4 *)param_3);
    return;
  }
  if (cVar1 == '\n') {
    FUN_0040b540(param_1,(int)param_2,(undefined4 *)param_3);
    return;
  }
  if (cVar1 == '\v') {
    FUN_0040b5d4(param_1,(int)param_2,param_3);
  }
  return;
}



// WARNING: Unable to track spacebase fully for stack

void FUN_0040b658(undefined4 param_1,undefined4 *param_2)

{
  code *pcVar1;
  undefined4 uVar2;
  int *piVar3;
  code *extraout_ECX;
  int extraout_EDX;
  int iVar4;
  undefined1 *puVar5;
  undefined4 uStack_10;
  
  iVar4 = (uint)*(byte *)(*(int *)*param_2 + 2 + (uint)*(byte *)(*(int *)*param_2 + 1)) * 4;
  if (*(char *)((int)param_2 + 0xb) == -1) {
    uStack_10 = 0x40b6e9;
    (**(code **)(&DAT_0040b6b1 + iVar4))(param_1,param_2,param_2[2] & 0xffffff);
  }
  else {
    pcVar1 = *(code **)(&DAT_0040b6b1 + iVar4);
    puVar5 = (undefined1 *)((int)&uStack_10 + -*(int *)(&DAT_0040b69d + iVar4));
    *(undefined4 *)((int)&uStack_10 + -*(int *)(&DAT_0040b69d + iVar4)) = 0x40b687;
    piVar3 = (int *)(*pcVar1)();
    uVar2 = *(undefined4 *)(extraout_EDX + 0x10);
    if (*(byte *)(extraout_EDX + 0xb) < 0xfe) {
      *(undefined4 *)(puVar5 + -4) = 0x40b69b;
      (*extraout_ECX)(piVar3,uVar2);
    }
    else {
      pcVar1 = *(code **)((int)(short)extraout_ECX + *piVar3);
      *(undefined4 *)(puVar5 + -4) = 0x40b697;
      (*pcVar1)();
    }
  }
  return;
}



void FUN_0040b6f0(VARIANT *param_1,VARIANTARG *param_2)

{
  FUN_0041c570(param_1,param_2);
  return;
}



void FUN_0040b704(int *param_1,int param_2,VARIANTARG *param_3)

{
  code *pcVar1;
  VARIANTARG *pVVar2;
  
  pVVar2 = *(VARIANTARG **)(param_2 + 0x10);
  if (*(VARIANTARG **)(param_2 + 0x10) == (VARIANTARG *)0x80000000) {
    pVVar2 = param_3;
  }
  pcVar1 = *(code **)(param_2 + 8);
  if (*(byte *)(param_2 + 0xb) != 0xff) {
    if (0xfd < *(byte *)(param_2 + 0xb)) {
      (**(code **)((int)(short)pcVar1 + *param_1))();
      return;
    }
    (*pcVar1)(param_1,pVVar2);
    return;
  }
  FUN_0040b6f0((VARIANT *)((int)param_1 + ((uint)pcVar1 & 0xffffff)),param_3);
  return;
}



void FUN_0040b744(int *param_1,int param_2,undefined4 *param_3)

{
  code *pcVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  
  pcVar1 = *(code **)(param_2 + 8);
  if (*(byte *)(param_2 + 0xb) == 0xff) {
    puVar3 = (undefined4 *)((int)param_1 + ((uint)pcVar1 & 0xffffff));
    uVar2 = param_3[1];
    *puVar3 = *param_3;
    puVar3[1] = uVar2;
    return;
  }
  if (0xfd < *(byte *)(param_2 + 0xb)) {
    (**(code **)((int)(short)pcVar1 + *param_1))();
    return;
  }
  (*pcVar1)(param_1,*(undefined4 *)(param_2 + 0x10),param_3,*param_3,param_3[1]);
  return;
}



void FUN_0040b77c(int *param_1,int param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5)

{
  code *pcVar1;
  undefined4 *puVar2;
  
  pcVar1 = *(code **)(param_2 + 8);
  if (*(byte *)(param_2 + 0xb) == 0xff) {
    puVar2 = (undefined4 *)((int)param_1 + ((uint)pcVar1 & 0xffffff));
    *puVar2 = param_4;
    puVar2[1] = param_5;
  }
  else if (*(byte *)(param_2 + 0xb) < 0xfe) {
    (*pcVar1)(param_1,*(undefined4 *)(param_2 + 0x10),pcVar1,param_4,param_5);
  }
  else {
    (**(code **)((int)(short)pcVar1 + *param_1))();
  }
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __stdcall FUN_0040b7b8(void)

{
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_10;
  undefined1 *puStack_c;
  undefined1 *puStack_8;
  
  puStack_8 = &stack0xfffffffc;
  puStack_c = &LAB_0040b7fe;
  uStack_10 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_10;
  _DAT_0043083c = _DAT_0043083c + 1;
  if (_DAT_0043083c == 0) {
    FUN_0041b840((int *)&PTR_DAT_0042d3d8);
    FUN_0041c0dc((VARIANTARG *)&PTR_s_False_0042d3d0,&LAB_004184a8,2);
  }
  *in_FS_OFFSET = uStack_10;
  return;
}



undefined4 FUN_0040c594(int param_1)

{
  undefined4 uVar1;
  
  uVar1 = 0;
  if (*(int *)(param_1 + -0x38) != 0) {
    uVar1 = *(undefined4 *)(*(int *)(param_1 + -0x38) + 2);
  }
  return uVar1;
}



void FUN_0040c5a0(undefined4 param_1)

{
  undefined4 uVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 uVar2;
  undefined4 *puVar3;
  undefined1 *puStack_20;
  undefined1 *puStack_1c;
  undefined1 *puStack_18;
  byte *local_10;
  undefined4 local_c;
  undefined1 local_8;
  
  puStack_18 = &stack0xfffffffc;
  local_10 = (byte *)0x0;
  puStack_1c = &LAB_0040c600;
  puStack_20 = (undefined1 *)*in_FS_OFFSET;
  *in_FS_OFFSET = &puStack_20;
  local_8 = 0xb;
  puVar3 = &local_c;
  uVar2 = 0;
  local_c = param_1;
  FUN_0041d2c4((undefined4 *)PTR_PTR_00430680,(int *)&local_10);
  uVar1 = FUN_00416cd0((int)&DAT_0040bcb0,'\x01',local_10,uVar2,puVar3);
  FUN_0041b508(uVar1);
  *in_FS_OFFSET = uVar2;
  puStack_20 = &LAB_0040c607;
  FUN_0041b840((int *)&local_10);
  return;
}



int FUN_0040c60c(int param_1)

{
  char cVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined4 *in_FS_OFFSET;
  undefined4 uVar6;
  undefined1 *puVar7;
  
  puVar2 = (undefined4 *)FUN_0040cdf0(DAT_00430850);
  puVar7 = &LAB_0040c6a9;
  uVar6 = *in_FS_OFFSET;
  *in_FS_OFFSET = &stack0xffffffdc;
  iVar5 = puVar2[2];
  if (-1 < iVar5 + -1) {
    iVar4 = 0;
    do {
      iVar3 = GetFieldDescriptor(puVar2,iVar4);
      cVar1 = FUN_0041adb4(iVar3,param_1);
      if (cVar1 != '\0') {
        FUN_0041b5a8(uVar6,(int)puVar7);
        return iVar3;
      }
      iVar4 = iVar4 + 1;
      iVar5 = iVar5 + -1;
    } while (iVar5 != 0);
  }
  iVar5 = (**(code **)(*DAT_00430854 + 0x50))(DAT_00430854,param_1);
  if (-1 < iVar5) {
    iVar5 = (**(code **)(*DAT_00430854 + 0x18))(DAT_00430854,iVar5);
    FUN_0041b5a8(uVar6,(int)puVar7);
    return iVar5;
  }
  *in_FS_OFFSET = uVar6;
  iVar5 = FUN_0040ce04(DAT_00430850);
  return iVar5;
}



void FUN_0040c6bc(undefined4 *param_1,char *param_2)

{
  short *psVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_12c;
  undefined1 *puStack_128;
  undefined1 *puStack_124;
  byte local_114 [256];
  char *local_14;
  undefined **local_10;
  int local_c;
  char *local_8;
  
  puStack_124 = &stack0xfffffffc;
  local_14 = (char *)0x0;
  puStack_128 = &LAB_0040c775;
  uStack_12c = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_12c;
  local_8 = param_2;
  for (local_10 = (undefined **)FUN_0041ad9c(param_1); local_10 != &PTR_LAB_0040cf35_3_0040bedc;
      local_10 = (undefined **)FUN_0041addc((int)local_10)) {
    psVar1 = (short *)FUN_0040c594((int)local_10);
    if ((psVar1 != (short *)0x0) && (iVar3 = (int)*psVar1, -1 < iVar3 + -1)) {
      local_c = 0;
      do {
        FUN_0041ada0(**(int **)(psVar1 + local_c * 2 + 1),local_114);
        FUN_0041ba64((int *)&local_14,local_114);
        uVar2 = FUN_004140f8(local_14,local_8);
        if ((char)uVar2 != '\0') goto LAB_0040c75f;
        local_c = local_c + 1;
        iVar3 = iVar3 + -1;
      } while (iVar3 != 0);
    }
  }
  FUN_0040c60c((int)local_8);
LAB_0040c75f:
  *in_FS_OFFSET = uStack_12c;
  puStack_124 = &LAB_0040c77c;
  puStack_128 = (undefined1 *)0x40c774;
  FUN_0041b840((int *)&local_14);
  return;
}



void FUN_0040c788(PVOID param_1)

{
  undefined1 *puVar1;
  int *piVar2;
  LPCVOID pvVar3;
  int iVar4;
  undefined4 *in_FS_OFFSET;
  _MEMORY_BASIC_INFORMATION *p_Var5;
  SIZE_T SVar6;
  undefined4 uStack_38;
  undefined1 *puStack_34;
  undefined1 *puStack_30;
  _MEMORY_BASIC_INFORMATION local_20;
  
  puStack_30 = (undefined1 *)0x40c79d;
  piVar2 = (int *)FUN_0040cdf0(DAT_00430850);
  puStack_34 = &LAB_0040c840;
  uStack_38 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_38;
  iVar4 = piVar2[2] + -1;
  puStack_30 = &stack0xfffffffc;
  puVar1 = &stack0xfffffffc;
  if (-1 < iVar4) {
    do {
      SVar6 = 0x1c;
      p_Var5 = &local_20;
      pvVar3 = (LPCVOID)GetFieldDescriptor(piVar2,iVar4);
      VirtualQuery(pvVar3,p_Var5,SVar6);
      if ((param_1 == (PVOID)0x0) || (param_1 == local_20.AllocationBase)) {
        FUN_0040c940(piVar2,iVar4);
      }
      iVar4 = iVar4 + -1;
      puVar1 = puStack_30;
    } while (iVar4 != -1);
  }
  puStack_30 = puVar1;
  iVar4 = (**(code **)(*DAT_00430854 + 0x14))();
  iVar4 = iVar4 + -1;
  if (-1 < iVar4) {
    do {
      SVar6 = 0x1c;
      p_Var5 = &local_20;
      pvVar3 = (LPCVOID)(**(code **)(*DAT_00430854 + 0x18))(DAT_00430854,iVar4);
      VirtualQuery(pvVar3,p_Var5,SVar6);
      if ((param_1 == (PVOID)0x0) || (param_1 == local_20.AllocationBase)) {
        (**(code **)(*DAT_00430854 + 0x44))(DAT_00430854,iVar4);
      }
      iVar4 = iVar4 + -1;
    } while (iVar4 != -1);
  }
  *in_FS_OFFSET = uStack_38;
  puStack_30 = &LAB_0040c847;
  puStack_34 = (undefined1 *)0x40c83f;
  FUN_0040ce04(DAT_00430850);
  return;
}



undefined4 FUN_0040c850(int param_1)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  undefined4 *in_FS_OFFSET;
  undefined1 *puVar7;
  
  puVar2 = (undefined4 *)FUN_0040cdf0(DAT_00430858);
  puVar7 = &LAB_0040c8c0;
  uVar4 = *in_FS_OFFSET;
  *in_FS_OFFSET = &stack0xffffffdc;
  iVar5 = puVar2[2];
  if (-1 < iVar5 + -1) {
    iVar6 = 0;
    do {
      iVar3 = GetFieldDescriptor(puVar2,iVar6);
      if (*(int *)(iVar3 + 4) == param_1) {
        uVar1 = *(undefined4 *)(iVar3 + 8);
        FUN_0041b5a8(uVar4,(int)puVar7);
        return uVar1;
      }
      iVar6 = iVar6 + 1;
      iVar5 = iVar5 + -1;
    } while (iVar5 != 0);
  }
  *in_FS_OFFSET = uVar4;
  uVar4 = FUN_0040ce04(DAT_00430858);
  return uVar4;
}



int FUN_0040c8f4(int *param_1,int param_2)

{
  int iVar1;
  
  iVar1 = param_1[2];
  if (iVar1 == param_1[3]) {
    (**(code **)*param_1)();
  }
  *(int *)(param_1[1] + iVar1 * 4) = param_2;
  param_1[2] = param_1[2] + 1;
  if (param_2 != 0) {
    (**(code **)(*param_1 + 4))(param_1,param_2,0);
  }
  return iVar1;
}



void FUN_0040c940(int *param_1,int param_2)

{
  int iVar1;
  
  if ((param_2 < 0) || (param_1[2] <= param_2)) {
    FUN_0040c9dc(*param_1,(undefined4 *)PTR_PTR_00430684,param_2);
  }
  iVar1 = GetFieldDescriptor(param_1,param_2);
  param_1[2] = param_1[2] + -1;
  if (param_2 < param_1[2]) {
    FUN_0041aae4((undefined4 *)(param_1[1] + 4 + param_2 * 4),
                 (undefined4 *)(param_1[1] + param_2 * 4),(param_1[2] - param_2) * 4);
  }
  if (iVar1 != 0) {
    (**(code **)(*param_1 + 4))(param_1,iVar1,2);
  }
  return;
}



undefined4 __stdcall FUN_0040c9a0(void)

{
  int unaff_EBP;
  
  return *(undefined4 *)(unaff_EBP + 4);
}



void FUN_0040c9a4(undefined4 param_1,byte *param_2,undefined4 param_3)

{
  undefined4 uVar1;
  undefined4 local_10;
  undefined1 local_c;
  
  FUN_0040c9a0();
  local_c = 0;
  local_10 = param_3;
  uVar1 = FUN_00416cd0((int)&DAT_0040bd0c,'\x01',param_2,0,&local_10);
  FUN_0041b508(uVar1);
  return;
}



void FUN_0040c9dc(undefined4 param_1,undefined4 *param_2,undefined4 param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_1c;
  undefined1 *puStack_18;
  undefined1 *puStack_14;
  byte *local_8;
  
  puStack_14 = &stack0xfffffffc;
  local_8 = (byte *)0x0;
  puStack_18 = &LAB_0040ca24;
  uStack_1c = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_1c;
  FUN_0041d2c4(param_2,(int *)&local_8);
  FUN_0040c9a4(&PTR_LAB_0040be20,local_8,param_3);
  *in_FS_OFFSET = uStack_1c;
  puStack_14 = &LAB_0040ca2b;
  puStack_18 = (undefined1 *)0x40ca23;
  FUN_0041b840((int *)&local_8);
  return;
}



undefined4 GetFieldDescriptor(undefined4 *param_1,int param_2)

{
  if ((param_2 < 0) || ((int)param_1[2] <= param_2)) {
    FUN_0040c9dc(*param_1,(undefined4 *)PTR_PTR_00430684,param_2);
  }
  return *(undefined4 *)(param_1[1] + param_2 * 4);
}



int FUN_0040ca8c(int param_1,int param_2)

{
  int iVar1;
  
  for (iVar1 = 0;
      (iVar1 < *(int *)(param_1 + 8) && (param_2 != *(int *)(*(int *)(param_1 + 4) + iVar1 * 4)));
      iVar1 = iVar1 + 1) {
  }
  if (iVar1 == *(int *)(param_1 + 8)) {
    iVar1 = -1;
  }
  return iVar1;
}



void FUN_0040caac(int *param_1,int param_2,int param_3)

{
  if ((param_2 < 0) || (param_1[2] < param_2)) {
    FUN_0040c9dc(*param_1,(undefined4 *)PTR_PTR_00430684,param_2);
  }
  if (param_1[2] == param_1[3]) {
    (**(code **)*param_1)();
  }
  if (param_2 < param_1[2]) {
    FUN_0041aae4((undefined4 *)(param_1[1] + param_2 * 4),
                 (undefined4 *)(param_1[1] + 4 + param_2 * 4),(param_1[2] - param_2) * 4);
  }
  *(int *)(param_1[1] + param_2 * 4) = param_3;
  param_1[2] = param_1[2] + 1;
  if (param_3 != 0) {
    (**(code **)(*param_1 + 4))(param_1,param_3,0);
  }
  return;
}



void FUN_0040cb18(undefined4 *param_1)

{
  GetFieldDescriptor(param_1,param_1[2] + -1);
  return;
}



void FUN_0040cb24(int *param_1,int param_2,int param_3)

{
  undefined4 uVar1;
  
  if (param_3 != param_2) {
    if ((param_3 < 0) || (param_1[2] <= param_3)) {
      FUN_0040c9dc(*param_1,(undefined4 *)PTR_PTR_00430684,param_3);
    }
    uVar1 = GetFieldDescriptor(param_1,param_2);
    *(undefined4 *)(param_1[1] + param_2 * 4) = 0;
    FUN_0040c940(param_1,param_2);
    FUN_0040caac(param_1,param_3,0);
    *(undefined4 *)(param_1[1] + param_3 * 4) = uVar1;
  }
  return;
}



void FUN_0040cb7c(int *param_1,int param_2,int param_3)

{
  int iVar1;
  
  if ((param_2 < 0) || (param_1[2] <= param_2)) {
    FUN_0040c9dc(*param_1,(undefined4 *)PTR_PTR_00430684,param_2);
  }
  iVar1 = *(int *)(param_1[1] + param_2 * 4);
  *(int *)(param_1[1] + param_2 * 4) = param_3;
  if (iVar1 != 0) {
    (**(code **)(*param_1 + 4))(param_1,iVar1,2);
  }
  if (param_3 != 0) {
    (**(code **)(*param_1 + 4))(param_1,param_3,0);
  }
  return;
}



int FUN_0040cbc8(int *param_1,int param_2)

{
  int iVar1;
  
  iVar1 = FUN_0040ca8c((int)param_1,param_2);
  if (-1 < iVar1) {
    FUN_0040c940(param_1,iVar1);
  }
  return iVar1;
}



void FUN_0040cbe8(undefined4 *param_1,int param_2)

{
  if ((param_2 < (int)param_1[2]) || (0x7ffffff < param_2)) {
    FUN_0040c9dc(*param_1,(undefined4 *)PTR_PTR_00430688,param_2);
  }
  if (param_2 != param_1[3]) {
    FUN_0041a9f0(param_1 + 1,param_2 << 2);
    param_1[3] = param_2;
  }
  return;
}



void FUN_0040cc24(int *param_1,int param_2)

{
  int iVar1;
  int iVar2;
  
  if ((param_2 < 0) || (0x7ffffff < param_2)) {
    FUN_0040c9dc(*param_1,(undefined4 *)PTR_PTR_0043068c,param_2);
  }
  if (param_1[3] < param_2) {
    FUN_0040cbe8(param_1,param_2);
  }
  iVar1 = param_1[2];
  if (iVar1 < param_2) {
    FUN_0041abb0((undefined4 *)(param_1[1] + iVar1 * 4),(param_2 - iVar1) * 4,0);
  }
  else {
    iVar1 = iVar1 + -1;
    if (param_2 - iVar1 == 0 || param_2 < iVar1) {
      iVar2 = (param_2 - iVar1) + -1;
      do {
        FUN_0040c940(param_1,iVar1);
        iVar1 = iVar1 + -1;
        iVar2 = iVar2 + 1;
      } while (iVar2 != 0);
    }
  }
  param_1[2] = param_2;
  return;
}



void FUN_0040ccec(int *param_1,char param_2)

{
  int *piVar1;
  byte extraout_DL;
  undefined4 *in_FS_OFFSET;
  undefined4 uStackY_18;
  
  piVar1 = FUN_0041b130(param_1,param_2);
  FUN_0040cdf0((int)piVar1);
  uStackY_18 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStackY_18;
  FUN_0041ae54((int *)piVar1[1]);
  FUN_0041ae44(piVar1,extraout_DL & 0xfc);
  *in_FS_OFFSET = uStackY_18;
  FUN_0040ce04((int)piVar1);
  uStackY_18 = 0x40cd4d;
  DeleteCriticalSection((LPCRITICAL_SECTION)(piVar1 + 2));
  return;
}



void FUN_0040cd68(int param_1,int param_2)

{
  int iVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_18;
  undefined1 *puStack_14;
  undefined1 *puStack_10;
  
  puStack_10 = (undefined1 *)0x40cd7a;
  FUN_0040cdf0(param_1);
  puStack_14 = &LAB_0040cde4;
  uStack_18 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_18;
  puStack_10 = &stack0xfffffffc;
  if (*(char *)(param_1 + 0x20) != '\x01') {
    puStack_10 = &stack0xfffffffc;
    iVar1 = FUN_0040ca8c(*(int *)(param_1 + 4),param_2);
    if (iVar1 != -1) {
      if (*(char *)(param_1 + 0x20) == '\x02') {
        FUN_0040c9dc(**(undefined4 **)(param_1 + 4),(undefined4 *)PTR_LAB_004306b8,param_2);
      }
      goto LAB_0040cdce;
    }
  }
  FUN_0040c8f4(*(int **)(param_1 + 4),param_2);
LAB_0040cdce:
  *in_FS_OFFSET = uStack_18;
  puStack_10 = &LAB_0040cdeb;
  puStack_14 = (undefined1 *)0x40cde3;
  FUN_0040ce04(param_1);
  return;
}



undefined4 FUN_0040cdf0(int param_1)

{
  EnterCriticalSection((LPCRITICAL_SECTION)(param_1 + 8));
  return *(undefined4 *)(param_1 + 4);
}



void FUN_0040ce04(int param_1)

{
  LeaveCriticalSection((LPCRITICAL_SECTION)(param_1 + 8));
  return;
}



void FUN_0040ce28(int *param_1,char param_2)

{
  int *piVar1;
  byte extraout_DL;
  
  piVar1 = FUN_0041b130(param_1,param_2);
  FUN_0040e958((int)piVar1);
  FUN_0041ae44(piVar1,extraout_DL & 0xfc);
  if ('\0' < (char)extraout_DL) {
    FUN_0041b120(piVar1);
  }
  return;
}



void FUN_0040ce54(int *param_1,undefined4 *param_2)

{
  if (param_2 != (undefined4 *)0x0) {
    (**(code **)*param_2)(param_2,param_1);
    return;
  }
  FUN_0040ce68(param_1,(int *)0x0);
  return;
}



void FUN_0040ce68(int *param_1,int *param_2)

{
  undefined4 uVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 uVar2;
  undefined1 *puStack_12c;
  undefined1 *puStack_128;
  undefined1 *puStack_124;
  int local_118;
  undefined1 local_114;
  byte *local_110;
  undefined1 local_10c;
  byte local_108 [256];
  int local_8;
  
  puStack_124 = &stack0xfffffffc;
  local_8 = 0;
  puStack_128 = &LAB_0040cf1e;
  puStack_12c = (undefined1 *)*in_FS_OFFSET;
  *in_FS_OFFSET = &puStack_12c;
  if (param_2 == (int *)0x0) {
    FUN_0041b8d8(&local_8,0x40cf34);
  }
  else {
    puStack_124 = &stack0xfffffffc;
    FUN_0041ada0(*param_2,local_108);
    FUN_0041ba64(&local_8,local_108);
  }
  local_118 = local_8;
  local_114 = 0xb;
  FUN_0041ada0(*param_1,local_108);
  local_110 = local_108;
  local_10c = 4;
  uVar2 = 1;
  uVar1 = FUN_00416d8c((int)&DAT_00413bb4,'\x01',(undefined4 *)PTR_PTR_0043066c,1,&local_118);
  FUN_0041b508(uVar1);
  *in_FS_OFFSET = uVar2;
  puStack_12c = &LAB_0040cf25;
  FUN_0041b840(&local_8);
  return;
}



void FUN_0040cf44(int *param_1,int *param_2)

{
  int iVar1;
  int *piVar2;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  undefined4 extraout_ECX_01;
  undefined4 extraout_EDX;
  undefined4 extraout_EDX_00;
  int *in_FS_OFFSET;
  int iStack_12c;
  int iStack_120;
  undefined1 *puStack_11c;
  undefined1 *puStack_118;
  byte local_108 [256];
  int local_8;
  
  puStack_118 = &stack0xfffffffc;
  local_8 = 0;
  puStack_11c = &LAB_0040cfd9;
  iStack_120 = *in_FS_OFFSET;
  *in_FS_OFFSET = (int)&iStack_120;
  FUN_0041ada0(*param_1,local_108);
  FUN_0041ba64(param_2,local_108);
  iVar1 = FUN_0041b014(param_1,extraout_EDX,extraout_ECX);
  if (iVar1 != 0) {
    piVar2 = (int *)FUN_0041b014(param_1,extraout_EDX_00,extraout_ECX_00);
    FUN_0041b014(piVar2,&local_8,extraout_ECX_01);
    if (local_8 != 0) {
      iStack_12c = *param_2;
      FUN_0041bb80(param_2,3);
    }
  }
  *in_FS_OFFSET = iStack_12c;
  FUN_0041b840(&local_8);
  return;
}



void FUN_0040d060(int param_1,char param_2)

{
  int *piVar1;
  
  piVar1 = *(int **)(param_1 + 4);
  if ((piVar1 != (int *)0x0) && (piVar1[3] == 0)) {
    if (param_2 != '\0') {
      param_1 = 0;
    }
    (**(code **)(*piVar1 + 0x10))(piVar1,param_1);
  }
  return;
}



int FUN_0040d084(int param_1)

{
  int iVar1;
  
  if (*(int *)(param_1 + 4) != 0) {
    iVar1 = FUN_0040ca8c(*(int *)(*(int *)(param_1 + 4) + 8),param_1);
    return iVar1;
  }
  return -1;
}



void FUN_0040d0c4(int *param_1,int *param_2)

{
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_130;
  undefined1 *puStack_12c;
  undefined1 *puStack_128;
  byte local_118 [256];
  int local_18;
  int local_14;
  undefined1 local_10;
  int local_c;
  undefined1 local_8;
  
  puStack_128 = &stack0xfffffffc;
  local_18 = 0;
  puStack_12c = &LAB_0040d159;
  uStack_130 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_130;
  if (param_1[1] == 0) {
    FUN_0041ada0(*param_1,local_118);
    FUN_0041ba64(param_2,local_118);
  }
  else {
    puStack_128 = &stack0xfffffffc;
    FUN_0041b014((int *)param_1[1],&local_18,0);
    local_14 = local_18;
    local_10 = 0xb;
    local_c = FUN_0040d084((int)param_1);
    local_8 = 0;
    FUN_00414ac0((byte *)"%s[%d]",&local_14,1,param_2);
  }
  *in_FS_OFFSET = uStack_130;
  puStack_128 = &LAB_0040d160;
  puStack_12c = (undefined1 *)0x40d158;
  FUN_0041b840(&local_18);
  return;
}



void FUN_0040d17c(int *param_1,int *param_2,undefined4 param_3)

{
  int *piVar1;
  
  piVar1 = (int *)param_1[1];
  if (param_2 != piVar1) {
    if (piVar1 != (int *)0x0) {
      FUN_0040d3b8(piVar1,(int)param_1,param_3);
    }
    if (param_2 != (int *)0x0) {
      FUN_0040d35c(param_2,param_1);
    }
  }
  return;
}



void FUN_0040d1ac(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = FUN_0040d084(param_1);
  if ((-1 < iVar1) && (param_2 != iVar1)) {
    FUN_0040cb24(*(int **)(*(int *)(param_1 + 4) + 8),iVar1,param_2);
    FUN_0040d060(param_1,'\x01');
  }
  return;
}



void FUN_0040d1dc(int param_1)

{
  (**(code **)(*(int *)(param_1 + 4) + 0x18))(*(int *)(param_1 + 4),1,param_1);
  return;
}



void FUN_0040d1e8(int *param_1)

{
  if (param_1[3] == 0) {
    (**(code **)(*param_1 + 0x10))(param_1,0);
  }
  return;
}



void FUN_0040d1f8(int *param_1)

{
  undefined1 *puVar1;
  int *piVar2;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_18;
  undefined1 *puStack_14;
  undefined1 *puStack_10;
  
  if (0 < *(int *)(param_1[2] + 8)) {
    puStack_10 = (undefined1 *)0x40d214;
    (**(code **)(*param_1 + 0x14))();
    puStack_14 = &LAB_0040d252;
    uStack_18 = *in_FS_OFFSET;
    *in_FS_OFFSET = &uStack_18;
    puStack_10 = &stack0xfffffffc;
    while (puVar1 = puStack_10, 0 < (int)((undefined4 *)param_1[2])[2]) {
      piVar2 = (int *)FUN_0040cb18((undefined4 *)param_1[2]);
      FUN_0041ae54(piVar2);
    }
    *in_FS_OFFSET = uStack_18;
    puStack_10 = (undefined1 *)0x40d259;
    puStack_14 = (undefined1 *)0x40d251;
    (**(code **)(*param_1 + 0x18))(param_1,*param_1,puVar1);
    return;
  }
  return;
}



void FUN_0040d260(int *param_1,int *param_2)

{
  int *piVar1;
  undefined4 uVar2;
  int *piVar3;
  undefined4 extraout_ECX;
  undefined4 extraout_EDX;
  
  if (param_2 != (int *)0x0) {
    *param_2 = 0;
  }
  FUN_0041d31c(param_2);
  if (param_1 != (int *)0x0) {
    piVar1 = (int *)FUN_0041b014(param_1,extraout_EDX,extraout_ECX);
    if (piVar1 == (int *)0x0) {
      uVar2 = FUN_0041afcc(param_1,(int)&PTR_LAB_0040cf35_3_0040c358);
      if (((char)uVar2 != '\0') && ((*(byte *)(param_1 + 8) & 0x10) != 0)) {
        FUN_0041d31c(param_2);
        (**(code **)(*param_1 + 0x28))();
      }
    }
    else {
      uVar2 = FUN_0041afcc(param_1,(int)&PTR_LAB_0040cf35_3_0040c358);
      if (((char)uVar2 == '\0') || ((*(byte *)(param_1 + 8) & 0x10) != 0)) {
        piVar3 = FUN_0041d31c(param_2);
        FUN_0040d260(piVar1,piVar3);
      }
    }
  }
  return;
}



void FUN_0040d2f8(int *param_1,undefined4 param_2,undefined4 param_3)

{
  int *piVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_20;
  undefined1 *puStack_1c;
  undefined1 *puStack_18;
  int *local_8;
  
  puStack_18 = &stack0xfffffffc;
  local_8 = (int *)0x0;
  puStack_1c = &LAB_0040d34d;
  uStack_20 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_20;
  piVar1 = FUN_0041d31c((int *)&local_8);
  FUN_0040d260(param_1,piVar1);
  if (local_8 != (int *)0x0) {
    (**(code **)(*local_8 + 0x10))(local_8,param_2,param_3);
  }
  *in_FS_OFFSET = uStack_20;
  puStack_18 = &LAB_0040d354;
  puStack_1c = (undefined1 *)0x40d34c;
  FUN_0041d31c((int *)&local_8);
  return;
}



void FUN_0040d35c(int *param_1,int *param_2)

{
  undefined4 uVar1;
  
  uVar1 = FUN_0041afcc(param_2,param_1[1]);
  if ((char)uVar1 == '\0') {
    FUN_0040c9dc(&PTR_LAB_0040be20,(undefined4 *)PTR_PTR_004306b4,0);
  }
  FUN_0040c8f4((int *)param_1[2],(int)param_2);
  param_2[1] = (int)param_1;
  param_2[2] = param_1[4];
  param_1[4] = param_1[4] + 1;
  (**(code **)(*param_1 + 0xc))(param_1,param_2);
  FUN_0040d1e8(param_1);
  FUN_0040d2f8(param_1,param_2,0);
  return;
}



void FUN_0040d3b8(int *param_1,int param_2,undefined4 param_3)

{
  FUN_0040d2f8(param_1,param_2,CONCAT31((int3)((uint)param_3 >> 8),1));
  FUN_0040cbc8((int *)param_1[2],param_2);
  *(undefined4 *)(param_2 + 4) = 0;
  FUN_0040d1e8(param_1);
  return;
}



void FUN_0040d3e4(int *param_1,char param_2)

{
  int *piVar1;
  byte extraout_DL;
  
  piVar1 = FUN_0041b130(param_1,param_2);
  FUN_0040ddbc((int)piVar1,(int *)0x0);
  FUN_0040ce28(piVar1,extraout_DL & 0xfc);
  if ('\0' < (char)extraout_DL) {
    FUN_0041b120(piVar1);
  }
  return;
}



undefined4 FUN_0040d414(int *param_1,undefined4 param_2)

{
  undefined4 uVar1;
  
  uVar1 = (**(code **)(*param_1 + 0x14))();
  (**(code **)(*param_1 + 0x54))(param_1,uVar1,param_2);
  return uVar1;
}



undefined4 FUN_0040d438(int *param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  
  uVar1 = (**(code **)(*param_1 + 0x34))();
  (**(code **)(*param_1 + 0x24))(param_1,uVar1,param_3);
  return uVar1;
}



void FUN_0040d45c(int *param_1,int *param_2)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_34;
  undefined1 *puStack_30;
  undefined1 *puStack_2c;
  undefined4 uStack_28;
  undefined1 *puStack_24;
  undefined1 *puStack_20;
  undefined4 local_10;
  int *local_c;
  int *local_8;
  
  puStack_20 = &stack0xfffffffc;
  local_10 = 0;
  puStack_24 = &LAB_0040d503;
  uStack_28 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_28;
  puStack_2c = (undefined1 *)0x40d486;
  local_c = param_2;
  local_8 = param_1;
  FUN_0040d584(param_1);
  puStack_30 = &LAB_0040d4e6;
  uStack_34 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_34;
  puStack_2c = &stack0xfffffffc;
  iVar2 = (**(code **)(*local_c + 0x14))();
  if (-1 < iVar2 + -1) {
    iVar4 = 0;
    do {
      (**(code **)(*local_c + 0xc))(local_c,iVar4,&local_10);
      uVar1 = local_10;
      uVar3 = (**(code **)(*local_c + 0x18))(local_c,iVar4);
      (**(code **)(*local_8 + 0x38))(local_8,uVar1,uVar3);
      iVar4 = iVar4 + 1;
      iVar2 = iVar2 + -1;
    } while (iVar2 != 0);
  }
  *in_FS_OFFSET = uStack_34;
  puStack_2c = &LAB_0040d4ed;
  puStack_30 = (undefined1 *)0x40d4e5;
  FUN_0040d640(local_8);
  return;
}



void FUN_0040d514(int *param_1,int *param_2)

{
  undefined4 uVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_18;
  undefined1 *puStack_14;
  undefined1 *puStack_10;
  
  puStack_10 = (undefined1 *)0x40d52b;
  uVar1 = FUN_0041afcc(param_2,(int)&PTR_LAB_0040cf35_3_0040c00c);
  if ((char)uVar1 != '\0') {
    puStack_10 = (undefined1 *)0x40d537;
    FUN_0040d584(param_1);
    puStack_14 = &LAB_0040d56d;
    uStack_18 = *in_FS_OFFSET;
    *in_FS_OFFSET = &uStack_18;
    puStack_10 = &stack0xfffffffc;
    (**(code **)(*param_1 + 0x40))();
    (**(code **)(*param_1 + 0x3c))(param_1,param_2);
    *in_FS_OFFSET = uStack_18;
    puStack_10 = (undefined1 *)0x40d57e;
    puStack_14 = (undefined1 *)0x40d56c;
    FUN_0040d640(param_1);
    return;
  }
  puStack_10 = (undefined1 *)0x40d57e;
  FUN_0040ce54(param_1,param_2);
  return;
}



void FUN_0040d584(int *param_1)

{
  if (param_1[1] == 0) {
    (**(code **)(*param_1 + 0x30))(param_1,1);
  }
  param_1[1] = param_1[1] + 1;
  return;
}



byte __cdecl FUN_0040d59c(int param_1)

{
  int *piVar1;
  byte bVar2;
  undefined4 uVar3;
  int iVar4;
  
  piVar1 = *(int **)(*(int *)(param_1 + -4) + 0x20);
  if (piVar1 == (int *)0x0) {
    iVar4 = (**(code **)(**(int **)(param_1 + -8) + 0x14))();
    bVar2 = 0 < iVar4;
  }
  else {
    bVar2 = 1;
    uVar3 = FUN_0041afcc(piVar1,(int)&PTR_LAB_0040cf35_3_0040c00c);
    if ((char)uVar3 != '\0') {
      bVar2 = FUN_0040d654(*(int **)(param_1 + -8),piVar1);
      bVar2 = bVar2 ^ 1;
    }
  }
  return bVar2;
}



void FUN_0040d5f4(undefined4 param_1,int *param_2)

{
  byte bVar1;
  code *pcVar2;
  code *pcVar3;
  undefined4 uVar4;
  
  pcVar3 = FUN_0040dc54;
  pcVar2 = FUN_0040df04;
  uVar4 = param_1;
  bVar1 = FUN_0040d59c((int)&stack0xfffffffc);
  (**(code **)(*param_2 + 4))(param_2,"Strings",bVar1,pcVar2,param_1,pcVar3,uVar4);
  return;
}



void FUN_0040d640(int *param_1)

{
  param_1[1] = param_1[1] + -1;
  if (param_1[1] == 0) {
    (**(code **)(*param_1 + 0x30))(param_1,0);
  }
  return;
}



void FUN_0040d654(int *param_1,int *param_2)

{
  uint *puVar1;
  int iVar2;
  int iVar3;
  undefined4 *in_FS_OFFSET;
  undefined1 uVar4;
  undefined4 uStack_30;
  undefined1 *puStack_2c;
  undefined1 *puStack_28;
  uint *local_18;
  uint *local_14;
  undefined1 local_d;
  int *local_c;
  int *local_8;
  
  puStack_28 = &stack0xfffffffc;
  local_14 = (uint *)0x0;
  local_18 = (uint *)0x0;
  puStack_2c = &LAB_0040d6e9;
  uStack_30 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_30;
  local_d = 0;
  local_c = param_2;
  local_8 = param_1;
  iVar2 = (**(code **)(*param_1 + 0x14))();
  iVar3 = (**(code **)(*local_c + 0x14))();
  if (iVar2 == iVar3) {
    if (-1 < iVar2 + -1) {
      iVar3 = 0;
      uVar4 = true;
      do {
        (**(code **)(*local_8 + 0xc))(local_8,iVar3,&local_14);
        puVar1 = local_14;
        (**(code **)(*local_c + 0xc))(local_c,iVar3,&local_18);
        FUN_0041bbd0(puVar1,local_18);
        if (!(bool)uVar4) goto LAB_0040d6ce;
        iVar3 = iVar3 + 1;
        iVar2 = iVar2 + -1;
        uVar4 = iVar2 == 0;
      } while (!(bool)uVar4);
    }
    local_d = 1;
  }
LAB_0040d6ce:
  *in_FS_OFFSET = uStack_30;
  puStack_28 = &LAB_0040d6f0;
  puStack_2c = (undefined1 *)0x40d6e8;
  FUN_0041b864((int *)&local_18,2);
  return;
}



undefined4 __stdcall FUN_0040d6fc(void)

{
  int unaff_EBP;
  
  return *(undefined4 *)(unaff_EBP + 4);
}



void FUN_0040d700(undefined4 param_1,byte *param_2,undefined4 param_3)

{
  undefined4 uVar1;
  undefined4 local_10;
  undefined1 local_c;
  
  FUN_0040d6fc();
  local_c = 0;
  local_10 = param_3;
  uVar1 = FUN_00416cd0((int)&DAT_0040bd64,'\x01',param_2,0,&local_10);
  FUN_0041b508(uVar1);
  return;
}



void FUN_0040d738(undefined4 param_1,undefined4 *param_2,undefined4 param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_20;
  undefined1 *puStack_1c;
  undefined1 *puStack_18;
  byte *local_8;
  
  puStack_18 = &stack0xfffffffc;
  local_8 = (byte *)0x0;
  puStack_1c = &LAB_0040d780;
  uStack_20 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_20;
  FUN_0041d2c4(param_2,(int *)&local_8);
  FUN_0040d700(param_1,local_8,param_3);
  *in_FS_OFFSET = uStack_20;
  puStack_18 = &LAB_0040d787;
  puStack_1c = (undefined1 *)0x40d77f;
  FUN_0041b840((int *)&local_8);
  return;
}



void FUN_0040d790(int *param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_38;
  undefined1 *puStack_34;
  undefined1 *puStack_30;
  undefined4 uStack_2c;
  undefined1 *puStack_28;
  undefined1 *puStack_24;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  int *local_8;
  
  puStack_24 = &stack0xfffffffc;
  local_14 = 0;
  local_10 = 0;
  puStack_28 = &LAB_0040d868;
  uStack_2c = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_2c;
  puStack_30 = (undefined1 *)0x40d7be;
  local_8 = param_1;
  FUN_0040d584(param_1);
  puStack_34 = &LAB_0040d846;
  uStack_38 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_38;
  puStack_30 = &stack0xfffffffc;
  (**(code **)(*local_8 + 0xc))(local_8,param_2,&local_10);
  local_c = (**(code **)(*local_8 + 0x18))(local_8,param_2);
  (**(code **)(*local_8 + 0xc))(local_8,param_3,&local_14);
  (**(code **)(*local_8 + 0x20))(local_8,param_2,local_14);
  uVar1 = (**(code **)(*local_8 + 0x18))(local_8,param_3);
  (**(code **)(*local_8 + 0x24))(local_8,param_2,uVar1);
  (**(code **)(*local_8 + 0x20))(local_8,param_3,local_10);
  (**(code **)(*local_8 + 0x24))(local_8,param_3,local_c);
  *in_FS_OFFSET = uStack_38;
  puStack_30 = &LAB_0040d84d;
  puStack_34 = (undefined1 *)0x40d845;
  FUN_0040d640(local_8);
  return;
}



void FUN_0040d884(int *param_1)

{
  char *pcVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_18;
  undefined1 *puStack_14;
  undefined1 *puStack_10;
  undefined *local_8;
  
  puStack_10 = &stack0xfffffffc;
  local_8 = (undefined *)0x0;
  puStack_14 = &LAB_0040d8c9;
  uStack_18 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_18;
  (**(code **)(*param_1 + 0x1c))(param_1,&local_8);
  pcVar1 = FUN_0041bc74(local_8);
  FUN_004145e0(pcVar1);
  *in_FS_OFFSET = uStack_18;
  puStack_10 = &LAB_0040d8d0;
  puStack_14 = (undefined1 *)0x40d8c8;
  FUN_0041b840((int *)&local_8);
  return;
}



void FUN_0040d8d8(int *param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  undefined4 *puVar4;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_34;
  undefined1 *puStack_30;
  undefined1 *puStack_2c;
  int local_1c;
  undefined4 *local_18;
  int local_14;
  int local_10;
  int *local_c;
  int *local_8;
  
  puStack_2c = &stack0xfffffffc;
  local_1c = 0;
  local_18 = (undefined4 *)0x0;
  puStack_30 = &LAB_0040d9af;
  uStack_34 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_34;
  local_c = param_2;
  local_8 = param_1;
  iVar1 = (**(code **)(*param_1 + 0x14))();
  uVar3 = 0;
  local_14 = iVar1;
  if (-1 < iVar1 + -1) {
    local_10 = 0;
    do {
      (**(code **)(*local_8 + 0xc))(local_8,local_10,&local_1c);
      iVar2 = FUN_0041bac0(local_1c);
      uVar3 = uVar3 + iVar2 + 2;
      local_10 = local_10 + 1;
      iVar1 = iVar1 + -1;
    } while (iVar1 != 0);
  }
  FUN_0041b928(local_c,(undefined4 *)0x0,uVar3);
  puVar4 = (undefined4 *)*local_c;
  if (-1 < local_14 + -1) {
    local_10 = 0;
    iVar1 = local_14;
    do {
      (**(code **)(*local_8 + 0xc))(local_8,local_10,&local_18);
      uVar3 = FUN_0041bac0((int)local_18);
      if (uVar3 != 0) {
        FUN_0041aae4(local_18,puVar4,uVar3);
        puVar4 = (undefined4 *)((int)puVar4 + uVar3);
      }
      *(undefined1 *)puVar4 = 0xd;
      *(undefined1 *)((int)puVar4 + 1) = 10;
      puVar4 = (undefined4 *)((int)puVar4 + 2);
      local_10 = local_10 + 1;
      iVar1 = iVar1 + -1;
    } while (iVar1 != 0);
  }
  *in_FS_OFFSET = uStack_34;
  puStack_2c = &LAB_0040d9b6;
  puStack_30 = (undefined1 *)0x40d9ae;
  FUN_0041b864(&local_1c,2);
  return;
}



void FUN_0040d9c0(int *param_1,undefined *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_28;
  undefined1 *puStack_24;
  undefined1 *puStack_20;
  undefined *local_10;
  undefined *local_c;
  int *local_8;
  
  puStack_20 = &stack0xfffffffc;
  local_10 = (undefined *)0x0;
  puStack_24 = &LAB_0040da2d;
  uStack_28 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_28;
  local_c = param_2;
  local_8 = param_1;
  iVar1 = (**(code **)(*param_1 + 0x14))();
  if (-1 < iVar1 + -1) {
    iVar3 = 0;
    do {
      (**(code **)(*local_8 + 0xc))(local_8,iVar3,&local_10);
      iVar2 = FUN_0041411c(local_10,local_c);
      if (iVar2 == 0) break;
      iVar3 = iVar3 + 1;
      iVar1 = iVar1 + -1;
    } while (iVar1 != 0);
  }
  *in_FS_OFFSET = uStack_28;
  puStack_20 = &LAB_0040da34;
  puStack_24 = (undefined1 *)0x40da2c;
  FUN_0041b840((int *)&local_10);
  return;
}



void FUN_0040da40(int *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  (**(code **)(*param_1 + 0x54))(param_1,param_2,param_3);
  (**(code **)(*param_1 + 0x24))(param_1,param_2,param_4);
  return;
}



void FUN_0040da70(int *param_1,undefined *param_2)

{
  int *piVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 uStackY_18;
  undefined1 *puStackY_14;
  
  puStackY_14 = (undefined1 *)0x40da87;
  piVar1 = FUN_0040e5f0((int *)&LAB_0040c2dc,'\x01',param_2,0x20);
  puStackY_14 = &LAB_0040dab8;
  uStackY_18 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStackY_18;
  (**(code **)(*param_1 + 0x5c))(param_1,piVar1);
  *in_FS_OFFSET = uStackY_18;
  puStackY_14 = (undefined1 *)0x40dab7;
  FUN_0041ae54(piVar1);
  return;
}



void FUN_0040dac4(int *param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_2c;
  undefined1 *puStack_28;
  undefined1 *puStack_24;
  undefined4 uStack_20;
  undefined1 *puStack_1c;
  undefined1 *puStack_18;
  int local_c;
  int *local_8;
  
  puStack_18 = &stack0xfffffffc;
  local_c = 0;
  puStack_1c = &LAB_0040db62;
  uStack_20 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_20;
  puStack_24 = (undefined1 *)0x40daec;
  local_8 = param_1;
  FUN_0040d584(param_1);
  puStack_28 = &LAB_0040db45;
  uStack_2c = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_2c;
  puStack_24 = &stack0xfffffffc;
  iVar1 = FUN_0040e534(param_2);
  iVar2 = FUN_0040e524(param_2);
  FUN_0041b928(&local_c,(undefined4 *)0x0,iVar1 - iVar2);
  (**(code **)(*param_2 + 4))(param_2,local_c,iVar1 - iVar2);
  (**(code **)(*local_8 + 0x2c))(local_8,local_c);
  *in_FS_OFFSET = uStack_2c;
  puStack_24 = &LAB_0040db4c;
  puStack_28 = (undefined1 *)0x40db44;
  FUN_0040d640(local_8);
  return;
}



void FUN_0040db70(int *param_1,int param_2,int param_3)

{
  undefined4 uVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_30;
  undefined1 *puStack_2c;
  undefined1 *puStack_28;
  undefined4 uStack_24;
  undefined1 *puStack_20;
  undefined1 *puStack_1c;
  int local_c;
  int *local_8;
  
  puStack_1c = &stack0xfffffffc;
  local_c = 0;
  puStack_20 = &LAB_0040dc11;
  uStack_24 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_24;
  local_8 = param_1;
  if (param_3 != param_2) {
    puStack_28 = (undefined1 *)0x40db9f;
    FUN_0040d584(param_1);
    puStack_2c = &LAB_0040dbf4;
    uStack_30 = *in_FS_OFFSET;
    *in_FS_OFFSET = &uStack_30;
    puStack_28 = &stack0xfffffffc;
    (**(code **)(*local_8 + 0xc))(local_8,param_2,&local_c);
    uVar1 = (**(code **)(*local_8 + 0x18))(local_8,param_2);
    (**(code **)(*local_8 + 0x44))(local_8,param_2);
    FUN_0040da40(local_8,param_3,local_c,uVar1);
    *in_FS_OFFSET = uStack_30;
    puStack_28 = (undefined1 *)0x40dbfb;
    puStack_2c = (undefined1 *)0x40dbf3;
    FUN_0040d640(local_8);
    return;
  }
  *in_FS_OFFSET = uStack_24;
  puStack_1c = &LAB_0040dc18;
  puStack_20 = (undefined1 *)0x40dc10;
  FUN_0041b840(&local_c);
  return;
}



void FUN_0040dc20(int *param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  
  uVar1 = (**(code **)(*param_1 + 0x18))(param_1,param_2);
  (**(code **)(*param_1 + 0x44))(param_1,param_2);
  FUN_0040da40(param_1,param_2,param_3,uVar1);
  return;
}



void FUN_0040dc54(int *param_1,int param_2)

{
  bool bVar1;
  uint extraout_ECX;
  uint extraout_ECX_00;
  uint uVar2;
  uint extraout_ECX_01;
  undefined4 extraout_EDX;
  undefined4 extraout_EDX_00;
  undefined4 uVar3;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_28;
  undefined1 *puStack_24;
  undefined1 *puStack_20;
  undefined4 uStack_1c;
  undefined1 *puStack_18;
  undefined1 *puStack_14;
  int local_10;
  int local_c;
  int *local_8;
  
  puStack_14 = &stack0xfffffffc;
  local_10 = 0;
  puStack_18 = &LAB_0040dcf8;
  uStack_1c = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_1c;
  puStack_20 = (undefined1 *)0x40dc7b;
  local_c = param_2;
  local_8 = param_1;
  FUN_0040f6b4(param_2,param_2,0);
  puStack_20 = (undefined1 *)0x40dc83;
  FUN_0040d584(local_8);
  puStack_24 = &LAB_0040dcd3;
  uStack_28 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_28;
  puStack_20 = &stack0xfffffffc;
  (**(code **)(*local_8 + 0x40))();
  uVar2 = extraout_ECX;
  uVar3 = extraout_EDX;
  while( true ) {
    bVar1 = FUN_0040ea48(local_c,uVar3,uVar2);
    if (bVar1) break;
    FUN_0040ffc4(local_c,&local_10,extraout_ECX_01);
    (**(code **)(*local_8 + 0x34))(local_8,local_10);
    uVar2 = extraout_ECX_00;
    uVar3 = extraout_EDX_00;
  }
  *in_FS_OFFSET = uStack_28;
  puStack_20 = &LAB_0040dcda;
  puStack_24 = (undefined1 *)0x40dcd2;
  FUN_0040d640(local_8);
  return;
}



void FUN_0040dd04(int *param_1,undefined *param_2)

{
  int *piVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 uStackY_18;
  undefined1 *puStackY_14;
  
  puStackY_14 = (undefined1 *)0x40dd1e;
  piVar1 = FUN_0040e5f0((int *)&LAB_0040c2dc,'\x01',param_2,0xffff);
  puStackY_14 = &LAB_0040dd4f;
  uStackY_18 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStackY_18;
  (**(code **)(*param_1 + 0x68))(param_1,piVar1);
  *in_FS_OFFSET = uStackY_18;
  puStackY_14 = (undefined1 *)0x40dd4e;
  FUN_0041ae54(piVar1);
  return;
}



void FUN_0040dd5c(int *param_1,int *param_2)

{
  int iVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_1c;
  undefined1 *puStack_18;
  undefined1 *puStack_14;
  int local_8;
  
  puStack_14 = &stack0xfffffffc;
  local_8 = 0;
  puStack_18 = &LAB_0040dda9;
  uStack_1c = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_1c;
  (**(code **)(*param_1 + 0x1c))(param_1,&local_8);
  iVar1 = FUN_0041bac0(local_8);
  FUN_0040e56c(param_2,local_8,iVar1);
  *in_FS_OFFSET = uStack_1c;
  puStack_14 = &LAB_0040ddb0;
  puStack_18 = (undefined1 *)0x40dda8;
  FUN_0041b840(&local_8);
  return;
}



void FUN_0040ddbc(int param_1,int *param_2)

{
  if (*(int *)(param_1 + 8) != 0) {
    (**(code **)(**(int **)(param_1 + 8) + 0x10))();
  }
  FUN_0041d334((int *)(param_1 + 8),param_2);
  if (*(int *)(param_1 + 8) != 0) {
    (**(code **)(**(int **)(param_1 + 8) + 0xc))(*(int **)(param_1 + 8),param_1);
  }
  return;
}



void FUN_0040ddf0(int *param_1,char *param_2)

{
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_1c;
  undefined1 *puStack_18;
  undefined1 *puStack_14;
  int local_8;
  
  puStack_14 = &stack0xfffffffc;
  local_8 = 0;
  puStack_18 = &LAB_0040de33;
  uStack_1c = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_1c;
  FUN_0041b9f8(&local_8,param_2);
  (**(code **)(*param_1 + 0x2c))(param_1,local_8);
  *in_FS_OFFSET = uStack_1c;
  puStack_14 = &LAB_0040de3a;
  puStack_18 = (undefined1 *)0x40de32;
  FUN_0041b840(&local_8);
  return;
}



void FUN_0040de40(int *param_1,char *param_2)

{
  char cVar1;
  char *pcVar2;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_28;
  undefined1 *puStack_24;
  undefined1 *puStack_20;
  undefined4 uStack_1c;
  undefined1 *puStack_18;
  undefined1 *puStack_14;
  int local_c;
  int *local_8;
  
  puStack_14 = &stack0xfffffffc;
  local_c = 0;
  puStack_18 = &LAB_0040def4;
  uStack_1c = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_1c;
  puStack_20 = (undefined1 *)0x40de67;
  local_8 = param_1;
  FUN_0040d584(param_1);
  puStack_24 = &LAB_0040ded7;
  uStack_28 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_28;
  puStack_20 = &stack0xfffffffc;
  (**(code **)(*local_8 + 0x40))();
  if (param_2 != (char *)0x0) {
    while (pcVar2 = param_2, *param_2 != '\0') {
      for (; ((cVar1 = *pcVar2, cVar1 != '\0' && (cVar1 != '\n')) && (cVar1 != '\r'));
          pcVar2 = pcVar2 + 1) {
      }
      FUN_0041b928(&local_c,(undefined4 *)param_2,(int)pcVar2 - (int)param_2);
      (**(code **)(*local_8 + 0x34))(local_8,local_c);
      param_2 = pcVar2;
      if (*pcVar2 == '\r') {
        param_2 = pcVar2 + 1;
      }
      if (*param_2 == '\n') {
        param_2 = param_2 + 1;
      }
    }
  }
  *in_FS_OFFSET = uStack_28;
  puStack_20 = &LAB_0040dede;
  puStack_24 = (undefined1 *)0x40ded6;
  FUN_0040d640(local_8);
  return;
}



void FUN_0040df04(int *param_1,int param_2)

{
  int iVar1;
  undefined4 extraout_ECX;
  uint extraout_ECX_00;
  undefined4 extraout_ECX_01;
  undefined4 uVar2;
  undefined4 extraout_EDX;
  undefined4 extraout_EDX_00;
  undefined4 uVar3;
  int iVar4;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_28;
  undefined1 *puStack_24;
  undefined1 *puStack_20;
  undefined4 *local_10;
  int local_c;
  int *local_8;
  
  puStack_20 = &stack0xfffffffc;
  local_10 = (undefined4 *)0x0;
  puStack_24 = &LAB_0040df7a;
  uStack_28 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_28;
  local_c = param_2;
  local_8 = param_1;
  FUN_0041056c(param_2,param_2,0);
  iVar1 = (**(code **)(*local_8 + 0x14))();
  uVar2 = extraout_ECX;
  uVar3 = extraout_EDX;
  if (-1 < iVar1 + -1) {
    iVar4 = 0;
    do {
      (**(code **)(*local_8 + 0xc))(local_8,iVar4,&local_10);
      FUN_0041057c(local_c,local_10,extraout_ECX_00);
      iVar4 = iVar4 + 1;
      iVar1 = iVar1 + -1;
      uVar2 = extraout_ECX_01;
      uVar3 = extraout_EDX_00;
    } while (iVar1 != 0);
  }
  FUN_00410574(local_c,uVar3,uVar2);
  *in_FS_OFFSET = uStack_28;
  puStack_20 = &LAB_0040df81;
  puStack_24 = (undefined1 *)0x40df79;
  FUN_0041b840((int *)&local_10);
  return;
}



int FUN_0040dfe4(int *param_1,undefined4 *param_2,int param_3)

{
  char cVar1;
  int local_10;
  
  if ((char)param_1[6] == '\0') {
    local_10 = param_1[4];
  }
  else {
    local_10 = param_3;
    cVar1 = (**(code **)(*param_1 + 0x78))(param_1,param_2,&local_10);
    if (cVar1 != '\0') {
      if (*(char *)((int)param_1 + 0x19) == '\0') {
        return local_10;
      }
      if (*(char *)((int)param_1 + 0x19) == '\x02') {
        FUN_0040d738(param_1,(undefined4 *)PTR_PTR_00430694,0);
      }
    }
  }
  FUN_0040e30c(param_1,local_10,param_2);
  return local_10;
}



void FUN_0040e0ac(int *param_1,int param_2)

{
  if ((param_2 < 0) || (param_1[4] <= param_2)) {
    FUN_0040d738(param_1,(undefined4 *)PTR_PTR_00430684,param_2);
  }
  (**(code **)(*param_1 + 0x74))();
  FUN_0041c1c4((VARIANTARG *)(param_1[3] + param_2 * 8),"\x0e\vTStringItem\b");
  param_1[4] = param_1[4] + -1;
  if (param_2 < param_1[4]) {
    FUN_0041aae4((undefined4 *)(param_1[3] + 8 + param_2 * 8),
                 (undefined4 *)(param_1[3] + param_2 * 8),(param_1[4] - param_2) * 8);
  }
  (**(code **)(*param_1 + 0x70))();
  return;
}



void FUN_0040e110(int *param_1,int param_2,int param_3)

{
  if ((param_2 < 0) || (param_1[4] <= param_2)) {
    FUN_0040d738(param_1,(undefined4 *)PTR_PTR_00430684,param_2);
  }
  if ((param_3 < 0) || (param_1[4] <= param_3)) {
    FUN_0040d738(param_1,(undefined4 *)PTR_PTR_00430684,param_3);
  }
  (**(code **)(*param_1 + 0x74))();
  FUN_0040e168((int)param_1,param_2,param_3);
  (**(code **)(*param_1 + 0x70))();
  return;
}



void FUN_0040e168(int param_1,int param_2,int param_3)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  
  puVar1 = (undefined4 *)(*(int *)(param_1 + 0xc) + param_2 * 8);
  puVar2 = (undefined4 *)(*(int *)(param_1 + 0xc) + param_3 * 8);
  uVar3 = *puVar1;
  *puVar1 = *puVar2;
  *puVar2 = uVar3;
  uVar3 = puVar1[1];
  puVar1[1] = puVar2[1];
  puVar2[1] = uVar3;
  return;
}



undefined1 FUN_0040e18c(int param_1,undefined *param_2,uint *param_3)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  undefined1 local_14;
  
  local_14 = 0;
  uVar3 = 0;
  iVar4 = *(int *)(param_1 + 0x10) + -1;
  if (-1 < iVar4) {
    do {
      uVar2 = iVar4 + uVar3 >> 1;
      iVar1 = FUN_0041411c(*(undefined **)(*(int *)(param_1 + 0xc) + uVar2 * 8),param_2);
      if (iVar1 < 0) {
        uVar3 = uVar2 + 1;
      }
      else {
        iVar4 = uVar2 - 1;
        if ((iVar1 == 0) && (local_14 = 1, *(char *)(param_1 + 0x19) != '\x01')) {
          uVar3 = uVar2;
        }
      }
    } while ((int)uVar3 <= iVar4);
  }
  *param_3 = uVar3;
  return local_14;
}



void FUN_0040e1f4(int param_1,int param_2,int *param_3)

{
  if ((param_2 < 0) || (*(int *)(param_1 + 0x10) <= param_2)) {
    FUN_0040d738(param_1,(undefined4 *)PTR_PTR_00430684,param_2);
  }
  FUN_0041b894(param_3,*(undefined4 **)(*(int *)(param_1 + 0xc) + param_2 * 8));
  return;
}



undefined4 FUN_0040e230(int param_1,int param_2)

{
  if ((param_2 < 0) || (*(int *)(param_1 + 0x10) <= param_2)) {
    FUN_0040d738(param_1,(undefined4 *)PTR_PTR_00430684,param_2);
  }
  return *(undefined4 *)(*(int *)(param_1 + 0xc) + 4 + param_2 * 8);
}



void FUN_0040e258(int *param_1)

{
  int iVar1;
  int iVar2;
  
  iVar1 = param_1[5];
  if (iVar1 < 0x41) {
    if (iVar1 < 9) {
      iVar2 = 4;
    }
    else {
      iVar2 = 0x10;
    }
  }
  else {
    iVar2 = iVar1;
    if (iVar1 < 0) {
      iVar2 = iVar1 + 3;
    }
    iVar2 = iVar2 >> 2;
  }
  (**(code **)(*param_1 + 0x28))(param_1,iVar2 + iVar1);
  return;
}



undefined4 FUN_0040e28c(int *param_1,undefined *param_2,undefined4 param_3)

{
  char cVar1;
  undefined4 local_c;
  
  local_c = param_3;
  if ((char)param_1[6] == '\0') {
    local_c = FUN_0040d9c0(param_1,param_2);
  }
  else {
    cVar1 = (**(code **)(*param_1 + 0x78))(param_1,param_2,&local_c);
    if (cVar1 == '\0') {
      local_c = 0xffffffff;
    }
  }
  return local_c;
}



void FUN_0040e2c4(int *param_1,int param_2,undefined4 *param_3)

{
  if ((char)param_1[6] != '\0') {
    FUN_0040d738(param_1,(undefined4 *)PTR_PTR_00430690,0);
  }
  if ((param_2 < 0) || (param_1[4] < param_2)) {
    FUN_0040d738(param_1,(undefined4 *)PTR_PTR_00430684,param_2);
  }
  FUN_0040e30c(param_1,param_2,param_3);
  return;
}



void FUN_0040e30c(int *param_1,int param_2,undefined4 *param_3)

{
  int *piVar1;
  
  (**(code **)(*param_1 + 0x74))();
  if (param_1[4] == param_1[5]) {
    FUN_0040e258(param_1);
  }
  if (param_2 < param_1[4]) {
    FUN_0041aae4((undefined4 *)(param_1[3] + param_2 * 8),
                 (undefined4 *)(param_1[3] + 8 + param_2 * 8),(param_1[4] - param_2) * 8);
  }
  piVar1 = (int *)(param_1[3] + param_2 * 8);
  *piVar1 = 0;
  piVar1[1] = 0;
  FUN_0041b894(piVar1,param_3);
  param_1[4] = param_1[4] + 1;
  (**(code **)(*param_1 + 0x70))();
  return;
}



void FUN_0040e374(int *param_1,int param_2,undefined4 *param_3)

{
  if ((char)param_1[6] != '\0') {
    FUN_0040d738(param_1,(undefined4 *)PTR_PTR_00430690,0);
  }
  if ((param_2 < 0) || (param_1[4] <= param_2)) {
    FUN_0040d738(param_1,(undefined4 *)PTR_PTR_00430684,param_2);
  }
  (**(code **)(*param_1 + 0x74))();
  FUN_0041b894((int *)(param_1[3] + param_2 * 8),param_3);
  (**(code **)(*param_1 + 0x70))();
  return;
}



void FUN_0040e3cc(int *param_1,int param_2,undefined4 param_3)

{
  if ((param_2 < 0) || (param_1[4] <= param_2)) {
    FUN_0040d738(param_1,(undefined4 *)PTR_PTR_00430684,param_2);
  }
  (**(code **)(*param_1 + 0x74))();
  *(undefined4 *)(param_1[3] + 4 + param_2 * 8) = param_3;
  (**(code **)(*param_1 + 0x70))();
  return;
}



void FUN_0040e408(int param_1,uint param_2,uint param_3,undefined *param_4)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  uint local_14;
  uint local_c;
  
  local_c = param_2;
  do {
    local_14 = local_c + param_3 >> 1;
    uVar3 = local_c;
    uVar4 = param_3;
    do {
      while (iVar2 = (*(code *)param_4)(param_1,uVar3,local_14), iVar2 < 0) {
        uVar3 = uVar3 + 1;
      }
      while (iVar2 = (*(code *)param_4)(param_1,uVar4,local_14), 0 < iVar2) {
        uVar4 = uVar4 - 1;
      }
      if ((int)uVar3 <= (int)uVar4) {
        FUN_0040e168(param_1,uVar3,uVar4);
        uVar1 = uVar4;
        if ((uVar3 != local_14) && (uVar1 = local_14, uVar4 == local_14)) {
          uVar1 = uVar3;
        }
        local_14 = uVar1;
        uVar3 = uVar3 + 1;
        uVar4 = uVar4 - 1;
      }
    } while ((int)uVar3 <= (int)uVar4);
    if ((int)local_c < (int)uVar4) {
      FUN_0040e408(param_1,local_c,uVar4,param_4);
    }
    local_c = uVar3;
  } while ((int)uVar3 < (int)param_3);
  return;
}



void FUN_0040e49c(int param_1,int param_2)

{
  FUN_0041a9f0((int *)(param_1 + 0xc),param_2 << 3);
  *(int *)(param_1 + 0x14) = param_2;
  return;
}



void FUN_0040e4f0(int *param_1,undefined *param_2)

{
  if (((char)param_1[6] == '\0') && (1 < param_1[4])) {
    (**(code **)(*param_1 + 0x74))();
    FUN_0040e408((int)param_1,0,param_1[4] - 1,param_2);
    (**(code **)(*param_1 + 0x70))();
  }
  return;
}



void FUN_0040e524(int *param_1)

{
  (**(code **)(*param_1 + 0xc))(param_1,0,1);
  return;
}



undefined4 FUN_0040e534(int *param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined2 extraout_var;
  
  uVar1 = (**(code **)(*param_1 + 0xc))(param_1,0,1);
  uVar2 = (**(code **)(*param_1 + 0xc))(param_1,0,CONCAT22(extraout_var,2));
  (**(code **)(*param_1 + 0xc))(param_1,uVar1,0);
  return uVar2;
}



void FUN_0040e56c(int *param_1,undefined4 param_2,int param_3)

{
  int iVar1;
  int *piVar2;
  
  if (param_3 != 0) {
    iVar1 = (**(code **)(*param_1 + 8))(param_1,param_2,param_3);
    if (param_3 != iVar1) {
      piVar2 = FUN_00416d50((int *)&DAT_0040bc58,'\x01',(undefined4 *)PTR_PTR_0043067c);
      FUN_0041b508(piVar2);
    }
  }
  return;
}



void FUN_0040e5d0(int *param_1,undefined4 param_2)

{
  BOOL BVar1;
  
  (**(code **)(*param_1 + 0xc))(param_1,param_2,0);
  BVar1 = SetEndOfFile((HANDLE)param_1[1]);
  FUN_00417dc4(BVar1);
  return;
}



int * FUN_0040e5f0(int *param_1,char param_2,undefined *param_3,ushort param_4)

{
  int iVar1;
  undefined4 uVar2;
  undefined *extraout_ECX;
  char extraout_DL;
  undefined4 *in_FS_OFFSET;
  undefined4 in_stack_ffffffd4;
  undefined4 in_stack_ffffffd8;
  undefined *local_10;
  undefined1 local_c;
  char local_5;
  
  local_5 = '\0';
  if (param_2 != '\0') {
    param_1 = (int *)FUN_0041b0d0((int)param_1,param_2,param_3,in_stack_ffffffd4,in_stack_ffffffd8);
    param_3 = extraout_ECX;
    local_5 = extraout_DL;
  }
  if (param_4 == 0xffff) {
    iVar1 = FUN_004142cc(param_3);
    param_1[1] = iVar1;
    if (iVar1 < 0) {
      local_c = 0xb;
      local_10 = param_3;
      uVar2 = FUN_00416d8c((int)&DAT_0040baf4,'\x01',(undefined4 *)PTR_PTR_00430670,0,&local_10);
      FUN_0041b508(uVar2);
    }
  }
  else {
    iVar1 = FUN_0041428c(param_3,(uint)param_4);
    param_1[1] = iVar1;
    if (param_1[1] < 0) {
      local_c = 0xb;
      local_10 = param_3;
      uVar2 = FUN_00416d8c((int)&DAT_0040bb50,'\x01',(undefined4 *)PTR_PTR_00430674,0,&local_10);
      FUN_0041b508(uVar2);
    }
  }
  if (local_5 != '\0') {
    FUN_0041b128(param_1);
    *in_FS_OFFSET = in_stack_ffffffd4;
  }
  return param_1;
}



int * FUN_0040e744(int *param_1,char param_2,int param_3,undefined4 *param_4,undefined4 *param_5,
                  int param_6,int param_7)

{
  int extraout_ECX;
  char extraout_DL;
  char cVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 in_stack_ffffffe4;
  undefined4 in_stack_ffffffe8;
  
  cVar1 = '\0';
  if (param_2 != '\0') {
    param_1 = (int *)FUN_0041b0d0((int)param_1,param_2,param_3,in_stack_ffffffe4,in_stack_ffffffe8);
    param_3 = extraout_ECX;
    cVar1 = extraout_DL;
  }
  param_1[1] = param_3;
  param_1[2] = param_7;
  param_1[3] = param_6;
  FUN_0041b894(param_1 + 4,param_5);
  FUN_0041b894(param_1 + 5,param_4);
  if (cVar1 != '\0') {
    FUN_0041b128(param_1);
    *in_FS_OFFSET = in_stack_ffffffe4;
  }
  return param_1;
}



bool FUN_0040e79c(int param_1)

{
  char cVar1;
  char *pcVar2;
  char *pcVar3;
  
  pcVar2 = *(char **)(param_1 + 0x14);
  for (pcVar3 = pcVar2; (*pcVar3 != '\0' && (*pcVar3 != '.')); pcVar3 = pcVar3 + 1) {
  }
  cVar1 = *pcVar3;
  if (cVar1 != '\0') {
    FUN_0041b928((int *)(param_1 + 0x10),(undefined4 *)pcVar2,(int)pcVar3 - (int)pcVar2);
    FUN_0041bcf8((int *)(param_1 + 0x14),1,((int)pcVar3 - (int)pcVar2) + 1);
  }
  return cVar1 != '\0';
}



void FUN_0040e7e4(int param_1,char *param_2)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  char *pcVar4;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_24;
  undefined1 *puStack_20;
  undefined1 *puStack_1c;
  char *local_c;
  int local_8;
  
  puStack_1c = &stack0xfffffffc;
  local_c = (char *)0x0;
  puStack_20 = &LAB_0040e88a;
  uStack_24 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_24;
  local_8 = 0;
  iVar1 = local_8;
  if (param_2 != (char *)0x0) {
    while (pcVar4 = param_2, iVar1 = param_1, *param_2 != '\0') {
      for (; (*pcVar4 != '\0' && (1 < (byte)(*pcVar4 - 0x2dU))); pcVar4 = pcVar4 + 1) {
      }
      FUN_0041b928((int *)&local_c,(undefined4 *)param_2,(int)pcVar4 - (int)param_2);
      iVar2 = FUN_00410ac8(param_1,local_c);
      if ((iVar2 == 0) && (uVar3 = FUN_004140f8(local_c,"Owner"), (char)uVar3 != '\0')) {
        iVar2 = param_1;
      }
      iVar1 = local_8;
      if (iVar2 == 0) break;
      param_2 = pcVar4;
      if (*pcVar4 == '.') {
        param_2 = pcVar4 + 1;
      }
      if (*param_2 == '-') {
        param_2 = param_2 + 1;
      }
      param_1 = iVar2;
      if (*param_2 == '>') {
        param_2 = param_2 + 1;
      }
    }
  }
  local_8 = iVar1;
  *in_FS_OFFSET = uStack_24;
  puStack_1c = &LAB_0040e891;
  puStack_20 = (undefined1 *)0x40e889;
  FUN_0041b840((int *)&local_c);
  return;
}



void FUN_0040e8ac(int param_1,char *param_2)

{
  int *piVar1;
  int *piVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_24;
  undefined1 *puStack_20;
  undefined1 *puStack_1c;
  
  if (DAT_0043085c == 0) {
    return;
  }
  puStack_1c = (undefined1 *)0x40e8d2;
  piVar1 = (int *)FUN_0040cdf0(DAT_0043085c);
  puStack_20 = &LAB_0040e947;
  uStack_24 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_24;
  iVar4 = piVar1[2] + -1;
  puStack_1c = &stack0xfffffffc;
  if (-1 < iVar4) {
    do {
      piVar2 = (int *)GetFieldDescriptor(piVar1,iVar4);
      if ((param_1 == 0) || (piVar2[2] == param_1)) {
        if (param_2 != (char *)0x0) {
          uVar3 = FUN_004140f8(param_2,(char *)piVar2[4]);
          if ((char)uVar3 == '\0') goto LAB_0040e929;
        }
        FUN_0040c940(piVar1,iVar4);
        FUN_0041ae54(piVar2);
      }
LAB_0040e929:
      iVar4 = iVar4 + -1;
    } while (iVar4 != -1);
  }
  *in_FS_OFFSET = uStack_24;
  puStack_1c = (undefined1 *)0x40e94e;
  puStack_20 = (undefined1 *)0x40e946;
  FUN_0040ce04(DAT_0043085c);
  return;
}



void FUN_0040e958(int param_1)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_20;
  undefined1 *puStack_1c;
  undefined1 *puStack_18;
  
  if (DAT_0043085c != 0) {
    puStack_18 = (undefined1 *)0x40e975;
    piVar1 = (int *)FUN_0040cdf0(DAT_0043085c);
    puStack_1c = &LAB_0040e9cf;
    uStack_20 = *in_FS_OFFSET;
    *in_FS_OFFSET = &uStack_20;
    iVar3 = piVar1[2] + -1;
    puStack_18 = &stack0xfffffffc;
    if (-1 < iVar3) {
      do {
        piVar2 = (int *)GetFieldDescriptor(piVar1,iVar3);
        if (piVar2[1] == param_1) {
          FUN_0040c940(piVar1,iVar3);
          FUN_0041ae54(piVar2);
        }
        iVar3 = iVar3 + -1;
      } while (iVar3 != -1);
    }
    *in_FS_OFFSET = uStack_20;
    puStack_18 = (undefined1 *)0x40e9d6;
    puStack_1c = (undefined1 *)0x40e9ce;
    FUN_0040ce04(DAT_0043085c);
    return;
  }
  return;
}



void FUN_0040e9dc(undefined4 *param_1)

{
  int *piVar1;
  
  piVar1 = FUN_00416d50((int *)&DAT_0040bc00,'\x01',param_1);
  FUN_0041b508(piVar1);
  return;
}



void __stdcall FUN_0040e9f4(void)

{
  FUN_0040e9dc((undefined4 *)PTR_PTR_004306a0);
  return;
}



void __stdcall FUN_0040ea00(void)

{
  FUN_0040e9dc((undefined4 *)PTR_PTR_004306a8);
  return;
}



int FUN_0040ea0c(char *param_1,byte *param_2)

{
  int iVar1;
  
  iVar1 = FUN_0040b354(param_1,param_2);
  if (iVar1 == -1) {
    FUN_0040e9f4();
  }
  return iVar1;
}



void FUN_0040ea24(int param_1,undefined4 param_2,uint param_3)

{
  uint uVar1;
  uint extraout_ECX;
  undefined4 extraout_EDX;
  
  uVar1 = FUN_0041006c(param_1,param_2,param_3);
  if ((char)param_2 != (char)uVar1) {
    *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + -1;
    FUN_004101cc(param_1,extraout_EDX,extraout_ECX);
    FUN_0040e9f4();
  }
  return;
}



bool FUN_0040ea48(int param_1,undefined4 param_2,uint param_3)

{
  uint uVar1;
  
  uVar1 = FUN_0041006c(param_1,param_2,param_3);
  *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + -1;
  return (char)uVar1 == '\0';
}



void FUN_0040ea5c(int param_1)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_1c;
  undefined1 *puStack_18;
  undefined1 *puStack_14;
  
  puStack_14 = (undefined1 *)0x40ea6e;
  piVar1 = (int *)FUN_0040cdf0(DAT_0043085c);
  puStack_18 = &LAB_0040eacc;
  uStack_1c = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_1c;
  iVar3 = piVar1[2] + -1;
  puStack_14 = &stack0xfffffffc;
  if (-1 < iVar3) {
    do {
      piVar2 = (int *)GetFieldDescriptor(piVar1,iVar3);
      if ((piVar2[1] == *(int *)(param_1 + 4)) && (piVar2[3] == *(int *)(param_1 + 0xc))) {
        FUN_0041ae54(piVar2);
        FUN_0040c940(piVar1,iVar3);
      }
      iVar3 = iVar3 + -1;
    } while (iVar3 != -1);
  }
  *in_FS_OFFSET = uStack_1c;
  puStack_14 = &LAB_0040ead3;
  puStack_18 = (undefined1 *)0x40eacb;
  FUN_0040ce04(DAT_0043085c);
  return;
}



void FUN_0040ead8(int *param_1)

{
  bool bVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_34;
  undefined1 *puStack_30;
  undefined1 *puStack_2c;
  undefined4 uStack_28;
  undefined1 *puStack_24;
  undefined1 *puStack_20;
  int local_10;
  char *local_c;
  int *local_8;
  
  puStack_20 = &stack0xfffffffc;
  puStack_2c = &stack0xfffffffc;
  local_c = (char *)0x0;
  puStack_24 = &LAB_0040ebec;
  uStack_28 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_28;
  iVar5 = param_1[0xc];
  local_8 = param_1;
  if (iVar5 == 0) {
    *in_FS_OFFSET = uStack_28;
    puStack_20 = &LAB_0040ebf3;
    puStack_24 = (undefined1 *)0x40ebeb;
    FUN_0041b840((int *)&local_c);
    return;
  }
  puStack_30 = &LAB_0040ebcf;
  uStack_34 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_34;
  local_10 = *(int *)(iVar5 + 8);
  if (-1 < local_10 + -1) {
    iVar5 = 0;
    puStack_20 = &stack0xfffffffc;
    do {
      iVar2 = GetFieldDescriptor((undefined4 *)local_8[0xc],iVar5);
      FUN_0041b8d8((int *)&local_c,*(int *)(iVar2 + 0x14));
      (**(code **)(*local_8 + 0x20))(local_8,&local_c);
      iVar3 = FUN_0040e7e4(*(int *)(iVar2 + 8),local_c);
      iVar4 = GetFieldDescriptor((undefined4 *)local_8[0xc],iVar5);
      FUN_0040ea5c(iVar4);
      if (iVar3 == 0) {
        bVar1 = FUN_0040e79c(iVar2);
        if (!bVar1) goto LAB_0040eba2;
        iVar2 = GetFieldDescriptor((undefined4 *)local_8[0xc],iVar5);
        FUN_0040cd68(DAT_0043085c,iVar2);
        FUN_0040cb7c((int *)local_8[0xc],iVar5,0);
      }
      else {
LAB_0040eba2:
        FUN_0040b448(*(int **)(iVar2 + 4),*(undefined4 **)(iVar2 + 0xc),iVar3);
      }
      iVar5 = iVar5 + 1;
      local_10 = local_10 + -1;
    } while (local_10 != 0);
  }
  *in_FS_OFFSET = uStack_34;
  puStack_2c = (undefined1 *)0x40ebd6;
  puStack_30 = (undefined1 *)0x40ebce;
  FUN_0040ebfc((int)local_8);
  return;
}



void FUN_0040ebfc(int param_1)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  
  if (*(int *)(param_1 + 0x30) != 0) {
    iVar2 = *(int *)(*(int *)(param_1 + 0x30) + 8);
    if (-1 < iVar2 + -1) {
      iVar3 = 0;
      do {
        piVar1 = (int *)GetFieldDescriptor(*(undefined4 **)(param_1 + 0x30),iVar3);
        FUN_0041ae54(piVar1);
        iVar3 = iVar3 + 1;
        iVar2 = iVar2 + -1;
      } while (iVar2 != 0);
    }
    FUN_0041ae54(*(int **)(param_1 + 0x30));
    *(undefined4 *)(param_1 + 0x30) = 0;
  }
  return;
}



void FUN_0040ec38(int param_1,undefined4 param_2,uint param_3)

{
  FUN_0041006c(param_1,param_2,param_3);
  *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + -1;
  return;
}



void FUN_0040ec48(int param_1,undefined4 param_2,uint param_3)

{
  FUN_004101cc(param_1,param_2,param_3);
  FUN_0040ea00();
  return;
}



void FUN_0040ec54(int param_1,undefined4 *param_2,uint param_3)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  undefined4 *puVar4;
  
  while (param_3 != 0) {
    uVar2 = *(uint *)(param_1 + 0x14) - *(uint *)(param_1 + 0x10);
    if (*(uint *)(param_1 + 0x14) < *(uint *)(param_1 + 0x10) || uVar2 == 0) {
      FUN_0040eca0(param_1);
      uVar2 = *(uint *)(param_1 + 0x14);
    }
    if (param_3 <= uVar2) {
      uVar2 = param_3;
    }
    param_3 = param_3 - uVar2;
    iVar1 = *(int *)(param_1 + 0x10);
    *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + uVar2;
    puVar4 = (undefined4 *)(*(int *)(param_1 + 8) + iVar1);
    for (uVar3 = uVar2 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
      *param_2 = *puVar4;
      puVar4 = puVar4 + 1;
      param_2 = param_2 + 1;
    }
    for (uVar2 = uVar2 & 3; uVar2 != 0; uVar2 = uVar2 - 1) {
      *(undefined1 *)param_2 = *(undefined1 *)puVar4;
      puVar4 = (undefined4 *)((int)puVar4 + 1);
      param_2 = (undefined4 *)((int)param_2 + 1);
    }
  }
  return;
}



void FUN_0040eca0(int param_1)

{
  int iVar1;
  int *piVar2;
  
  iVar1 = (**(code **)(**(int **)(param_1 + 4) + 4))
                    (*(int **)(param_1 + 4),*(undefined4 *)(param_1 + 8),
                     *(undefined4 *)(param_1 + 0xc));
  *(int *)(param_1 + 0x14) = iVar1;
  if (iVar1 == 0) {
    piVar2 = FUN_00416d50((int *)&DAT_0040bc00,'\x01',(undefined4 *)PTR_PTR_00430678);
    FUN_0041b508(piVar2);
  }
  *(undefined4 *)(param_1 + 0x10) = 0;
  return;
}



void FUN_0040ecdc(int param_1,undefined4 param_2,uint param_3)

{
  uint extraout_ECX;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_1c;
  undefined1 *puStack_18;
  undefined1 *puStack_14;
  int local_c;
  char local_5;
  
  puStack_14 = &stack0xfffffffc;
  local_c = 0;
  puStack_18 = &LAB_0040ed4e;
  uStack_1c = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_1c;
  FUN_0040ea24(param_1,6,param_3);
  FUN_0040ec54(param_1,(undefined4 *)&local_5,1);
  if (local_5 != '\x01') {
    *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + -1;
    FUN_0040ff88(param_1,&local_c,extraout_ECX);
    FUN_0040e9f4();
  }
  FUN_0040ec54(param_1,(undefined4 *)&local_5,1);
  *in_FS_OFFSET = uStack_1c;
  puStack_14 = &LAB_0040ed55;
  puStack_18 = (undefined1 *)0x40ed4d;
  FUN_0041b840(&local_c);
  return;
}



void FUN_0040ed60(int *param_1,int *param_2)

{
  undefined1 *puVar1;
  bool bVar2;
  char cVar3;
  int *piVar4;
  uint extraout_ECX;
  uint extraout_ECX_00;
  uint extraout_ECX_01;
  uint extraout_ECX_02;
  uint extraout_ECX_03;
  uint extraout_ECX_04;
  uint extraout_ECX_05;
  uint extraout_ECX_06;
  uint uVar5;
  uint extraout_ECX_07;
  undefined4 extraout_EDX;
  undefined4 extraout_EDX_00;
  undefined4 extraout_EDX_01;
  undefined4 extraout_EDX_02;
  undefined4 extraout_EDX_03;
  undefined4 extraout_EDX_04;
  undefined4 extraout_EDX_05;
  undefined4 extraout_EDX_06;
  undefined4 uVar6;
  undefined4 extraout_EDX_07;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_1c;
  undefined1 *puStack_18;
  undefined1 *puStack_14;
  
  puStack_14 = (undefined1 *)0x40ed73;
  (**(code **)(*param_2 + 0x14))();
  puStack_18 = &LAB_0040ee00;
  uStack_1c = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_1c;
  puStack_14 = &stack0xfffffffc;
  bVar2 = FUN_0040ea48((int)param_1,extraout_EDX,extraout_ECX);
  uVar5 = extraout_ECX_00;
  uVar6 = extraout_EDX_00;
  if (!bVar2) {
    FUN_0040d1f8(param_2);
    uVar5 = extraout_ECX_01;
    uVar6 = extraout_EDX_01;
  }
  while( true ) {
    bVar2 = FUN_0040ea48((int)param_1,uVar6,uVar5);
    if (bVar2) break;
    cVar3 = FUN_0040ec38((int)param_1,extraout_EDX_07,extraout_ECX_07);
    if ((byte)(cVar3 - 2U) < 3) {
      FUN_0040f604((int)param_1,extraout_EDX_02,extraout_ECX_02);
    }
    piVar4 = (int *)FUN_0040d1dc((int)param_2);
    FUN_0040f6b4((int)param_1,extraout_EDX_03,extraout_ECX_03);
    uVar5 = extraout_ECX_04;
    uVar6 = extraout_EDX_04;
    while( true ) {
      bVar2 = FUN_0040ea48((int)param_1,uVar6,uVar5);
      if (bVar2) break;
      FUN_0040f7e0(param_1,piVar4);
      uVar5 = extraout_ECX_05;
      uVar6 = extraout_EDX_05;
    }
    FUN_0040f6bc(param_1);
    uVar5 = extraout_ECX_06;
    uVar6 = extraout_EDX_06;
  }
  FUN_0040f6bc(param_1);
  puVar1 = puStack_14;
  *in_FS_OFFSET = uStack_1c;
  puStack_14 = &LAB_0040ee07;
  puStack_18 = (undefined1 *)0x40edff;
  (**(code **)(*param_2 + 0x18))(param_2,*param_2,puVar1);
  return;
}



undefined4 __cdecl FUN_0040ee0c(int param_1)

{
  if (((*(byte *)(param_1 + -1) & 1) == 0) && (*(int *)(param_1 + -8) == 0)) {
    return CONCAT31((int3)((uint)param_1 >> 8),1);
  }
  return 0;
}



undefined4 FUN_0040ee2c(undefined4 *param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 uVar4;
  
  uVar4 = 0;
  piVar1 = (int *)FUN_00416a70();
  uVar2 = FUN_0041afcc(piVar1,(int)&DAT_004135b0);
  if ((char)uVar2 != '\0') {
    uVar2 = FUN_0040ee0c(param_4);
    if ((char)uVar2 != '\0') {
      FUN_0041ae54((int *)*param_1);
    }
    *param_1 = 0;
    FUN_0041037c(*(int **)(param_4 + -0xc),0);
    iVar3 = FUN_00416a70();
    uVar4 = (**(code **)(**(int **)(param_4 + -0xc) + 0x10))
                      (*(int **)(param_4 + -0xc),*(undefined4 *)(iVar3 + 4));
  }
  return uVar4;
}



void __cdecl FUN_0040ee8c(int param_1)

{
  ushort *puVar1;
  int iVar2;
  undefined4 uVar3;
  int in_ECX;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_28;
  undefined1 *puStack_24;
  undefined1 *puStack_20;
  undefined4 uStack_1c;
  undefined1 *puStack_18;
  undefined1 *puStack_14;
  
  puStack_14 = &stack0xfffffffc;
  puStack_18 = &LAB_0040ef72;
  uStack_1c = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_1c;
  puStack_20 = (undefined1 *)0x40eeb1;
  iVar2 = FUN_00410424(*(int *)(param_1 + -0xc),*(char **)(param_1 + -0x14),in_ECX);
  *(undefined4 *)(param_1 + -0x10) = 0;
  if (*(short *)(*(int *)(param_1 + -0xc) + 0x6a) != 0) {
    puStack_20 = (undefined1 *)(param_1 + -0x10);
    puStack_24 = (undefined1 *)0x40eee3;
    (**(code **)(*(int *)(param_1 + -0xc) + 0x68))
              (*(undefined4 *)(*(int *)(param_1 + -0xc) + 0x6c),*(undefined4 *)(param_1 + -0xc),
               iVar2);
  }
  if (*(int *)(param_1 + -0x10) == 0) {
    puStack_20 = (undefined1 *)0x40eef1;
    uVar3 = (**(code **)(iVar2 + -0xc))();
    *(undefined4 *)(param_1 + -0x10) = uVar3;
    if ((*(byte *)(param_1 + -1) & 4) != 0) {
      puVar1 = (ushort *)(*(int *)(param_1 + -0x10) + 0x20);
      *puVar1 = *puVar1 | 1;
      puVar1 = (ushort *)(*(int *)(param_1 + -0x10) + 0x20);
      *puVar1 = *puVar1 | 0x200;
    }
    puStack_24 = &LAB_0040ef46;
    uStack_28 = *in_FS_OFFSET;
    *in_FS_OFFSET = &uStack_28;
    puStack_20 = &stack0xfffffffc;
    (**(code **)(**(int **)(param_1 + -0x10) + 0x2c))
              (*(int **)(param_1 + -0x10),0xffffffff,
               *(undefined4 *)(*(int *)(param_1 + -0xc) + 0x28));
    *in_FS_OFFSET = uStack_28;
  }
  puVar1 = (ushort *)(*(int *)(param_1 + -0x10) + 0x20);
  *puVar1 = *puVar1 | 1;
  *in_FS_OFFSET = uStack_1c;
  return;
}



void __cdecl FUN_0040ef9c(int param_1)

{
  ushort *puVar1;
  int iVar2;
  undefined4 in_ECX;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_1c;
  undefined1 *puStack_18;
  undefined1 *puStack_14;
  
  puStack_14 = &stack0xfffffffc;
  puStack_18 = &LAB_0040f012;
  uStack_1c = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_1c;
  FUN_0041b014(*(int **)(param_1 + -0x10),*(undefined4 *)(*(int *)(param_1 + -0xc) + 0x2c),in_ECX);
  (**(code **)(**(int **)(param_1 + -0xc) + 0x1c))
            (*(int **)(param_1 + -0xc),*(undefined4 *)(param_1 + -0x10),param_1 + -0x18);
  if (DAT_00430848 != (code *)0x0) {
    iVar2 = (*DAT_00430848)(*(undefined4 *)(param_1 + -0x18));
    if (iVar2 == *(int *)(param_1 + -0x10)) {
      puVar1 = (ushort *)(*(int *)(param_1 + -0x10) + 0x20);
      *puVar1 = *puVar1 | 0x200;
    }
  }
  *in_FS_OFFSET = uStack_1c;
  return;
}



void __cdecl FUN_0040f03c(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  int in_ECX;
  undefined4 extraout_ECX;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_1c;
  undefined1 *puStack_18;
  undefined1 *puStack_14;
  
  puStack_14 = &stack0xfffffffc;
  puStack_18 = &LAB_0040f0b7;
  uStack_1c = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_1c;
  iVar1 = FUN_00410424(*(int *)(param_1 + -0xc),*(char **)(param_1 + -0x14),in_ECX);
  uVar2 = (**(code **)(**(int **)(param_1 + -0xc) + 0x14))
                    (*(int **)(param_1 + -0xc),*(undefined4 *)(param_1 + -0x18),iVar1);
  *(undefined4 *)(param_1 + -0x10) = uVar2;
  iVar1 = FUN_0041b014(*(int **)(param_1 + -0x10),param_1,extraout_ECX);
  *(int *)(*(int *)(param_1 + -0xc) + 0x2c) = iVar1;
  if (iVar1 == 0) {
    *(undefined4 *)(*(int *)(param_1 + -0xc) + 0x2c) =
         *(undefined4 *)(*(int *)(param_1 + -0xc) + 0x18);
  }
  *in_FS_OFFSET = uStack_1c;
  return;
}



void FUN_0040f0e0(int *param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  uint extraout_ECX;
  uint extraout_ECX_00;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_58;
  undefined1 *puStack_54;
  undefined4 uVar4;
  undefined1 *puVar5;
  undefined4 uStack_40;
  undefined1 *puStack_3c;
  undefined1 *puStack_38;
  undefined4 local_20;
  int local_1c [2];
  int *local_14;
  int *local_10;
  int *local_c;
  byte local_5;
  
  puStack_38 = &stack0xfffffffc;
  local_1c[1] = 0;
  local_1c[0] = 0;
  puStack_3c = &LAB_0040f2a5;
  uStack_40 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_40;
  local_10 = param_1;
  local_c = param_2;
  (**(code **)(*param_1 + 0x24))(param_1,&local_5,&local_20);
  FUN_0040ff88((int)local_10,local_1c + 1,extraout_ECX);
  FUN_0040ff88((int)local_10,local_1c,extraout_ECX_00);
  iVar1 = local_10[0xb];
  iVar2 = local_10[7];
  puVar5 = &LAB_0040f283;
  uVar4 = *in_FS_OFFSET;
  *in_FS_OFFSET = &stack0xffffffb4;
  local_14 = local_c;
  if (local_c == (int *)0x0) {
    if ((local_5 & 1) == 0) {
      puStack_54 = (undefined1 *)0x40f16a;
      FUN_0040ee8c((int)&stack0xfffffffc);
    }
    else {
      puStack_54 = (undefined1 *)0x40f161;
      FUN_0040f03c((int)&stack0xfffffffc);
    }
  }
  if (local_14 != (int *)0x0) {
    puStack_54 = &LAB_0040f241;
    uStack_58 = *in_FS_OFFSET;
    *in_FS_OFFSET = &uStack_58;
    *(ushort *)(local_14 + 8) = *(ushort *)(local_14 + 8) | 1;
    if ((local_5 & 1) == 0) {
      FUN_0040ef9c((int)&stack0xfffffffc);
    }
    if (local_14 == (int *)0x0) {
      *in_FS_OFFSET = uStack_58;
      FUN_0041b5a8(uVar4,(int)puVar5);
      *in_FS_OFFSET = uStack_40;
      puStack_38 = &LAB_0040f2ac;
      puStack_3c = (undefined1 *)0x40f2a4;
      FUN_0041b864(local_1c,2);
      return;
    }
    if ((*(byte *)((int)local_14 + 0x21) & 2) != 0) {
      local_10[7] = (int)local_14;
    }
    *(ushort *)(local_14 + 8) = *(ushort *)(local_14 + 8) | 2;
    (**(code **)(*local_14 + 0x14))(local_14,local_10);
    *(ushort *)(local_14 + 8) = *(ushort *)(local_14 + 8) & 0xfffd;
    if ((local_5 & 2) != 0) {
      FUN_0041b014((int *)local_10[0xb],local_14,local_20);
    }
    if (((local_5 & 1) == 0) && ((*(byte *)((int)local_14 + 0x21) & 2) == 0)) {
      FUN_0040c8f4((int *)local_10[0xd],(int)local_14);
    }
    else {
      iVar3 = FUN_0040ca8c(local_10[0xd],(int)local_14);
      if (iVar3 < 0) {
        FUN_0040c8f4((int *)local_10[0xd],(int)local_14);
      }
    }
    *in_FS_OFFSET = uStack_58;
  }
  *in_FS_OFFSET = uVar4;
  local_10[0xb] = iVar1;
  local_10[7] = iVar2;
  return;
}



void FUN_0040f2b8(int *param_1,int *param_2,uint param_3)

{
  int iVar1;
  uint extraout_ECX;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_18;
  undefined1 *puStack_14;
  undefined1 *puStack_10;
  
  if (param_1[0xc] == 0) {
    puStack_10 = (undefined1 *)0x40f2d7;
    iVar1 = FUN_0041ae24((int *)&PTR_LAB_0040be20,'\x01',param_3);
    param_1[0xc] = iVar1;
    puStack_14 = &LAB_0040f313;
    uStack_18 = *in_FS_OFFSET;
    *in_FS_OFFSET = &uStack_18;
    puStack_10 = &stack0xfffffffc;
    FUN_0040f328(param_1,param_2,extraout_ECX);
    FUN_0040ead8(param_1);
    *in_FS_OFFSET = uStack_18;
    puStack_10 = (undefined1 *)0x40f324;
    puStack_14 = (undefined1 *)0x40f312;
    FUN_0040ebfc((int)param_1);
    return;
  }
  puStack_10 = (undefined1 *)0x40f324;
  FUN_0040f328(param_1,param_2,param_3);
  return;
}



void FUN_0040f328(int *param_1,int *param_2,uint param_3)

{
  int iVar1;
  int iVar2;
  bool bVar3;
  int iVar4;
  uint extraout_ECX;
  undefined4 extraout_ECX_00;
  undefined4 extraout_ECX_01;
  uint extraout_ECX_02;
  uint extraout_ECX_03;
  uint uVar5;
  int *extraout_EDX;
  undefined4 extraout_EDX_00;
  int *extraout_EDX_01;
  int *extraout_EDX_02;
  int *piVar6;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_28;
  undefined1 *puStack_24;
  undefined1 *puStack_20;
  
  piVar6 = param_2;
  while( true ) {
    puStack_20 = (undefined1 *)0x40f34a;
    bVar3 = FUN_0040ea48((int)param_1,piVar6,param_3);
    if (bVar3) break;
    puStack_20 = (undefined1 *)0x40f342;
    FUN_0040f7e0(param_1,param_2);
    param_3 = extraout_ECX;
    piVar6 = extraout_EDX;
  }
  puStack_20 = (undefined1 *)0x40f356;
  FUN_0040f6bc(param_1);
  iVar1 = param_1[0xb];
  iVar2 = param_1[10];
  puStack_20 = (undefined1 *)0x40f373;
  iVar4 = FUN_0041b014(param_2,extraout_EDX_00,extraout_ECX_00);
  param_1[0xb] = iVar4;
  puStack_24 = &LAB_0040f3ea;
  uStack_28 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_28;
  puStack_20 = &stack0xfffffffc;
  iVar4 = FUN_0041b014(param_2,param_1,extraout_ECX_01);
  param_1[10] = iVar4;
  uVar5 = extraout_ECX_02;
  piVar6 = extraout_EDX_01;
  if (iVar4 == 0) {
    param_1[10] = param_1[6];
    piVar6 = param_1;
  }
  while( true ) {
    bVar3 = FUN_0040ea48((int)param_1,piVar6,uVar5);
    if (bVar3) break;
    FUN_0040f0e0(param_1,(int *)0x0);
    uVar5 = extraout_ECX_03;
    piVar6 = extraout_EDX_02;
  }
  FUN_0040f6bc(param_1);
  *in_FS_OFFSET = uStack_28;
  param_1[0xb] = iVar1;
  param_1[10] = iVar2;
  return;
}



void FUN_0040f3f8(int param_1,undefined4 param_2,uint param_3)

{
  uint uVar1;
  uint extraout_ECX;
  undefined4 extraout_EDX;
  undefined4 local_14 [4];
  
  uVar1 = FUN_0041006c(param_1,param_2,param_3);
  if ((char)uVar1 == '\x05') {
    FUN_0040ec54(param_1,local_14,10);
  }
  else {
    *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + -1;
    FUN_0040f604(param_1,extraout_EDX,extraout_ECX);
  }
  return;
}



void FUN_0040f438(int param_1,undefined4 param_2,uint param_3)

{
  uint uVar1;
  uint extraout_ECX;
  undefined4 extraout_EDX;
  undefined4 local_c [2];
  
  uVar1 = FUN_0041006c(param_1,param_2,param_3);
  if ((char)uVar1 == '\x0f') {
    FUN_0040ec54(param_1,local_c,4);
  }
  else {
    *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + -1;
    FUN_0040f604(param_1,extraout_EDX,extraout_ECX);
  }
  return;
}



void FUN_0040f478(int param_1,undefined4 param_2,uint param_3)

{
  uint uVar1;
  uint extraout_ECX;
  undefined4 extraout_EDX;
  undefined4 local_10 [3];
  
  uVar1 = FUN_0041006c(param_1,param_2,param_3);
  if ((char)uVar1 == '\x10') {
    FUN_0040ec54(param_1,local_10,8);
  }
  else {
    *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + -1;
    FUN_0040f604(param_1,extraout_EDX,extraout_ECX);
  }
  return;
}



void FUN_0040f4c4(int param_1,undefined4 param_2,uint param_3)

{
  uint uVar1;
  uint extraout_ECX;
  undefined4 extraout_EDX;
  undefined4 local_10 [3];
  
  uVar1 = FUN_0041006c(param_1,param_2,param_3);
  if ((char)uVar1 == '\x11') {
    FUN_0040ec54(param_1,local_10,8);
  }
  else {
    *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + -1;
    FUN_0040f604(param_1,extraout_EDX,extraout_ECX);
  }
  return;
}



void FUN_0040f504(int param_1,int *param_2,uint param_3)

{
  uint uVar1;
  undefined4 *puVar2;
  uint local_c;
  
  local_c = param_3;
  uVar1 = FUN_0041006c(param_1,param_2,param_3);
  switch(uVar1 & 0x7f) {
  case 0:
    FUN_0041b894(param_2,(undefined4 *)&DAT_0040f5fc);
    break;
  default:
    FUN_0040e9f4();
    break;
  case 7:
    FUN_0040ec54(param_1,&local_c,1);
    FUN_0041b928(param_2,(undefined4 *)0x0,local_c & 0xff);
    puVar2 = (undefined4 *)FUN_0041bc80(param_2);
    FUN_0040ec54(param_1,puVar2,local_c & 0xff);
    break;
  case 8:
    FUN_0041b894(param_2,(undefined4 *)"False");
    break;
  case 9:
    FUN_0041b894(param_2,(undefined4 *)&DAT_0040f5e0);
    break;
  case 0xd:
    FUN_0041b894(param_2,(undefined4 *)&DAT_0040f5f0);
  }
  return;
}



int FUN_0040f604(int param_1,undefined4 param_2,uint param_3)

{
  char cVar1;
  uint uVar2;
  int local_c;
  char local_8 [2];
  short local_6;
  
  uVar2 = FUN_0041006c(param_1,param_2,param_3);
  cVar1 = (char)uVar2;
  if (cVar1 == '\x02') {
    FUN_0040ec54(param_1,(undefined4 *)local_8,1);
    local_c = (int)local_8[0];
  }
  else if (cVar1 == '\x03') {
    FUN_0040ec54(param_1,(undefined4 *)&local_6,2);
    local_c = (int)local_6;
  }
  else if (cVar1 == '\x04') {
    FUN_0040ec54(param_1,&local_c,4);
  }
  else {
    FUN_0040e9f4();
  }
  return local_c;
}



int FUN_0040f670(int param_1,undefined4 param_2,uint param_3)

{
  char cVar1;
  uint extraout_ECX;
  undefined4 extraout_EDX;
  int local_c [2];
  
  cVar1 = FUN_0040ec38(param_1,param_2,param_3);
  if (cVar1 == '\x13') {
    FUN_0041006c(param_1,extraout_EDX,extraout_ECX);
    FUN_0040ec54(param_1,local_c,8);
  }
  else {
    local_c[0] = FUN_0040f604(param_1,extraout_EDX,extraout_ECX);
  }
  return local_c[0];
}



void FUN_0040f6b4(int param_1,undefined4 param_2,uint param_3)

{
  FUN_0040ea24(param_1,CONCAT31((int3)((uint)param_2 >> 8),1),param_3);
  return;
}



void FUN_0040f6bc(int param_1)

{
  uint in_ECX;
  
  FUN_0040ea24(param_1,0,in_ECX);
  return;
}



void FUN_0040f6c4(int param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  undefined4 uVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 uVar2;
  undefined1 *puStack_138;
  undefined1 *puStack_134;
  undefined1 *puStack_130;
  int local_128;
  undefined1 local_124;
  undefined4 local_120;
  undefined1 local_11c;
  undefined4 local_118;
  undefined1 local_114;
  undefined4 local_110;
  undefined1 local_10c;
  byte local_108 [256];
  int local_8;
  
  puStack_130 = &stack0xfffffffc;
  local_8 = 0;
  puStack_134 = &LAB_0040f7b5;
  puStack_138 = (undefined1 *)*in_FS_OFFSET;
  *in_FS_OFFSET = &puStack_138;
  FUN_0041b840(&local_8);
  uVar1 = FUN_0041afcc(*(int **)(param_4 + -4),(int)&PTR_LAB_0040cf35_3_0040c358);
  if ((char)uVar1 != '\0') {
    FUN_0041b8d8(&local_8,*(int *)(*(int *)(param_4 + -4) + 8));
  }
  if (local_8 == 0) {
    FUN_0041ada0(**(int **)(param_4 + -4),local_108);
    FUN_0041ba64(&local_8,local_108);
  }
  local_128 = local_8;
  local_124 = 0xb;
  local_120 = *(undefined4 *)PTR_PTR_00430668;
  local_11c = 0xb;
  local_118 = *(undefined4 *)(param_4 + -8);
  local_114 = 0xb;
  local_110 = *(undefined4 *)(param_1 + 4);
  local_10c = 0xb;
  uVar2 = 3;
  uVar1 = FUN_00416d8c((int)&DAT_0040bc00,'\x01',(undefined4 *)PTR_PTR_004306b0,3,&local_128);
  FUN_0041b508(uVar1);
  *in_FS_OFFSET = uVar2;
  puStack_138 = &LAB_0040f7bc;
  FUN_0041b840(&local_8);
  return;
}



void FUN_0040f7c4(undefined4 param_1,undefined4 param_2,uint param_3,int param_4)

{
  FUN_004101cc(*(int *)(param_4 + -0xc),param_2,param_3);
  FUN_0040e9dc((undefined4 *)PTR_PTR_004306a4);
  return;
}



void FUN_0040f7e0(int *param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  uint extraout_ECX;
  uint extraout_ECX_00;
  uint extraout_ECX_01;
  undefined4 extraout_EDX;
  undefined4 extraout_EDX_00;
  undefined4 extraout_EDX_01;
  int iVar5;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_48;
  undefined1 *puStack_44;
  undefined1 *puStack_40;
  undefined4 local_3c;
  undefined1 *puStack_38;
  undefined1 *puStack_34;
  undefined4 local_30;
  undefined1 *puStack_2c;
  undefined1 *puStack_28;
  int *local_18;
  int *local_14;
  int local_c;
  int *local_8;
  
  puStack_28 = &stack0xfffffffc;
  puStack_34 = &stack0xfffffffc;
  local_c = 0;
  puStack_2c = &LAB_0040f9a4;
  local_30 = *in_FS_OFFSET;
  *in_FS_OFFSET = &local_30;
  puStack_38 = &LAB_0040f959;
  local_3c = *in_FS_OFFSET;
  *in_FS_OFFSET = &local_3c;
  puStack_40 = (undefined1 *)0x40f81b;
  local_8 = param_2;
  FUN_0040ff88((int)param_1,&local_c,0);
  puStack_44 = &LAB_0040f932;
  uStack_48 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_48;
  iVar5 = 1;
  puStack_40 = &stack0xfffffffc;
  iVar1 = FUN_0041bac0(local_c);
  local_14 = local_8;
  *(undefined1 *)(param_1 + 0x1d) = 1;
  iVar2 = iVar5;
  while( true ) {
    for (; (iVar5 <= iVar1 && (*(char *)(local_c + -1 + iVar5) != '.')); iVar5 = iVar5 + 1) {
    }
    FUN_0041bcb8(local_c,iVar2,iVar5 - iVar2,param_1 + 0x1c);
    if (iVar1 < iVar5) break;
    iVar2 = FUN_0041b040(*local_14);
    puVar3 = (undefined4 *)FUN_0040b368(iVar2,(undefined1 *)param_1[0x1c]);
    if (puVar3 == (undefined4 *)0x0) {
      FUN_0040ec48((int)param_1,extraout_EDX,extraout_ECX);
    }
    local_18 = (int *)0x0;
    if (**(char **)*puVar3 == '\a') {
      local_18 = (int *)FUN_0040b3d4(local_14,puVar3);
    }
    uVar4 = FUN_0041afcc(local_18,(int)&PTR_LAB_0040cf35_3_0040bedc);
    if ((char)uVar4 == '\0') {
      FUN_0040f7c4(uVar4,extraout_EDX_00,extraout_ECX_00,(int)&stack0xfffffffc);
    }
    local_14 = local_18;
    iVar5 = iVar5 + 1;
    iVar2 = iVar5;
  }
  iVar2 = FUN_0041b040(*local_14);
  puVar3 = (undefined4 *)FUN_0040b368(iVar2,(undefined1 *)param_1[0x1c]);
  if (puVar3 == (undefined4 *)0x0) {
    *(undefined1 *)(param_1 + 0x1d) = 0;
    (**(code **)(*local_14 + 4))(local_14,param_1);
    *(undefined1 *)(param_1 + 0x1d) = 1;
    if (param_1[0x1c] != 0) {
      FUN_0040ec48((int)param_1,extraout_EDX_01,extraout_ECX_01);
    }
  }
  else {
    FUN_0040fc34(param_1,local_14,puVar3);
  }
  *in_FS_OFFSET = uStack_48;
  *in_FS_OFFSET = local_3c;
  *in_FS_OFFSET = local_30;
  puStack_28 = &LAB_0040f9ab;
  puStack_2c = (undefined1 *)0x40f9a3;
  FUN_0041b840(&local_c);
  return;
}



void FUN_0040f9b4(int *param_1,undefined4 *param_2,undefined4 param_3)

{
  char cVar1;
  code *pcVar2;
  int local_c;
  int *local_8;
  
  local_8 = param_1;
  pcVar2 = (code *)FUN_0040c850(*(int *)*param_2);
  if ((pcVar2 != (code *)0x0) && (cVar1 = (*pcVar2)(param_3,&local_c), cVar1 != '\0')) {
    FUN_0040b448(local_8,param_2,local_c);
    return;
  }
  FUN_0040e9f4();
  return;
}



void FUN_0040f9fc(int param_1,int param_2,undefined4 *param_3,int param_4)

{
  int *piVar1;
  
  piVar1 = FUN_0040e744((int *)&DAT_0040e71c,'\x01',param_1,param_3,(undefined4 *)0x0,param_2,
                        *(int *)(*(int *)(param_4 + -4) + 0x18));
  FUN_0040c8f4(*(int **)(*(int *)(param_4 + -4) + 0x30),(int)piVar1);
  return;
}



void __cdecl FUN_0040fa3c(int param_1)

{
  byte bVar1;
  uint uVar2;
  int iVar3;
  int *piVar4;
  uint extraout_ECX;
  undefined4 in_EDX;
  undefined4 extraout_EDX;
  undefined4 *in_FS_OFFSET;
  longdouble in_ST0;
  undefined4 uStack_2c;
  undefined1 *puStack_28;
  undefined1 *puStack_24;
  OLECHAR *local_1c;
  undefined4 *local_18;
  undefined1 local_14 [16];
  
  puStack_24 = &stack0xfffffffc;
  local_14._8_8_ = (_union_2685)0x0;
  local_14._4_4_ = 0;
  local_14._0_4_ = 0;
  local_18 = (undefined4 *)0x0;
  local_1c = (OLECHAR *)0x0;
  puStack_28 = &LAB_0040fc27;
  uStack_2c = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_2c;
  bVar1 = FUN_0040ec38(*(int *)(param_1 + -4),in_EDX,0);
  switch(bVar1) {
  case 0:
  case 0xd:
    uVar2 = FUN_0041006c(*(int *)(param_1 + -4),extraout_EDX,extraout_ECX);
    if ((char)uVar2 == '\r') {
      FUN_0041c52c((VARIANTARG *)local_14);
    }
    else {
      FUN_0041c570((VARIANT *)local_14,(VARIANTARG *)PTR_DAT_00430660);
    }
    break;
  default:
    piVar4 = FUN_00416d50((int *)&DAT_0040bc00,'\x01',(undefined4 *)PTR_PTR_00430678);
    FUN_0041b508(piVar4);
    break;
  case 2:
    iVar3 = FUN_0040f604(*(int *)(param_1 + -4),extraout_EDX,extraout_ECX);
    local_14[8] = (char)iVar3;
    break;
  case 3:
    iVar3 = FUN_0040f604(*(int *)(param_1 + -4),extraout_EDX,extraout_ECX);
    local_14._8_2_ = (short)iVar3;
    break;
  case 4:
    iVar3 = FUN_0040f604(*(int *)(param_1 + -4),extraout_EDX,extraout_ECX);
    local_14._8_4_ = iVar3;
    break;
  case 5:
    FUN_0040f3f8(*(int *)(param_1 + -4),extraout_EDX,extraout_ECX);
    local_14._8_8_ = (undefined8)in_ST0;
    break;
  case 6:
  case 0xc:
    FUN_0040ffc4(*(int *)(param_1 + -4),(int *)&local_18,extraout_ECX);
    FUN_0041caac((VARIANTARG *)local_14,local_18);
    break;
  case 8:
  case 9:
    uVar2 = FUN_0041006c(*(int *)(param_1 + -4),extraout_EDX,extraout_ECX);
    local_14._8_2_ = -(ushort)((char)uVar2 == '\t');
    break;
  case 0xf:
    FUN_0040f438(*(int *)(param_1 + -4),extraout_EDX,extraout_ECX);
    local_14._8_4_ = (float)in_ST0;
    break;
  case 0x10:
    FUN_0040f478(*(int *)(param_1 + -4),extraout_EDX,extraout_ECX);
    local_14._8_8_ = (undefined8)ROUND(in_ST0);
    break;
  case 0x11:
    FUN_0040f4c4(*(int *)(param_1 + -4),extraout_EDX,extraout_ECX);
    local_14._8_8_ = (undefined8)in_ST0;
    break;
  case 0x12:
    FUN_00410024(*(int *)(param_1 + -4),(int *)&local_1c,extraout_ECX);
    FUN_0041caec((VARIANTARG *)local_14,local_1c);
  }
  local_14._0_2_ = *(undefined2 *)(&DAT_0042d3e8 + (uint)bVar1 * 4);
  FUN_0040b704(*(int **)(param_1 + -8),*(int *)(param_1 + -0xc),(VARIANTARG *)local_14);
  *in_FS_OFFSET = uStack_2c;
  puStack_24 = &LAB_0040fc2e;
  puStack_28 = (undefined1 *)0x40fc16;
  FUN_0041be04(&local_1c);
  puStack_28 = (undefined1 *)0x40fc1e;
  FUN_0041b840((int *)&local_18);
  puStack_28 = (undefined1 *)0x40fc26;
  FUN_0041cec4((VARIANTARG *)local_14);
  return;
}



void FUN_0040fc34(int *param_1,int *param_2,undefined4 *param_3)

{
  char *pcVar1;
  undefined1 *puVar2;
  byte bVar3;
  char cVar4;
  int *piVar5;
  int iVar6;
  undefined4 *extraout_ECX;
  uint extraout_ECX_00;
  uint extraout_ECX_01;
  uint extraout_ECX_02;
  undefined4 extraout_ECX_03;
  int *extraout_EDX;
  undefined4 extraout_EDX_00;
  undefined4 extraout_EDX_01;
  undefined4 extraout_EDX_02;
  undefined4 extraout_EDX_03;
  undefined4 *in_FS_OFFSET;
  undefined1 *puVar7;
  undefined4 uStack_3c;
  undefined1 *puStack_38;
  undefined1 *puStack_34;
  int local_2c;
  undefined4 *local_28;
  LPCSTR local_24;
  byte *local_20;
  int local_1c;
  int local_18;
  int local_14;
  undefined4 *local_10;
  int *local_c;
  int *local_8;
  
  puStack_34 = &stack0xfffffffc;
  puVar7 = &stack0xfffffffc;
  local_2c = 0;
  local_28 = (undefined4 *)0x0;
  local_24 = (LPCSTR)0x0;
  local_20 = (byte *)0x0;
  local_1c = 0;
  puStack_38 = &LAB_0040fec0;
  uStack_3c = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_3c;
  puVar2 = &stack0xfffffffc;
  local_10 = param_3;
  local_c = param_2;
  local_8 = param_1;
  if (param_3[2] == 0) {
    FUN_0040e9dc((undefined4 *)PTR_PTR_004306ac);
    param_3 = extraout_ECX;
    param_2 = extraout_EDX;
    puVar2 = puStack_34;
  }
  puStack_34 = puVar2;
  pcVar1 = *(char **)*local_10;
  switch(*pcVar1) {
  case '\x01':
    cVar4 = FUN_0040ec38((int)local_8,param_2,(uint)param_3);
    if (cVar4 == '\a') {
      FUN_0040f504((int)local_8,&local_1c,extraout_ECX_00);
      FUN_0040f9b4(local_c,local_10,local_1c);
    }
    else {
      iVar6 = FUN_0040f604((int)local_8,extraout_EDX_00,extraout_ECX_00);
      FUN_0040b448(local_c,local_10,iVar6);
    }
    break;
  case '\x02':
    bVar3 = FUN_0040ecdc((int)local_8,param_2,(uint)param_3);
    FUN_0040b448(local_c,local_10,(uint)bVar3);
    break;
  case '\x03':
    FUN_0040f504((int)local_8,(int *)&local_20,(uint)param_3);
    iVar6 = FUN_0040ea0c(pcVar1,local_20);
    FUN_0040b448(local_c,local_10,iVar6);
    break;
  case '\x04':
    FUN_0040f3f8((int)local_8,param_2,(uint)param_3);
    FUN_0040b658(local_c,local_10);
    break;
  case '\x05':
  case '\n':
  case '\v':
    FUN_0040ffc4((int)local_8,(int *)&local_24,(uint)param_3);
    FUN_0040b62c(local_c,local_10,local_24);
    break;
  case '\x06':
    iVar6 = FUN_0040fecc((int)local_8,(int)pcVar1);
    FUN_0040b448(local_c,local_10,iVar6);
    break;
  case '\a':
    cVar4 = FUN_0040ec38((int)local_8,param_2,(uint)param_3);
    if (cVar4 == '\r') {
      FUN_0041006c((int)local_8,extraout_EDX_01,extraout_ECX_01);
      FUN_0040b448(local_c,local_10,0);
    }
    else if (cVar4 == '\x0e') {
      FUN_0041006c((int)local_8,extraout_EDX_01,extraout_ECX_01);
      piVar5 = (int *)FUN_0040b3d4(local_c,local_10);
      FUN_0040ed60(local_8,piVar5);
    }
    else {
      FUN_0040f504((int)local_8,(int *)&local_28,extraout_ECX_01);
      FUN_0040f9fc((int)local_c,(int)local_10,local_28,(int)puVar7);
    }
    break;
  case '\b':
    cVar4 = FUN_0040ec38((int)local_8,param_2,(uint)param_3);
    if (cVar4 == '\r') {
      FUN_0041006c((int)local_8,extraout_EDX_02,extraout_ECX_02);
      FUN_0040b744(local_c,(int)local_10,(undefined4 *)&DAT_0042d3e0);
    }
    else {
      FUN_0040f504((int)local_8,&local_2c,extraout_ECX_02);
      local_18 = (**(code **)(*local_8 + 0x18))(local_8,local_8[6],local_2c);
      local_14 = local_8[6];
      if (local_18 != 0) {
        FUN_0040b744(local_c,(int)local_10,&local_18);
      }
    }
    break;
  case '\f':
    FUN_0040fa3c((int)&stack0xfffffffc);
    break;
  case '\x10':
    iVar6 = FUN_0040f670((int)local_8,param_2,(uint)param_3);
    FUN_0040b77c(local_c,(int)local_10,extraout_ECX_03,iVar6,extraout_EDX_03);
  }
  *in_FS_OFFSET = uStack_3c;
  puStack_34 = &LAB_0040fec7;
  puStack_38 = (undefined1 *)0x40febf;
  FUN_0041b864(&local_2c,5);
  return;
}



void FUN_0040fecc(int param_1,int param_2)

{
  char *pcVar1;
  uint uVar2;
  int iVar3;
  uint extraout_ECX;
  uint extraout_ECX_00;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_30;
  undefined1 *puStack_2c;
  undefined1 *puStack_28;
  undefined4 local_24;
  undefined1 *puStack_20;
  undefined1 *puStack_1c;
  byte *local_c;
  int local_8;
  
  puStack_1c = &stack0xfffffffc;
  puStack_28 = &stack0xfffffffc;
  local_c = (byte *)0x0;
  puStack_20 = &LAB_0040ff76;
  local_24 = *in_FS_OFFSET;
  *in_FS_OFFSET = &local_24;
  puStack_2c = &LAB_0040ff49;
  uStack_30 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_30;
  local_8 = param_1;
  uVar2 = FUN_0041006c(param_1,param_2,0);
  if ((char)uVar2 != '\v') {
    FUN_0040e9f4();
  }
  iVar3 = FUN_0040b2fc(param_2);
  pcVar1 = (char *)**(undefined4 **)(iVar3 + 1);
  uVar2 = extraout_ECX;
  while (FUN_0040ff88(local_8,(int *)&local_c,uVar2), local_c != (byte *)0x0) {
    FUN_0040ea0c(pcVar1,local_c);
    uVar2 = extraout_ECX_00;
  }
  *in_FS_OFFSET = uStack_30;
  *in_FS_OFFSET = local_24;
  puStack_1c = &LAB_0040ff7d;
  puStack_20 = (undefined1 *)0x40ff75;
  FUN_0041b840((int *)&local_c);
  return;
}



void FUN_0040ff88(int param_1,int *param_2,uint param_3)

{
  undefined4 *puVar1;
  uint local_c;
  
  local_c = param_3;
  FUN_0040ec54(param_1,&local_c,1);
  FUN_0041b928(param_2,(undefined4 *)0x0,local_c & 0xff);
  puVar1 = (undefined4 *)FUN_0041bc80(param_2);
  FUN_0040ec54(param_1,puVar1,local_c & 0xff);
  return;
}



void FUN_0040ffc4(int param_1,int *param_2,uint param_3)

{
  uint uVar1;
  uint local_c;
  
  local_c = 0;
  uVar1 = FUN_0041006c(param_1,param_2,param_3);
  if ((char)uVar1 == '\x06') {
    FUN_0040ec54(param_1,&local_c,1);
  }
  else if ((char)uVar1 == '\f') {
    FUN_0040ec54(param_1,&local_c,4);
  }
  else {
    FUN_0040e9f4();
  }
  FUN_0041b928(param_2,(undefined4 *)0x0,local_c);
  FUN_0040ec54(param_1,(undefined4 *)*param_2,local_c);
  return;
}



void FUN_00410024(int param_1,int *param_2,uint param_3)

{
  uint uVar1;
  UINT local_c;
  
  local_c = 0;
  uVar1 = FUN_0041006c(param_1,param_2,param_3);
  if ((char)uVar1 != '\x12') {
    FUN_0040e9f4();
  }
  FUN_0040ec54(param_1,&local_c,4);
  FUN_0041bfa0(param_2,local_c);
  FUN_0040ec54(param_1,(undefined4 *)*param_2,local_c * 2);
  return;
}



uint FUN_0041006c(int param_1,undefined4 param_2,uint param_3)

{
  undefined4 uVar1;
  uint local_4;
  
  local_4 = param_3;
  uVar1 = FUN_0040ec54(param_1,&local_4,1);
  return CONCAT31((int3)((uint)uVar1 >> 8),(undefined1)local_4);
}



void FUN_00410080(int param_1,undefined4 param_2,uint param_3)

{
  uint extraout_ECX;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_18;
  undefined1 *puStack_14;
  undefined1 *puStack_10;
  int local_8;
  
  puStack_10 = &stack0xfffffffc;
  local_8 = 0;
  puStack_14 = &LAB_004100bc;
  uStack_18 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_18;
  do {
    FUN_0040ff88(param_1,&local_8,param_3);
    param_3 = extraout_ECX;
  } while (local_8 != 0);
  *in_FS_OFFSET = uStack_18;
  puStack_10 = &LAB_004100c3;
  puStack_14 = (undefined1 *)0x4100bb;
  FUN_0041b840(&local_8);
  return;
}



void FUN_004100c8(undefined4 param_1,undefined4 param_2,uint param_3,int param_4)

{
  bool bVar1;
  uint extraout_ECX;
  uint extraout_ECX_00;
  undefined4 extraout_EDX;
  undefined4 extraout_EDX_00;
  int *piVar2;
  
  piVar2 = (int *)(param_4 + -4);
  while( true ) {
    bVar1 = FUN_0040ea48(*piVar2,param_2,param_3);
    if (bVar1) break;
    FUN_004101cc(*piVar2,extraout_EDX_00,extraout_ECX_00);
    param_3 = extraout_ECX;
    param_2 = extraout_EDX;
  }
  FUN_0040f6bc(*piVar2);
  return;
}



void FUN_004100f0(uint param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  undefined4 local_104 [64];
  
  while (0 < (int)param_1) {
    if ((int)param_1 < 0x101) {
      FUN_0040ec54(*(int *)(param_4 + -4),local_104,param_1);
      param_1 = 0;
    }
    else {
      FUN_0040ec54(*(int *)(param_4 + -4),local_104,0x100);
      param_1 = param_1 - 0x100;
    }
  }
  return;
}



void __cdecl FUN_00410144(int param_1)

{
  undefined4 extraout_ECX;
  undefined4 extraout_EDX;
  uint local_8;
  
  FUN_0040ec54(*(int *)(param_1 + -4),&local_8,4);
  FUN_004100f0(local_8,extraout_EDX,extraout_ECX,param_1);
  return;
}



void FUN_0041016c(undefined4 param_1,undefined4 param_2,uint param_3,uint param_4)

{
  char cVar1;
  bool bVar2;
  uint extraout_ECX;
  uint extraout_ECX_00;
  uint uVar3;
  uint extraout_ECX_01;
  uint uVar4;
  uint extraout_ECX_02;
  uint extraout_ECX_03;
  uint extraout_ECX_04;
  undefined4 extraout_EDX;
  undefined4 extraout_EDX_00;
  undefined4 extraout_EDX_01;
  undefined4 extraout_EDX_02;
  undefined4 uVar5;
  undefined4 extraout_EDX_03;
  undefined4 extraout_EDX_04;
  undefined4 extraout_EDX_05;
  int *piVar6;
  
  piVar6 = (int *)(param_4 - 4);
  while( true ) {
    bVar2 = FUN_0040ea48(*piVar6,param_2,param_3);
    if (bVar2) break;
    cVar1 = FUN_0040ec38(*piVar6,extraout_EDX_05,extraout_ECX_04);
    uVar3 = extraout_ECX;
    uVar5 = extraout_EDX;
    if ((byte)(cVar1 - 2U) < 3) {
      FUN_004101cc(*piVar6,extraout_EDX,extraout_ECX);
      uVar3 = extraout_ECX_00;
      uVar5 = extraout_EDX_00;
    }
    uVar4 = param_4;
    FUN_004100f0(1,uVar5,uVar3,param_4);
    uVar5 = extraout_EDX_01;
    while( true ) {
      bVar2 = FUN_0040ea48(*piVar6,uVar5,uVar4);
      if (bVar2) break;
      FUN_00410334(*piVar6,extraout_EDX_03,extraout_ECX_02);
      uVar4 = extraout_ECX_01;
      uVar5 = extraout_EDX_02;
    }
    FUN_0040f6bc(*piVar6);
    param_3 = extraout_ECX_03;
    param_2 = extraout_EDX_04;
  }
  FUN_0040f6bc(*piVar6);
  return;
}



void FUN_004101cc(int param_1,undefined4 param_2,uint param_3)

{
  uint uVar1;
  uint extraout_ECX;
  undefined4 extraout_EDX;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_18;
  undefined1 *puStack_14;
  undefined1 *puStack_10;
  int local_c;
  int local_8;
  
  puStack_10 = &stack0xfffffffc;
  local_c = 0;
  puStack_14 = &LAB_00410329;
  uStack_18 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_18;
  local_8 = param_1;
  uVar1 = FUN_0041006c(param_1,0,param_3);
  uVar1 = uVar1 & 0x7f;
  switch(uVar1) {
  case 1:
    FUN_004100c8(uVar1,extraout_EDX,extraout_ECX,(int)&stack0xfffffffc);
    break;
  case 2:
    FUN_004100f0(1,extraout_EDX,extraout_ECX,(int)&stack0xfffffffc);
    break;
  case 3:
    FUN_004100f0(2,extraout_EDX,extraout_ECX,(int)&stack0xfffffffc);
    break;
  case 4:
    FUN_004100f0(4,extraout_EDX,extraout_ECX,(int)&stack0xfffffffc);
    break;
  case 5:
    FUN_004100f0(10,extraout_EDX,extraout_ECX,(int)&stack0xfffffffc);
    break;
  case 6:
  case 7:
    FUN_0040ff88(local_8,&local_c,extraout_ECX);
    break;
  case 10:
    FUN_00410144((int)&stack0xfffffffc);
    break;
  case 0xb:
    FUN_00410080(local_8,extraout_EDX,extraout_ECX);
    break;
  case 0xc:
    FUN_00410144((int)&stack0xfffffffc);
    break;
  case 0xe:
    FUN_0041016c(uVar1,extraout_EDX,extraout_ECX,(uint)&stack0xfffffffc);
    break;
  case 0xf:
    FUN_004100f0(4,extraout_EDX,extraout_ECX,(int)&stack0xfffffffc);
    break;
  case 0x10:
    FUN_004100f0(8,extraout_EDX,extraout_ECX,(int)&stack0xfffffffc);
    break;
  case 0x11:
    FUN_004100f0(8,extraout_EDX,extraout_ECX,(int)&stack0xfffffffc);
    break;
  case 0x12:
    FUN_00410144((int)&stack0xfffffffc);
    break;
  case 0x13:
    FUN_004100f0(8,extraout_EDX,extraout_ECX,(int)&stack0xfffffffc);
  }
  *in_FS_OFFSET = uStack_18;
  puStack_10 = &LAB_00410330;
  puStack_14 = (undefined1 *)0x410328;
  FUN_0041b840(&local_c);
  return;
}



void FUN_00410334(int param_1,undefined4 param_2,uint param_3)

{
  uint extraout_ECX;
  undefined4 extraout_EDX;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_18;
  undefined1 *puStack_14;
  undefined1 *puStack_10;
  int local_8;
  
  puStack_10 = &stack0xfffffffc;
  local_8 = 0;
  puStack_14 = &LAB_00410371;
  uStack_18 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_18;
  FUN_0040ff88(param_1,&local_8,param_3);
  FUN_004101cc(param_1,extraout_EDX,extraout_ECX);
  *in_FS_OFFSET = uStack_18;
  puStack_10 = &LAB_00410378;
  puStack_14 = (undefined1 *)0x410370;
  FUN_0041b840(&local_8);
  return;
}



void FUN_0041037c(int *param_1,undefined4 param_2)

{
  undefined1 *puVar1;
  bool bVar2;
  uint uVar3;
  uint extraout_ECX;
  uint extraout_ECX_00;
  uint extraout_ECX_01;
  uint extraout_ECX_02;
  uint extraout_ECX_03;
  uint extraout_ECX_04;
  uint extraout_ECX_05;
  undefined4 extraout_EDX;
  undefined4 extraout_EDX_00;
  undefined4 extraout_EDX_01;
  undefined4 extraout_EDX_02;
  undefined4 extraout_EDX_03;
  undefined4 uVar4;
  undefined4 extraout_EDX_04;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_28;
  undefined1 *puStack_24;
  undefined1 *puStack_20;
  int local_14 [2];
  undefined1 local_c [7];
  undefined1 local_5;
  
  puStack_20 = &stack0xfffffffc;
  uVar3 = 0;
  local_14[0] = 0;
  local_14[1] = 0;
  puStack_24 = &LAB_00410414;
  uStack_28 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_28;
  puVar1 = &stack0xfffffffc;
  if ((char)param_2 != '\0') {
    (**(code **)(*param_1 + 0x24))(param_1,&local_5,local_c);
    FUN_0040ff88((int)param_1,local_14 + 1,extraout_ECX);
    FUN_0040ff88((int)param_1,local_14,extraout_ECX_00);
    uVar3 = extraout_ECX_01;
    param_2 = extraout_EDX;
    puVar1 = puStack_20;
  }
  while( true ) {
    puStack_20 = puVar1;
    bVar2 = FUN_0040ea48((int)param_1,param_2,uVar3);
    if (bVar2) break;
    FUN_00410334((int)param_1,extraout_EDX_01,extraout_ECX_03);
    uVar3 = extraout_ECX_02;
    param_2 = extraout_EDX_00;
    puVar1 = puStack_20;
  }
  FUN_0040f6bc(param_1);
  uVar3 = extraout_ECX_04;
  uVar4 = extraout_EDX_02;
  while( true ) {
    bVar2 = FUN_0040ea48((int)param_1,uVar4,uVar3);
    if (bVar2) break;
    FUN_0041037c(param_1,CONCAT31((int3)((uint)extraout_EDX_04 >> 8),1));
    uVar3 = extraout_ECX_05;
    uVar4 = extraout_EDX_03;
  }
  FUN_0040f6bc(param_1);
  *in_FS_OFFSET = uStack_28;
  puStack_20 = &LAB_0041041b;
  puStack_24 = (undefined1 *)0x410413;
  FUN_0041b864(local_14,2);
  return;
}



int FUN_00410424(int param_1,char *param_2,int param_3)

{
  undefined4 uVar1;
  int local_c;
  
  local_c = param_3;
  local_c = FUN_0040c6bc(*(undefined4 **)(param_1 + 0x18),param_2);
  if (((local_c == 0) && (*(int *)(param_1 + 0x1c) != 0)) &&
     (*(int *)(param_1 + 0x1c) != *(int *)(param_1 + 0x18))) {
    local_c = FUN_0040c6bc(*(undefined4 **)(param_1 + 0x1c),param_2);
  }
  if (*(short *)(param_1 + 0x62) != 0) {
    (**(code **)(param_1 + 0x60))(*(undefined4 *)(param_1 + 100),param_1,param_2,&local_c);
  }
  if ((local_c != 0) &&
     (uVar1 = thunk_FUN_0041b030(local_c,(int)&PTR_LAB_0040cf35_3_0040c358), (char)uVar1 != '\0')) {
    return local_c;
  }
  FUN_0040c5a0(param_2);
  return local_c;
}



void FUN_00410494(int param_1,undefined4 *param_2,uint param_3)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  undefined4 *puVar4;
  
  while (param_3 != 0) {
    uVar2 = *(uint *)(param_1 + 0xc) - *(uint *)(param_1 + 0x10);
    if (*(uint *)(param_1 + 0xc) < *(uint *)(param_1 + 0x10) || uVar2 == 0) {
      FUN_004104e0(param_1);
      uVar2 = *(uint *)(param_1 + 0xc);
    }
    if (param_3 <= uVar2) {
      uVar2 = param_3;
    }
    param_3 = param_3 - uVar2;
    iVar1 = *(int *)(param_1 + 0x10);
    *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + uVar2;
    puVar4 = (undefined4 *)(*(int *)(param_1 + 8) + iVar1);
    for (uVar3 = uVar2 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
      *puVar4 = *param_2;
      param_2 = param_2 + 1;
      puVar4 = puVar4 + 1;
    }
    for (uVar2 = uVar2 & 3; uVar2 != 0; uVar2 = uVar2 - 1) {
      *(undefined1 *)puVar4 = *(undefined1 *)param_2;
      param_2 = (undefined4 *)((int)param_2 + 1);
      puVar4 = (undefined4 *)((int)puVar4 + 1);
    }
  }
  return;
}



void FUN_004104e0(int param_1)

{
  FUN_0040e56c(*(int **)(param_1 + 4),*(undefined4 *)(param_1 + 8),*(int *)(param_1 + 0x10));
  *(undefined4 *)(param_1 + 0x10) = 0;
  return;
}



void FUN_004104f8(int param_1,int param_2,undefined4 param_3)

{
  int local_8;
  
  local_8 = param_2;
  if ((-0x81 < param_2) && (param_2 < 0x80)) {
    FUN_004105d8(param_1,2,param_3);
    FUN_00410494(param_1,&local_8,1);
    return;
  }
  if ((param_2 < -0x8000) || (0x7fff < param_2)) {
    FUN_004105d8(param_1,4,param_3);
    FUN_00410494(param_1,&local_8,4);
  }
  else {
    FUN_004105d8(param_1,3,param_3);
    FUN_00410494(param_1,&local_8,2);
  }
  return;
}



void FUN_0041056c(int param_1,undefined4 param_2,undefined4 param_3)

{
  FUN_004105d8(param_1,1,param_3);
  return;
}



void FUN_00410574(int param_1,undefined4 param_2,undefined4 param_3)

{
  FUN_004105d8(param_1,0,param_3);
  return;
}



void FUN_0041057c(int param_1,undefined4 *param_2,uint param_3)

{
  undefined4 extraout_ECX;
  uint local_c;
  
  local_c = param_3;
  local_c = FUN_0041bac0((int)param_2);
  if ((int)local_c < 0x100) {
    FUN_004105d8(param_1,6,extraout_ECX);
    FUN_00410494(param_1,&local_c,1);
  }
  else {
    FUN_004105d8(param_1,0xc,extraout_ECX);
    FUN_00410494(param_1,&local_c,4);
  }
  FUN_00410494(param_1,param_2,local_c);
  return;
}



void FUN_004105d8(int param_1,undefined1 param_2,undefined4 param_3)

{
  undefined1 local_4;
  undefined3 uStack_3;
  
  _local_4 = CONCAT31((int3)((uint)param_3 >> 8),param_2);
  FUN_00410494(param_1,(undefined4 *)&local_4,1);
  return;
}



void __stdcall FUN_004105ec(void)

{
  if (DAT_00430878 != (HWND)0x0) {
    DestroyWindow(DAT_00430878);
    DAT_00430878 = (HWND)0x0;
  }
  return;
}



int * FUN_00410608(int *param_1,char param_2,int *param_3)

{
  int *extraout_ECX;
  char extraout_DL;
  char cVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 in_stack_ffffffe4;
  undefined4 in_stack_ffffffe8;
  
  cVar1 = '\0';
  if (param_2 != '\0') {
    param_1 = (int *)FUN_0041b0d0((int)param_1,param_2,param_3,in_stack_ffffffe4,in_stack_ffffffe8);
    param_3 = extraout_ECX;
    cVar1 = extraout_DL;
  }
  *(undefined1 *)((int)param_1 + 0x22) = 1;
  if (param_3 != (int *)0x0) {
    FUN_0041077c(param_3,param_1,param_3);
  }
  if (cVar1 != '\0') {
    FUN_0041b128(param_1);
    *in_FS_OFFSET = in_stack_ffffffe4;
  }
  return param_1;
}



void FUN_00410654(int *param_1,char param_2)

{
  int *piVar1;
  int *piVar2;
  byte extraout_DL;
  int iVar3;
  
  piVar1 = FUN_0041b130(param_1,param_2);
  FUN_00410864((int)piVar1);
  if (piVar1[5] != 0) {
    iVar3 = *(int *)(piVar1[5] + 8) + -1;
    if (-1 < iVar3) {
      do {
        piVar2 = (int *)GetFieldDescriptor((undefined4 *)piVar1[5],iVar3);
        (**(code **)(*piVar2 + 0x10))(piVar2,piVar1,1);
        if (piVar1[5] == 0) break;
        iVar3 = iVar3 + -1;
      } while (iVar3 != -1);
    }
    FUN_0041ae54((int *)piVar1[5]);
    piVar1[5] = 0;
  }
  FUN_0041080c(piVar1);
  if ((int *)piVar1[1] != (int *)0x0) {
    FUN_004107d4((int *)piVar1[1],(int)piVar1);
  }
  FUN_0040ce28(piVar1,extraout_DL & 0xfc);
  if ('\0' < (char)extraout_DL) {
    FUN_0041b120(piVar1);
  }
  return;
}



void FUN_004106e8(int param_1,int param_2,uint param_3)

{
  int iVar1;
  
  iVar1 = FUN_0040f604(param_2,param_2,param_3);
  *(short *)(param_1 + 0x18) = (short)iVar1;
  return;
}



void FUN_004106fc(int param_1,int param_2,uint param_3)

{
  int iVar1;
  
  iVar1 = FUN_0040f604(param_2,param_2,param_3);
  *(short *)(param_1 + 0x1a) = (short)iVar1;
  return;
}



void FUN_00410728(int param_1,int param_2,undefined4 param_3)

{
  undefined4 uVar1;
  
  if (*(int *)(param_1 + 0x10) == 0) {
    uVar1 = FUN_0041ae24((int *)&PTR_LAB_0040be20,'\x01',param_3);
    *(undefined4 *)(param_1 + 0x10) = uVar1;
  }
  FUN_0040c8f4(*(int **)(param_1 + 0x10),param_2);
  *(int *)(param_2 + 4) = param_1;
  return;
}



void FUN_00410754(int param_1,int param_2)

{
  *(undefined4 *)(param_2 + 4) = 0;
  FUN_0040cbc8(*(int **)(param_1 + 0x10),param_2);
  if ((*(int **)(param_1 + 0x10))[2] == 0) {
    FUN_0041ae54(*(int **)(param_1 + 0x10));
    *(undefined4 *)(param_1 + 0x10) = 0;
  }
  return;
}



void FUN_0041077c(int *param_1,int *param_2,undefined4 param_3)

{
  undefined4 extraout_ECX;
  undefined4 extraout_EDX;
  
  FUN_0041b014(param_2,param_1,param_3);
  (**(code **)(*param_1 + 0x20))(param_1,param_2,0,param_2[2]);
  FUN_00410728((int)param_1,(int)param_2,extraout_ECX);
  FUN_00410c44((int)param_2,'\x01');
  if ((*(byte *)(param_1 + 8) & 0x10) != 0) {
    FUN_00410bf4((int)param_2,CONCAT31((int3)((uint)extraout_EDX >> 8),1),'\x01');
  }
  (**(code **)(*param_1 + 0x10))(param_1,param_2,0);
  return;
}



void FUN_004107d4(int *param_1,int param_2)

{
  (**(code **)(*param_1 + 0x20))(param_1,param_2,*(undefined4 *)(param_2 + 8),0);
  (**(code **)(*param_1 + 0x10))(param_1,param_2,1);
  FUN_00410c44(param_2,'\0');
  FUN_00410754((int)param_1,param_2);
  return;
}



void FUN_0041080c(int *param_1)

{
  int *piVar1;
  
  while ((undefined4 *)param_1[4] != (undefined4 *)0x0) {
    piVar1 = (int *)FUN_0040cb18((undefined4 *)param_1[4]);
    if (((*(byte *)((int)piVar1 + 0x21) & 1) == 0) && ((*(ushort *)(param_1 + 8) & 0x210) != 0x210))
    {
      FUN_00410754((int)param_1,(int)piVar1);
    }
    else {
      FUN_004107d4(param_1,(int)piVar1);
    }
    (**(code **)(*piVar1 + -4))(piVar1,1);
  }
  return;
}



void FUN_00410864(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  if ((*(byte *)(param_1 + 0x20) & 8) == 0) {
    *(ushort *)(param_1 + 0x20) = *(ushort *)(param_1 + 0x20) | 8;
    if ((*(int *)(param_1 + 0x10) != 0) &&
       (iVar2 = *(int *)(*(int *)(param_1 + 0x10) + 8), -1 < iVar2 + -1)) {
      iVar3 = 0;
      do {
        iVar1 = GetFieldDescriptor(*(undefined4 **)(param_1 + 0x10),iVar3);
        FUN_00410864(iVar1);
        iVar3 = iVar3 + 1;
        iVar2 = iVar2 + -1;
      } while (iVar2 != 0);
    }
  }
  return;
}



void FUN_004108a0(int param_1,int param_2)

{
  if (*(int **)(param_1 + 0x14) != (int *)0x0) {
    FUN_0040cbc8(*(int **)(param_1 + 0x14),param_2);
    if ((*(int **)(param_1 + 0x14))[2] == 0) {
      FUN_0041ae54(*(int **)(param_1 + 0x14));
      *(undefined4 *)(param_1 + 0x14) = 0;
    }
  }
  return;
}



void FUN_004108c4(int param_1,int param_2)

{
  FUN_004108a0(param_1,param_2);
  FUN_004108a0(param_2,param_1);
  return;
}



void FUN_004108e0(int param_1,int param_2,char param_3)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  
  if ((param_3 == '\x01') && (param_2 != 0)) {
    FUN_004108c4(param_1,param_2);
  }
  if ((*(int *)(param_1 + 0x10) != 0) &&
     (iVar2 = *(int *)(*(int *)(param_1 + 0x10) + 8), -1 < iVar2 + -1)) {
    iVar3 = 0;
    do {
      piVar1 = (int *)GetFieldDescriptor(*(undefined4 **)(param_1 + 0x10),iVar3);
      (**(code **)(*piVar1 + 0x10))(piVar1,param_2,param_3);
      iVar3 = iVar3 + 1;
      iVar2 = iVar2 + -1;
    } while (iVar2 != 0);
  }
  return;
}



void FUN_0041093c(int param_1,int *param_2)

{
  undefined4 local_8;
  
  local_8 = 0;
  if (param_2[8] != 0) {
    local_8 = *(undefined4 *)(param_2[8] + 0x18);
  }
  (**(code **)(*param_2 + 4))
            (param_2,&DAT_004109b0,*(short *)(param_1 + 0x18) != (short)local_8,&LAB_00410710,
             param_1,FUN_004106e8,param_1);
  (**(code **)(*param_2 + 4))
            (param_2,&DAT_004109c0,local_8._2_2_ != *(short *)(param_1 + 0x1a),&LAB_0041071c,param_1
             ,FUN_004106fc,param_1);
  return;
}



void __stdcall FUN_004109c8(void)

{
  return;
}



void FUN_004109d8(int param_1,int *param_2)

{
  FUN_0041b894(param_2,*(undefined4 **)(param_1 + 8));
  return;
}



void FUN_00410a30(int param_1,int param_2,char *param_3,char *param_4)

{
  undefined4 uVar1;
  int iVar2;
  char *local_10;
  undefined1 local_c;
  char *local_8;
  
  local_8 = param_3;
  if (param_2 != 0) {
    uVar1 = FUN_004140f8(param_3,param_4);
    if (((char)uVar1 == '\0') && (param_1 == *(int *)(param_2 + 4))) {
      iVar2 = FUN_00410ac8(param_1,param_4);
      if (iVar2 != 0) {
        local_10 = param_4;
        local_c = 0xb;
        uVar1 = FUN_00416d8c((int)&DAT_0040bdc4,'\x01',(undefined4 *)PTR_PTR_00430698,0,&local_10);
        FUN_0041b508(uVar1);
      }
    }
  }
  if (((*(byte *)(param_1 + 0x20) & 0x10) != 0) && (*(int *)(param_1 + 4) != 0)) {
    (**(code **)(**(int **)(param_1 + 4) + 0x20))(*(int **)(param_1 + 4),param_2,local_8,param_4);
  }
  return;
}



void FUN_00410ab0(undefined4 param_1,int *param_2,undefined4 param_3)

{
  FUN_0041b014(param_2,param_1,param_3);
  return;
}



int FUN_00410ac8(int param_1,char *param_2)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  
  if (((param_2 != (char *)0x0) && (*(int *)(param_1 + 0x10) != 0)) &&
     (iVar3 = *(int *)(*(int *)(param_1 + 0x10) + 8), -1 < iVar3 + -1)) {
    iVar4 = 0;
    do {
      iVar1 = GetFieldDescriptor(*(undefined4 **)(param_1 + 0x10),iVar4);
      uVar2 = FUN_004140f8(*(char **)(iVar1 + 8),param_2);
      if ((char)uVar2 != '\0') {
        return iVar1;
      }
      iVar4 = iVar4 + 1;
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
  }
  return 0;
}



void FUN_00410b18(int *param_1,uint *param_2)

{
  int *piVar1;
  undefined4 uVar2;
  bool bVar3;
  uint *local_14;
  undefined1 local_10;
  
  bVar3 = &stack0x00000000 == (undefined1 *)0x14;
  FUN_0041bbd0((uint *)param_1[2],param_2);
  if (!bVar3) {
    if (param_2 != (uint *)0x0) {
      uVar2 = FUN_004141a4((char *)param_2);
      if ((char)uVar2 == '\0') {
        local_10 = 0xb;
        local_14 = param_2;
        uVar2 = FUN_00416d8c((int)&DAT_0040bdc4,'\x01',(undefined4 *)PTR_PTR_0043069c,0,&local_14);
        FUN_0041b508(uVar2);
      }
    }
    piVar1 = (int *)param_1[1];
    if (piVar1 == (int *)0x0) {
      (**(code **)(*param_1 + 0x20))(param_1,0,param_1[2],param_2);
    }
    else {
      (**(code **)(*piVar1 + 0x20))(piVar1,param_1,param_1[2],param_2);
    }
    FUN_00410c44((int)param_1,'\0');
    FUN_00410ba4((int)param_1,param_2);
    FUN_00410c44((int)param_1,'\x01');
  }
  return;
}



void FUN_00410ba4(int param_1,undefined4 *param_2)

{
  FUN_0041b894((int *)(param_1 + 8),param_2);
  return;
}



void FUN_00410bb8(int param_1,int param_2)

{
  if (*(int *)(param_1 + 0x10) == 0) {
    FUN_0040c9dc(&PTR_LAB_0040be20,(undefined4 *)PTR_PTR_00430684,param_2);
  }
  GetFieldDescriptor(*(undefined4 **)(param_1 + 0x10),param_2);
  return;
}



undefined4 FUN_00410be4(int param_1)

{
  if (*(int *)(param_1 + 0x10) != 0) {
    return *(undefined4 *)(*(int *)(param_1 + 0x10) + 8);
  }
  return 0;
}



void FUN_00410bf4(int param_1,undefined4 param_2,char param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  if ((char)param_2 == '\0') {
    *(ushort *)(param_1 + 0x20) = *(ushort *)(param_1 + 0x20) & 0xffef;
  }
  else {
    *(ushort *)(param_1 + 0x20) = *(ushort *)(param_1 + 0x20) | 0x10;
  }
  if (param_3 != '\0') {
    iVar1 = FUN_00410be4(param_1);
    if (-1 < iVar1 + -1) {
      iVar3 = 0;
      do {
        iVar2 = FUN_00410bb8(param_1,iVar3);
        FUN_00410bf4(iVar2,param_2,'\x01');
        iVar3 = iVar3 + 1;
        iVar1 = iVar1 + -1;
      } while (iVar1 != 0);
    }
  }
  return;
}



void FUN_00410c44(int param_1,char param_2)

{
  int *piVar1;
  char acStack_10c [256];
  
  piVar1 = *(int **)(param_1 + 4);
  if (piVar1 != (int *)0x0) {
    FUN_0041ba9c(acStack_10c,*(undefined4 **)(param_1 + 8),0xff);
    piVar1 = (int *)FUN_0041b080(piVar1,acStack_10c);
    if (piVar1 != (int *)0x0) {
      if (param_2 == '\0') {
        *piVar1 = 0;
      }
      else {
        *piVar1 = param_1;
      }
    }
  }
  return;
}



undefined4 FUN_00410c8c(undefined4 param_1,int *param_2)

{
  char cVar1;
  undefined4 uVar2;
  
  cVar1 = (**(code **)(*param_2 + 0x38))(param_2,param_1);
  if (cVar1 != '\0') {
    uVar2 = (**(code **)(*param_2 + 0x40))(param_2,param_1);
    return CONCAT31((int3)((uint)uVar2 >> 8),1);
  }
  return 0;
}



undefined4 FUN_00410cb4(undefined4 param_1,int *param_2)

{
  char cVar1;
  undefined4 uVar2;
  
  cVar1 = (**(code **)(*param_2 + 0x38))(param_2,param_1);
  if (cVar1 != '\0') {
    uVar2 = (**(code **)(*param_2 + 0x3c))(param_2,param_1);
    return CONCAT31((int3)((uint)uVar2 >> 8),1);
  }
  return 0;
}



void __stdcall FUN_00410cf4(void)

{
  return;
}



undefined4 __stdcall FUN_00410cfc(int *param_1,int *param_2,int *param_3)

{
  char cVar1;
  undefined4 uVar2;
  
  if (param_1[7] == 0) {
    cVar1 = FUN_0041aee8(param_1,param_2,param_3);
    if (cVar1 == '\0') {
      uVar2 = 0x80004002;
    }
    else {
      uVar2 = 0;
    }
  }
  else {
    uVar2 = (*(code *)**(undefined4 **)param_1[7])();
  }
  return uVar2;
}



int * FUN_00410d3c(int *param_1,char param_2,undefined4 param_3)

{
  undefined4 extraout_ECX;
  char extraout_DL;
  char cVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 in_stack_ffffffe4;
  undefined4 in_stack_ffffffe8;
  
  cVar1 = '\0';
  if (param_2 != '\0') {
    param_1 = (int *)FUN_0041b0d0((int)param_1,param_2,param_3,in_stack_ffffffe4,in_stack_ffffffe8);
    param_3 = extraout_ECX;
    cVar1 = extraout_DL;
  }
  FUN_0041ae24(param_1,'\0',param_3);
  (**(code **)*param_1)(param_1,param_3);
  if (cVar1 != '\0') {
    FUN_0041b128(param_1);
    *in_FS_OFFSET = in_stack_ffffffe4;
  }
  return param_1;
}



void FUN_00410de0(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 + 0x10);
  if (param_2 != iVar1) {
    if (iVar1 != 0) {
      FUN_00410f6c(iVar1,param_1);
    }
    *(int *)(param_1 + 0x10) = param_2;
    if (param_2 != 0) {
      FUN_00410f60(param_2,param_1);
    }
  }
  return;
}



void __stdcall FUN_00410e10(void)

{
  return;
}



void FUN_00410e68(int *param_1,char param_2)

{
  int *piVar1;
  int iVar2;
  byte extraout_DL;
  
  piVar1 = FUN_0041b130(param_1,param_2);
  FUN_00410654(piVar1,extraout_DL & 0xfc);
  while (0 < (int)((undefined4 *)piVar1[0x10])[2]) {
    iVar2 = FUN_0040cb18((undefined4 *)piVar1[0x10]);
    FUN_00410f6c((int)piVar1,iVar2);
  }
  FUN_0041ae54((int *)piVar1[0x10]);
  if ('\0' < (char)extraout_DL) {
    FUN_0041b120(piVar1);
  }
  return;
}



void FUN_00410ef8(int *param_1,undefined4 param_2,undefined4 param_3,int param_4,int param_5)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  
  if (param_4 != param_1[0xc]) {
    iVar2 = *(int *)(param_1[0x10] + 8);
    if (-1 < iVar2 + -1) {
      iVar3 = 0;
      do {
        piVar1 = (int *)GetFieldDescriptor((undefined4 *)param_1[0x10],iVar3);
        (**(code **)(*piVar1 + 0x10))();
        iVar3 = iVar3 + 1;
        iVar2 = iVar2 + -1;
      } while (iVar2 != 0);
    }
    param_1[0xc] = param_4;
    param_1[0xd] = param_5;
    (**(code **)(*param_1 + 0x30))();
  }
  return;
}



void FUN_00410f60(int param_1,int param_2)

{
  *(int *)(param_2 + 0x10) = param_1;
  FUN_0040c8f4(*(int **)(param_1 + 0x40),param_2);
  return;
}



void FUN_00410f6c(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = *(int *)(*(int *)(param_1 + 0x40) + 8);
  if (-1 < iVar3 + -1) {
    iVar2 = 0;
    do {
      iVar1 = GetFieldDescriptor(*(undefined4 **)(param_1 + 0x40),iVar2);
      if (param_2 == iVar1) {
        *(undefined4 *)(param_2 + 0x10) = 0;
        FUN_0040c940(*(int **)(param_1 + 0x40),iVar2);
        return;
      }
      iVar2 = iVar2 + 1;
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
  }
  return;
}



void __stdcall FUN_00410fac(void)

{
  undefined4 *puVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_1c;
  undefined1 *puStack_18;
  undefined1 *puStack_14;
  
  puStack_14 = (undefined1 *)0x410fbc;
  puVar1 = (undefined4 *)FUN_0040cdf0(DAT_00430858);
  puStack_18 = &LAB_00411001;
  uStack_1c = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_1c;
  iVar3 = puVar1[2];
  if (-1 < iVar3 + -1) {
    iVar4 = 0;
    puStack_14 = &stack0xfffffffc;
    do {
      piVar2 = (int *)GetFieldDescriptor(puVar1,iVar4);
      FUN_0041ae54(piVar2);
      iVar4 = iVar4 + 1;
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
  }
  *in_FS_OFFSET = uStack_1c;
  puStack_14 = &LAB_00411008;
  puStack_18 = (undefined1 *)0x411000;
  FUN_0040ce04(DAT_00430858);
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __stdcall FUN_00411020(void)

{
  int iVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_10;
  undefined1 *puStack_c;
  undefined1 *puStack_8;
  
  puStack_8 = &stack0xfffffffc;
  puStack_c = &LAB_004110cc;
  uStack_10 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_10;
  _DAT_0043084c = _DAT_0043084c + 1;
  if (_DAT_0043084c == 0) {
    FUN_0040c788(DAT_004309d0);
    FUN_00417f34((int)DAT_00430844);
    FUN_00410fac();
    FUN_0041ae54(DAT_00430850);
    FUN_0041ae54(DAT_00430854);
    FUN_0040e8ac(0,(char *)0x0);
    FUN_0041ae54(DAT_0043085c);
    DAT_0043085c = (int *)0x0;
    iVar1 = FUN_00401202();
    FUN_0041ae54(*(int **)(iVar1 + 4));
    FUN_004105ec();
    FUN_0041ae54(DAT_00430844);
    DAT_00430844 = (int *)0x0;
    FUN_0041d170(0x411018);
    DeleteCriticalSection((LPCRITICAL_SECTION)&lpCriticalSection_00430860);
  }
  *in_FS_OFFSET = uStack_10;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __stdcall FUN_0041114c(void)

{
  undefined4 uVar1;
  undefined4 *in_FS_OFFSET;
  undefined1 auStack_10 [12];
  
  uVar1 = *in_FS_OFFSET;
  *in_FS_OFFSET = auStack_10;
  _DAT_00430880 = _DAT_00430880 + 1;
  *in_FS_OFFSET = uVar1;
  return;
}



bool FUN_004119b4(uint param_1)

{
  return (param_1 & 0x80000000) == 0;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __stdcall FUN_004119c0(void)

{
  undefined4 uVar1;
  undefined4 *in_FS_OFFSET;
  undefined1 auStack_10 [12];
  
  uVar1 = *in_FS_OFFSET;
  *in_FS_OFFSET = auStack_10;
  _DAT_00430884 = _DAT_00430884 + 1;
  *in_FS_OFFSET = uVar1;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __stdcall FUN_004119f8(void)

{
  undefined4 uVar1;
  undefined4 *in_FS_OFFSET;
  undefined1 auStack_10 [12];
  
  uVar1 = *in_FS_OFFSET;
  *in_FS_OFFSET = auStack_10;
  _DAT_00430888 = _DAT_00430888 + 1;
  *in_FS_OFFSET = uVar1;
  return;
}



void FUN_00411bdc(undefined4 *param_1,int *param_2)

{
  byte *pbVar1;
  undefined *puVar2;
  int iVar3;
  
  FUN_0041b894(param_2,param_1);
  pbVar1 = FUN_0041435c((undefined *)*param_2);
  while( true ) {
    iVar3 = FUN_0041bac0(*param_2);
    if ((iVar3 < 1) || ((0x20 < *pbVar1 && (*pbVar1 != 0x2e)))) break;
    puVar2 = FUN_0041bc74((undefined *)*param_2);
    FUN_0041bd88(param_2,(int)pbVar1 - (int)puVar2);
    pbVar1 = FUN_0041435c((undefined *)*param_2);
  }
  return;
}



void FUN_00411c2c(int *param_1,char param_2,int param_3,int param_4,DWORD param_5)

{
  int extraout_ECX;
  undefined1 extraout_DL;
  undefined4 *in_FS_OFFSET;
  undefined4 uVar1;
  undefined4 uStack_40;
  undefined1 *puStack_3c;
  undefined1 *puStack_38;
  undefined4 in_stack_ffffffcc;
  undefined4 in_stack_ffffffd0;
  DWORD local_18;
  undefined1 local_14;
  byte *local_10;
  undefined4 *local_c;
  undefined1 local_5;
  
  local_10 = (byte *)0x0;
  local_c = (undefined4 *)0x0;
  local_5 = 0;
  if (param_2 != '\0') {
    puStack_38 = (undefined1 *)0x411c49;
    param_1 = (int *)FUN_0041b0d0((int)param_1,param_2,param_3,in_stack_ffffffcc,in_stack_ffffffd0);
    param_3 = extraout_ECX;
    local_5 = extraout_DL;
  }
  puStack_3c = &LAB_00411cd3;
  uStack_40 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_40;
  puStack_38 = &stack0xfffffffc;
  FUN_0041b8d8((int *)&local_c,param_3);
  if (local_c == (undefined4 *)0x0) {
    FUN_004164cc(param_5,(int *)&local_c);
    if (local_c == (undefined4 *)0x0) {
      uVar1 = 0;
      FUN_0041d2c4((undefined4 *)PTR_PTR_00430828,(int *)&local_10);
      local_18 = param_5;
      local_14 = 0;
      FUN_00414ad4((int *)&local_c,local_10,&local_18,uVar1);
    }
  }
  FUN_00416e20(param_1,'\0',local_c,param_4);
  param_1[3] = param_5;
  *in_FS_OFFSET = uStack_40;
  puStack_38 = &LAB_00411cda;
  puStack_3c = (undefined1 *)0x411cd2;
  FUN_0041b864((int *)&local_10,2);
  return;
}



void FUN_00411cfc(int *param_1,char param_2,undefined4 *param_3,int param_4,undefined4 *param_5,
                 undefined4 *param_6,DWORD param_7)

{
  undefined4 *extraout_ECX;
  int *in_FS_OFFSET;
  undefined1 *puStack_30;
  undefined1 *puStack_2c;
  undefined1 *puStack_28;
  undefined4 in_stack_ffffffdc;
  undefined4 in_stack_ffffffe0;
  int local_8;
  
  local_8 = 0;
  if (param_2 != '\0') {
    puStack_28 = (undefined1 *)0x411d10;
    param_1 = (int *)FUN_0041b0d0((int)param_1,param_2,param_3,in_stack_ffffffdc,in_stack_ffffffe0);
    param_3 = extraout_ECX;
  }
  puStack_2c = &LAB_00411d6e;
  puStack_30 = (undefined1 *)*in_FS_OFFSET;
  *in_FS_OFFSET = (int)&puStack_30;
  puStack_28 = &stack0xfffffffc;
  FUN_00411bdc(param_3,&local_8);
  FUN_00411c2c(param_1,'\0',local_8,param_4,param_7);
  FUN_0041b894(param_1 + 4,param_6);
  FUN_0041b894(param_1 + 5,param_5);
  *in_FS_OFFSET = param_4;
  puStack_30 = &LAB_00411d75;
  FUN_0041b840(&local_8);
  return;
}



void FUN_00411d94(DWORD param_1)

{
  undefined4 uVar1;
  
  uVar1 = FUN_00411c2c((int *)&DAT_00411b24,'\x01',0,0,param_1);
  FUN_0041b508(uVar1);
  return;
}



void FUN_00411db0(uint param_1)

{
  bool bVar1;
  
  bVar1 = FUN_004119b4(param_1);
  if (!bVar1) {
    FUN_00411d94(param_1);
  }
  return;
}



void FUN_00411fcc(int *param_1,char *param_2,undefined4 *param_3,int param_4,uint *param_5)

{
  uint *puVar1;
  int *piVar2;
  byte bVar3;
  BSTR pOVar4;
  DWORD DVar5;
  uint uVar6;
  int iVar7;
  uint *puVar8;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_678;
  undefined1 *puStack_674;
  undefined1 *puStack_670;
  undefined4 uStack_66c;
  undefined1 *puStack_668;
  undefined1 *puStack_664;
  uint local_654 [256];
  uint auStack_254 [128];
  VARIANTARG local_54 [2];
  uint *local_34;
  undefined4 *local_30;
  uint local_2c;
  uint local_28;
  uint *local_24;
  uint *local_20;
  uint *local_1c;
  int local_18;
  uint local_14;
  uint local_10;
  undefined4 *local_c;
  int *local_8;
  uint *puVar9;
  
  puStack_664 = (undefined1 *)0x411fee;
  local_c = param_3;
  local_8 = param_1;
  FUN_0041bfe4((int)local_54,(int)&DAT_0041148c);
  puStack_668 = &LAB_004122b8;
  uStack_66c = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_66c;
  local_18 = 0;
  puStack_674 = &LAB_00412295;
  uStack_678 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_678;
  local_14 = (uint)(byte)param_2[1];
  puStack_670 = &stack0xfffffffc;
  puStack_664 = &stack0xfffffffc;
  if (local_14 != 0) {
    local_1c = param_5;
    uVar6 = 0;
    puVar8 = local_654 + local_14 * 4;
    puStack_670 = &stack0xfffffffc;
    puStack_664 = &stack0xfffffffc;
    do {
      puVar1 = local_1c;
      puVar9 = puVar8 + -4;
      local_10 = (uint)(param_2[uVar6 + 3] & 0x7f);
      bVar3 = param_2[uVar6 + 3] & 0x80;
      if (local_10 == 10) {
        *puVar9 = 10;
        puVar8[-2] = 0x80020004;
      }
      else {
        if (local_10 == 0x48) {
          local_20 = auStack_254 + local_18 * 2;
          if (bVar3 == 0) {
            pOVar4 = FUN_0041c490((LPCSTR)*local_1c);
            *local_20 = (uint)pOVar4;
            local_20[1] = 0;
            *puVar9 = 8;
            puVar8[-2] = *local_20;
          }
          else {
            pOVar4 = FUN_0041c490(*(LPCSTR *)*local_1c);
            *local_20 = (uint)pOVar4;
            local_20[1] = *local_1c;
            *puVar9 = 0x4008;
            puVar8[-2] = (uint)local_20;
          }
          local_18 = local_18 + 1;
        }
        else if (bVar3 == 0) {
          if (local_10 == 0xc) {
            if ((short)*local_1c == 0x100) {
              local_24 = auStack_254 + local_18 * 2;
              pOVar4 = FUN_0041c490(*(LPCSTR *)(*local_1c + 8));
              *local_24 = (uint)pOVar4;
              local_24[1] = 0;
              *puVar9 = 8;
              puVar8[-2] = *local_24;
              local_18 = local_18 + 1;
            }
            else {
              *puVar9 = *local_1c;
              puVar8[-3] = puVar1[1];
              puVar8[-2] = puVar1[2];
              puVar8[-1] = puVar1[3];
              local_1c = local_1c + 3;
            }
          }
          else {
            *puVar9 = local_10;
            puVar8[-2] = *local_1c;
            if ((4 < (int)local_10) && ((int)local_10 < 8)) {
              local_1c = local_1c + 1;
              puVar8[-1] = *local_1c;
            }
          }
        }
        else {
          if ((local_10 == 0xc) && (*(short *)*local_1c == 0x100)) {
            FUN_0041c784((VARIANT *)*local_1c,(VARIANTARG *)*local_1c,8);
          }
          *puVar9 = local_10 | 0x4000;
          puVar8[-2] = *local_1c;
        }
        local_1c = local_1c + 1;
      }
      uVar6 = uVar6 + 1;
      puVar8 = puVar9;
    } while (uVar6 != local_14);
  }
  piVar2 = local_8;
  local_34 = local_654;
  local_30 = local_c + 1;
  local_2c = local_14;
  local_28 = (uint)(byte)param_2[2];
  if (*param_2 == '\x04') {
    *local_c = 0xfffffffd;
    local_30 = local_c;
    local_28 = local_28 + 1;
  }
  DVar5 = (**(code **)(*local_8 + 0x18))();
  iVar7 = local_18;
  if (DVar5 != 0) {
    FUN_00412548(DVar5,local_54);
    iVar7 = local_18;
  }
  while (iVar7 != 0) {
    iVar7 = iVar7 + -1;
    if ((int *)auStack_254[iVar7 * 2 + 1] != (int *)0x0) {
      FUN_0041c46c((LPCWSTR)auStack_254[iVar7 * 2],(int *)auStack_254[iVar7 * 2 + 1]);
    }
  }
  *in_FS_OFFSET = piVar2;
  iVar7 = local_18;
  while (iVar7 != 0) {
    iVar7 = iVar7 + -1;
    SysFreeString((BSTR)auStack_254[iVar7 * 2]);
  }
  return;
}



void __cdecl FUN_004122c8(int param_1)

{
  undefined4 uVar1;
  undefined4 local_c;
  undefined1 local_8;
  
  local_c = *(undefined4 *)(param_1 + -4);
  local_8 = 6;
  uVar1 = FUN_00416d8c((int)&DAT_00411acc,'\x01',(undefined4 *)PTR_PTR_0043082c,0,&local_c);
  FUN_0041b508(uVar1);
  return;
}



// WARNING: Unable to track spacebase fully for stack

void FUN_004122fc(int *param_1,char *param_2,int param_3,undefined4 param_4)

{
  code *pcVar1;
  int iVar2;
  int iVar3;
  undefined1 *puVar4;
  int iVar5;
  int iVar6;
  LCID LVar7;
  uint uVar8;
  undefined1 *puVar9;
  undefined1 *puVar10;
  int iVar11;
  
  iVar11 = 0;
  iVar2 = (param_3 + 1) * -4;
  puVar4 = &stack0xffffffd4 + iVar2;
  do {
    puVar9 = puVar4;
    *(undefined4 *)(puVar9 + -4) = 0x41232b;
    iVar5 = FUN_00414488(param_2);
    *(undefined4 *)(puVar9 + -4) = 0;
    *(undefined4 *)(puVar9 + -8) = 0;
    *(int *)(puVar9 + -0xc) = iVar5;
    *(char **)(puVar9 + -0x10) = param_2;
    *(undefined4 *)(puVar9 + -0x14) = 0;
    *(undefined4 *)(puVar9 + -0x18) = 0;
    *(undefined4 *)(puVar9 + -0x1c) = 0x41233c;
    iVar6 = MultiByteToWideChar(*(UINT *)(puVar9 + -0x18),*(DWORD *)(puVar9 + -0x14),
                                *(LPCSTR *)(puVar9 + -0x10),*(int *)(puVar9 + -0xc),
                                *(LPWSTR *)(puVar9 + -8),*(int *)(puVar9 + -4));
    iVar6 = iVar6 + 1;
    iVar3 = -(iVar6 * 2 + 3U & 0xfffffffc);
    if (iVar11 == 0) {
      *(int *)(&stack0xffffffd4 + iVar2) = (int)puVar9 + iVar3;
    }
    else {
      *(int *)(&stack0xffffffd4 + (param_3 - iVar11) * 4 + iVar2) = (int)puVar9 + iVar3;
    }
    *(int *)(puVar9 + iVar3 + -4) = iVar6;
    *(int *)(puVar9 + iVar3 + -8) = (int)puVar9 + iVar3;
    *(int *)(puVar9 + iVar3 + -0xc) = iVar5;
    *(char **)(puVar9 + iVar3 + -0x10) = param_2;
    *(undefined4 *)(puVar9 + iVar3 + -0x14) = 0;
    *(undefined4 *)(puVar9 + iVar3 + -0x18) = 0;
    *(undefined4 *)(puVar9 + iVar3 + -0x1c) = 0x412381;
    MultiByteToWideChar(*(UINT *)(puVar9 + iVar3 + -0x18),*(DWORD *)(puVar9 + iVar3 + -0x14),
                        *(LPCSTR *)(puVar9 + iVar3 + -0x10),*(int *)(puVar9 + iVar3 + -0xc),
                        *(LPWSTR *)(puVar9 + iVar3 + -8),*(int *)(puVar9 + iVar3 + -4));
    *(undefined2 *)(puVar9 + iVar6 * 2 + iVar3 + -2) = 0;
    param_2 = param_2 + iVar5 + 1;
    iVar11 = iVar11 + 1;
    puVar4 = puVar9 + iVar3;
  } while (iVar11 != param_3);
  *(undefined4 *)(puVar9 + iVar3 + -4) = param_4;
  *(undefined4 *)(puVar9 + iVar3 + -8) = 0x4123a0;
  LVar7 = GetThreadLocale();
  *(LCID *)(puVar9 + iVar3 + -8) = LVar7;
  *(int *)(puVar9 + iVar3 + -0xc) = param_3;
  *(undefined1 **)(puVar9 + iVar3 + -0x10) = &stack0xffffffd4 + iVar2;
  *(undefined **)(puVar9 + iVar3 + -0x14) = PTR_DAT_004306c0;
  *(int **)(puVar9 + iVar3 + -0x18) = param_1;
  pcVar1 = *(code **)(*param_1 + 0x14);
  puVar10 = puVar9 + iVar3 + -0x1c;
  *(undefined4 *)(puVar9 + iVar3 + -0x1c) = 0x4123b8;
  uVar8 = (*pcVar1)();
  if (uVar8 == 0x80020006) {
    *(undefined1 **)(puVar10 + -4) = &stack0xfffffffc;
    *(undefined4 *)(puVar10 + -8) = 0x4123c5;
    FUN_004122c8(*(int *)(puVar10 + -4));
  }
  else {
    *(undefined4 *)(puVar10 + -4) = 0x4123cd;
    FUN_00411db0(uVar8);
  }
  return;
}



void __stdcall FUN_004123dc(void)

{
  int *piVar1;
  
  piVar1 = FUN_00416d50((int *)&DAT_00411acc,'\x01',(undefined4 *)PTR_LAB_00430830);
  FUN_0041b508(piVar1);
  return;
}



void __cdecl FUN_004123f4(VARIANTARG *param_1,short *param_2,char *param_3)

{
  undefined4 local_108 [64];
  int *local_8;
  
  if (*param_2 == 9) {
    local_8 = *(int **)(param_2 + 4);
  }
  else if (*param_2 == 0x4009) {
    local_8 = (int *)**(undefined4 **)(param_2 + 4);
  }
  else {
    FUN_004123dc();
  }
  FUN_004122fc(local_8,param_3 + (byte)param_3[1] + 3,(byte)param_3[2] + 1,local_108);
  if (param_1 != (VARIANTARG *)0x0) {
    FUN_0041c52c(param_1);
  }
  FUN_00411fcc(local_8,param_3,local_108,param_1,(uint *)&stack0x00000010);
  return;
}



void FUN_00412474(DWORD param_1,VARIANTARG *param_2,int param_3,char param_4)

{
  undefined4 uVar1;
  undefined4 *in_FS_OFFSET;
  int iVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  DWORD DVar5;
  undefined1 *puStack_28;
  undefined1 *puStack_24;
  undefined1 *puStack_20;
  undefined4 *local_14;
  undefined4 *local_10;
  undefined4 *local_c;
  int local_8;
  
  puStack_20 = &stack0xfffffffc;
  local_c = (undefined4 *)0x0;
  local_10 = (undefined4 *)0x0;
  local_14 = (undefined4 *)0x0;
  puStack_24 = &LAB_00412539;
  puStack_28 = (undefined1 *)*in_FS_OFFSET;
  *in_FS_OFFSET = &puStack_28;
  local_8 = param_3;
  if (param_1 == 0x80020009) {
    DVar5 = *(DWORD *)((int)&param_2[1].n1 + 0xc);
    puStack_20 = &stack0xfffffffc;
    FUN_0041ba88((int *)&local_c,(LPCWSTR)(param_2->n1).decVal.Hi32);
    puVar4 = local_c;
    FUN_0041ba88((int *)&local_10,*(LPCWSTR *)((int)&param_2->n1 + 0xc));
    iVar2 = *(int *)&param_2[1].n1;
    puVar3 = local_10;
    FUN_0041ba88((int *)&local_14,*(LPCWSTR *)((int)&param_2->n1 + 8));
    uVar1 = FUN_00411cfc((int *)&DAT_00411b80,'\x01',local_14,iVar2,puVar3,puVar4,DVar5);
    if (param_4 != '\0') {
      FUN_0041c1c4(param_2,"\x0e\ftagEXCEPINFO ");
    }
  }
  else {
    puVar4 = (undefined4 *)0x0;
    uVar1 = FUN_00411c2c((int *)&DAT_00411b24,'\x01',0,0,param_1);
  }
  if (local_8 != 0) {
    FUN_0041b508(uVar1);
    return;
  }
  FUN_0041b508(uVar1);
  *in_FS_OFFSET = puVar4;
  puStack_28 = &LAB_00412540;
  FUN_0041b864((int *)&local_14,3);
  return;
}



void FUN_00412548(DWORD param_1,VARIANTARG *param_2)

{
  FUN_00412474(param_1,param_2,0,'\0');
  return;
}



void FUN_00412554(undefined4 *param_1)

{
  FUN_0041abb0(param_1,0x20,0);
  return;
}



void __stdcall FUN_00412564(int *param_1,char *param_2)

{
  DWORD DVar1;
  uint uVar2;
  int in_stack_00000000;
  VARIANTARG local_34 [2];
  undefined1 *local_14;
  undefined1 *local_10;
  uint local_c;
  uint local_8;
  
  local_c = (uint)(byte)param_2[1];
  for (uVar2 = local_c; uVar2 != 0; uVar2 = uVar2 - 1) {
  }
  local_14 = &stack0xffffffc0;
  local_8 = (uint)(byte)param_2[2];
  for (uVar2 = local_8; uVar2 != 0; uVar2 = uVar2 - 1) {
  }
  if (*param_2 == '\x04') {
    local_8 = local_8 + 1;
  }
  local_10 = &stack0xffffffc0;
  FUN_00412554((undefined4 *)local_34);
  DVar1 = (**(code **)(*param_1 + 0x18))();
  if (DVar1 != 0) {
    FUN_00412474(DVar1,local_34,in_stack_00000000,(char)param_1);
    return;
  }
  return;
}



void __stdcall FUN_0041264c(undefined4 param_1,int *param_2,int param_3)

{
  FUN_00412564(param_2,(char *)(param_3 + 5));
                    // WARNING: Could not recover jumptable at 0x00412676. Too many branches
                    // WARNING: Treating indirect jump as call
  (**(code **)(&DAT_0041267d + (uint)*(byte *)(param_3 + 4) * 4))();
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __stdcall FUN_004127a0(void)

{
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_10;
  undefined1 *puStack_c;
  undefined1 *puStack_8;
  
  puStack_8 = &stack0xfffffffc;
  puStack_c = &LAB_00412816;
  uStack_10 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_10;
  _DAT_00430890 = _DAT_00430890 + 1;
  if (_DAT_00430890 == 0) {
    DAT_00430894 = 1;
    FUN_0041ae54(DAT_00430898);
    *(undefined4 *)PTR_DAT_004306c4 = 0;
    *(undefined4 *)PTR_PTR_004306c8 = 0;
    *(undefined4 *)PTR_PTR_00430664 = 0;
    if (DAT_004308a0 != '\0') {
      CoUninitialize();
    }
    FUN_0041c0dc((VARIANTARG *)&DAT_0042d464,&LAB_004184a8,4);
  }
  *in_FS_OFFSET = uStack_10;
  return;
}



int FUN_00412cae(undefined1 *param_1,undefined4 param_2,char param_3,undefined4 param_4,int param_5,
                byte param_6)

{
  int iVar1;
  int extraout_ECX;
  uint uVar2;
  undefined *puVar3;
  undefined1 *puVar4;
  ushort local_20;
  
  iVar1 = 0x13;
  if (param_3 == '\0') {
    iVar1 = param_5;
    if (param_5 < 2) {
      iVar1 = 2;
    }
    if (0x12 < iVar1) {
      iVar1 = 0x12;
    }
  }
  FUN_00413251();
  puVar4 = param_1;
  if (local_20 - 0x7fff < 2) {
    FUN_00412d6c();
    puVar3 = &DAT_00412ca8 + extraout_ECX * 3;
    for (iVar1 = 3; iVar1 != 0; iVar1 = iVar1 + -1) {
      *puVar4 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar4 = puVar4 + 1;
    }
  }
  else {
    uVar2 = (uint)param_6;
    if ((param_6 != 1) && ((4 < param_6 || (iVar1 < (short)local_20)))) {
      uVar2 = 0;
    }
    (**(code **)(&DAT_00412d4f + uVar2 * 4))();
  }
  return (int)puVar4 - (int)param_1;
}



void __stdcall FUN_00412d6c(void)

{
  int unaff_EBP;
  undefined1 *unaff_EDI;
  
  if (*(char *)(unaff_EBP + -0x1a) != '\0') {
    *unaff_EDI = 0x2d;
  }
  return;
}



uint FUN_00412e0c(undefined4 param_1,uint param_2,int param_3)

{
  ulonglong uVar1;
  char cVar2;
  char unaff_BL;
  uint *puVar3;
  undefined1 *unaff_EDI;
  char *pcVar4;
  uint local_4;
  
  *unaff_EDI = (char)param_1;
  if (unaff_BL == '\0') {
    param_2 = 0;
LAB_00412e1f:
    cVar2 = (char)((uint)param_1 >> 8);
    pcVar4 = unaff_EDI + 1;
    if (cVar2 == '\0') goto LAB_00412e26;
  }
  else {
    if (-1 < (int)param_2) goto LAB_00412e1f;
    cVar2 = '-';
    param_2 = -param_2;
  }
  pcVar4 = unaff_EDI + 2;
  unaff_EDI[1] = cVar2;
LAB_00412e26:
  puVar3 = &local_4;
  local_4 = param_2;
  do {
    do {
      uVar1 = (ulonglong)param_2;
      param_2 = param_2 / 10;
      *(char *)puVar3 = (char)(uVar1 % 10) + '0';
      puVar3 = (uint *)((int)puVar3 + 1);
      param_3 = param_3 + -1;
    } while (param_2 != 0);
  } while (0 < param_3);
  do {
    puVar3 = (uint *)((int)puVar3 + -1);
    *pcVar4 = *(char *)puVar3;
    pcVar4 = pcVar4 + 1;
  } while (puVar3 != &local_4);
  return local_4;
}



void __stdcall FUN_0041304a(void)

{
  char cVar1;
  char cVar2;
  int in_ECX;
  int unaff_EBP;
  char *pcVar3;
  char *pcVar4;
  
  pcVar4 = *(char **)(unaff_EBP + 8);
  do {
    pcVar3 = pcVar4;
    if (in_ECX == 0) {
      return;
    }
    do {
      while( true ) {
        pcVar4 = pcVar3 + 1;
        cVar1 = *pcVar3;
        if ((cVar1 != '\'') && (cVar1 != '\"')) break;
        while( true ) {
          pcVar3 = pcVar4 + 1;
          cVar2 = *pcVar4;
          if (cVar2 == cVar1) break;
          pcVar4 = pcVar3;
          if (cVar2 == '\0') {
            return;
          }
        }
      }
      if (cVar1 == '\0') {
        return;
      }
      pcVar3 = pcVar4;
    } while (cVar1 != ';');
    in_ECX = in_ECX + -1;
  } while( true );
}



void __stdcall FUN_00413085(void)

{
  char cVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int unaff_EBP;
  char *unaff_ESI;
  char *pcVar6;
  
  *(char **)(unaff_EBP + -0x10) = unaff_ESI;
  iVar5 = 0x7fff;
  iVar4 = 0;
  *(undefined4 *)(unaff_EBP + -0x18) = 0xffffffff;
  *(undefined1 *)(unaff_EBP + -8) = 0;
  *(undefined1 *)(unaff_EBP + -0xc) = 0;
  iVar3 = 0;
LAB_0041309f:
  do {
    cVar2 = *unaff_ESI;
    unaff_ESI = unaff_ESI + 1;
    while (cVar2 != '#') {
      if (cVar2 == '0') {
        if (iVar3 < iVar5) {
          iVar5 = iVar3;
        }
        iVar4 = iVar3 + 1;
        iVar3 = iVar4;
        goto LAB_0041309f;
      }
      if (cVar2 == '.') {
        if (*(int *)(unaff_EBP + -0x18) == -1) {
          *(int *)(unaff_EBP + -0x18) = iVar3;
        }
        goto LAB_0041309f;
      }
      if (cVar2 == ',') {
        *(undefined1 *)(unaff_EBP + -8) = 1;
        goto LAB_0041309f;
      }
      pcVar6 = unaff_ESI;
      if ((cVar2 == '\'') || (cVar2 == '\"')) goto LAB_004130eb;
      if ((cVar2 != 'E') && (cVar2 != 'e')) {
        if ((cVar2 == ';') || (cVar2 == '\0')) goto LAB_0041310a;
        goto LAB_0041309f;
      }
      pcVar6 = unaff_ESI + 1;
      cVar2 = *unaff_ESI;
      if ((cVar2 == '-') || (unaff_ESI = pcVar6, cVar2 == '+')) {
        *(undefined1 *)(unaff_EBP + -0xc) = 1;
        do {
          unaff_ESI = pcVar6 + 1;
          cVar2 = *pcVar6;
          pcVar6 = unaff_ESI;
        } while (cVar2 == '0');
      }
    }
    iVar3 = iVar3 + 1;
  } while( true );
LAB_004130eb:
  unaff_ESI = pcVar6 + 1;
  cVar1 = *pcVar6;
  if (cVar1 == cVar2) goto LAB_0041309f;
  pcVar6 = unaff_ESI;
  if (cVar1 == '\0') {
LAB_0041310a:
    *(int *)(unaff_EBP + -0x14) = iVar3;
    if (*(int *)(unaff_EBP + -0x18) == -1) {
      *(int *)(unaff_EBP + -0x18) = iVar3;
    }
    iVar3 = *(int *)(unaff_EBP + -0x18) - iVar4;
    if (iVar3 != 0 && iVar4 <= *(int *)(unaff_EBP + -0x18)) {
      iVar3 = 0;
    }
    *(int *)(unaff_EBP + -0x20) = iVar3;
    iVar3 = *(int *)(unaff_EBP + -0x18) - iVar5;
    if (*(int *)(unaff_EBP + -0x18) < iVar5) {
      iVar3 = 0;
    }
    *(int *)(unaff_EBP + -0x1c) = iVar3;
    return;
  }
  goto LAB_004130eb;
}



int __stdcall FUN_00413130(void)

{
  byte bVar1;
  byte bVar2;
  undefined3 uVar4;
  undefined2 uVar5;
  uint uVar3;
  uint uVar6;
  int iVar7;
  int unaff_EBP;
  byte *pbVar8;
  byte *pbVar10;
  byte bVar11;
  byte *pbVar9;
  
  bVar11 = 0;
  if (*(char *)(unaff_EBP + -0xc) == '\0') {
    uVar3 = (uint)*(short *)(unaff_EBP + -0x40);
    if ((int)uVar3 <= *(int *)(unaff_EBP + -0x18)) {
      uVar3 = *(uint *)(unaff_EBP + -0x18);
    }
    iVar7 = (int)*(short *)(unaff_EBP + -0x40) - *(int *)(unaff_EBP + -0x18);
  }
  else {
    uVar3 = *(uint *)(unaff_EBP + -0x18);
    iVar7 = 0;
  }
  *(uint *)(unaff_EBP + -0x24) = uVar3;
  *(int *)(unaff_EBP + -0x28) = iVar7;
  pbVar9 = *(byte **)(unaff_EBP + -0x10);
  pbVar10 = *(byte **)(unaff_EBP + -4);
  if ((*(char *)(unaff_EBP + -0x3e) != '\0') && (pbVar9 == *(byte **)(unaff_EBP + 8))) {
    *pbVar10 = 0x2d;
    uVar3 = CONCAT31((int3)(uVar3 >> 8),0x2d);
    pbVar10 = pbVar10 + 1;
  }
LAB_0041316d:
  do {
    while( true ) {
      do {
        while( true ) {
          uVar6 = uVar3;
          pbVar8 = pbVar9 + (uint)bVar11 * -2 + 1;
          bVar1 = *pbVar9;
          uVar4 = (undefined3)(uVar6 >> 8);
          uVar3 = CONCAT31(uVar4,bVar1);
          pbVar9 = pbVar8;
          if ((bVar1 != 0x23) && (bVar1 != 0x30)) break;
          uVar3 = FUN_004131eb();
        }
      } while ((bVar1 == 0x2e) || (bVar1 == 0x2c));
      uVar5 = (undefined2)(uVar6 >> 0x10);
      if ((bVar1 != 0x27) && (bVar1 != 0x22)) break;
      while( true ) {
        bVar2 = *pbVar8;
        uVar3 = CONCAT31(CONCAT21(uVar5,bVar1),bVar2);
        pbVar9 = pbVar8 + (uint)bVar11 * -2 + 1;
        if (bVar2 == bVar1) break;
        if (bVar2 == 0) goto LAB_004131e5;
        *pbVar10 = bVar2;
        pbVar8 = pbVar8 + (uint)bVar11 * -2 + 1;
        pbVar10 = pbVar10 + (uint)bVar11 * -2 + 1;
      }
    }
    if ((bVar1 == 0x45) || (bVar1 == 0x65)) {
      bVar2 = *pbVar8;
      uVar3 = CONCAT22(uVar5,CONCAT11(bVar2,bVar1));
      if (bVar2 == 0x2b) break;
      if (bVar2 == 0x2d) {
        uVar3 = CONCAT22(uVar5,(ushort)bVar1);
        break;
      }
    }
    else if ((bVar1 == 0x3b) || (uVar3 = CONCAT31(uVar4,bVar1), bVar1 == 0)) {
LAB_004131e5:
      return (int)pbVar10 - *(int *)(unaff_EBP + -4);
    }
    *pbVar10 = (byte)uVar3;
    pbVar10 = pbVar10 + (uint)bVar11 * -2 + 1;
  } while( true );
  uVar6 = 0xffffffff;
  do {
    uVar6 = uVar6 + 1;
    pbVar8 = pbVar8 + 1;
  } while (*pbVar8 == 0x30);
  if (3 < uVar6) {
    uVar6 = 4;
  }
  uVar3 = FUN_00412e0c(uVar3,(int)*(short *)(unaff_EBP + -0x40) - *(int *)(unaff_EBP + -0x18),uVar6)
  ;
  pbVar9 = pbVar8;
  goto LAB_0041316d;
}



void __stdcall FUN_004131eb(void)

{
  int *piVar1;
  char cVar2;
  char *unaff_EBX;
  int unaff_EBP;
  char *unaff_EDI;
  
  if (*(int *)(unaff_EBP + -0x28) == 0) {
    cVar2 = *unaff_EBX;
    if (cVar2 == '\0') {
      if (*(int *)(unaff_EBP + -0x24) <= *(int *)(unaff_EBP + -0x20)) goto LAB_0041324d;
      goto LAB_0041321c;
    }
  }
  else {
    if (-1 < *(int *)(unaff_EBP + -0x28)) {
      do {
        FUN_0041320c();
        piVar1 = (int *)(unaff_EBP + -0x28);
        *piVar1 = *piVar1 + -1;
      } while (*piVar1 != 0);
      FUN_0041320c();
      return;
    }
    *(int *)(unaff_EBP + -0x28) = *(int *)(unaff_EBP + -0x28) + 1;
    if (*(int *)(unaff_EBP + -0x1c) < *(int *)(unaff_EBP + -0x24)) goto LAB_0041324d;
LAB_0041321c:
    cVar2 = '0';
  }
  if (*(int *)(unaff_EBP + -0x24) == 0) {
    *(ushort *)unaff_EDI = CONCAT11(cVar2,DAT_004308af);
  }
  else {
    *unaff_EDI = cVar2;
    if (((*(char *)(unaff_EBP + -8) != '\0') && (1 < *(int *)(unaff_EBP + -0x24))) &&
       ((char)((ushort)*(int *)(unaff_EBP + -0x24) % 3) == '\x01')) {
      unaff_EDI[1] = DAT_004308ae;
    }
  }
LAB_0041324d:
  *(int *)(unaff_EBP + -0x24) = *(int *)(unaff_EBP + -0x24) + -1;
  return;
}



void __stdcall FUN_0041320c(void)

{
  char cVar1;
  char *unaff_EBX;
  int unaff_EBP;
  char *unaff_EDI;
  
  cVar1 = *unaff_EBX;
  if (cVar1 == '\0') {
    if (*(int *)(unaff_EBP + -0x24) <= *(int *)(unaff_EBP + -0x20)) goto LAB_0041324d;
    cVar1 = '0';
  }
  if (*(int *)(unaff_EBP + -0x24) == 0) {
    *(ushort *)unaff_EDI = CONCAT11(cVar1,DAT_004308af);
  }
  else {
    *unaff_EDI = cVar1;
    if (((*(char *)(unaff_EBP + -8) != '\0') && (1 < *(int *)(unaff_EBP + -0x24))) &&
       ((char)((ushort)*(int *)(unaff_EBP + -0x24) % 3) == '\x01')) {
      unaff_EDI[1] = DAT_004308ae;
    }
  }
LAB_0041324d:
  *(int *)(unaff_EBP + -0x24) = *(int *)(unaff_EBP + -0x24) + -1;
  return;
}



void __stdcall FUN_00413251(void)

{
  char in_CL;
  
  if (in_CL == '\0') {
    FUN_00413278();
  }
  else {
    FUN_0041337f();
  }
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __stdcall FUN_00413278(void)

{
  char *pcVar1;
  unkbyte10 Var2;
  byte bVar3;
  undefined2 uVar4;
  uint uVar5;
  ushort uVar6;
  int iVar7;
  undefined2 *unaff_EBX;
  int unaff_EBP;
  longdouble *unaff_ESI;
  short *psVar8;
  short *psVar9;
  byte bVar10;
  longdouble lVar11;
  
  bVar10 = 0;
  uVar6 = *(ushort *)((int)unaff_ESI + 8);
  uVar5 = uVar6 & 0x7fff;
  if ((uVar6 & 0x7fff) == 0) {
LAB_004132a3:
    uVar6 = 0;
  }
  else {
    if (uVar5 != 0x7fff) {
      lVar11 = *unaff_ESI;
      *(int *)(unaff_EBP + -8) = ((int)((uVar5 - 0x3fff) * 0x4d10) >> 0x10) + 1;
      lVar11 = ABS(lVar11);
      FUN_00418f64(0x12 - *(int *)(unaff_EBP + -8));
      lVar11 = ROUND(lVar11);
      *(ushort *)(unaff_EBP + -4) =
           (ushort)(_DAT_00412c9c < lVar11) << 8 | (ushort)(NAN(_DAT_00412c9c) || NAN(lVar11)) << 10
           | (ushort)(_DAT_00412c9c == lVar11) << 0xe;
      if ((*(ushort *)(unaff_EBP + -4) & 0x4100) != 0) {
        lVar11 = lVar11 / (longdouble)10;
        *(int *)(unaff_EBP + -8) = *(int *)(unaff_EBP + -8) + 1;
      }
      Var2 = to_bcd(lVar11);
      *(unkbyte10 *)(unaff_EBP + -0x1a) = Var2;
      iVar7 = 9;
      psVar8 = (short *)((int)unaff_EBX + 3);
      do {
        bVar3 = *(byte *)(unaff_EBP + -0x1b + iVar7);
        psVar9 = psVar8 + (uint)bVar10 * -2 + 1;
        *psVar8 = (CONCAT11(bVar3,bVar3 >> 4) & 0xfff) + 0x3030;
        iVar7 = iVar7 + -1;
        psVar8 = psVar9;
      } while (iVar7 != 0);
      *(undefined1 *)psVar9 = 0;
      uVar5 = *(int *)(unaff_EBP + -8) + *(int *)(unaff_EBP + 8);
      if ((int)uVar5 < 0) {
        uVar5 = 0;
        goto LAB_004132a3;
      }
      if (*(uint *)(unaff_EBP + 0xc) <= uVar5) {
        uVar5 = *(uint *)(unaff_EBP + 0xc);
      }
      if (uVar5 < 0x12) {
        if (*(byte *)((int)unaff_EBX + uVar5 + 3) < 0x35) goto LAB_0041335a;
        do {
          *(undefined1 *)((int)unaff_EBX + uVar5 + 3) = 0;
          if ((int)(uVar5 - 1) < 0) {
            *(undefined2 *)((int)unaff_EBX + 3) = 0x31;
            *(int *)(unaff_EBP + -8) = *(int *)(unaff_EBP + -8) + 1;
            break;
          }
          pcVar1 = (char *)((int)unaff_EBX + uVar5 + 2);
          *pcVar1 = *pcVar1 + '\x01';
          iVar7 = uVar5 + 2;
          uVar5 = uVar5 - 1;
        } while (0x39 < *(byte *)((int)unaff_EBX + iVar7));
      }
      else {
        uVar5 = 0x12;
LAB_0041335a:
        do {
          *(undefined1 *)((int)unaff_EBX + uVar5 + 3) = 0;
          if ((int)(uVar5 - 1) < 0) {
            bVar10 = 0;
            goto LAB_0041336d;
          }
          iVar7 = uVar5 + 2;
          uVar5 = uVar5 - 1;
        } while (*(char *)((int)unaff_EBX + iVar7) == '0');
      }
      bVar10 = (byte)((ushort)*(undefined2 *)((int)unaff_ESI + 8) >> 8);
LAB_0041336d:
      uVar4 = (undefined2)*(undefined4 *)(unaff_EBP + -8);
      goto LAB_00413370;
    }
    if ((((unkuint10)*unaff_ESI & 0x8000000000000000) != 0) &&
       ((*(int *)unaff_ESI != 0 || (*(int *)((int)unaff_ESI + 4) != -0x80000000)))) {
      uVar5 = 0x8000;
      goto LAB_004132a3;
    }
  }
  bVar10 = (byte)(uVar6 >> 8);
  uVar4 = (undefined2)uVar5;
  *(undefined1 *)((int)unaff_EBX + 3) = 0;
LAB_00413370:
  *unaff_EBX = uVar4;
  *(byte *)(unaff_EBX + 1) = bVar10 >> 7;
  return;
}



void __stdcall FUN_0041337f(void)

{
  unkbyte10 Var1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  byte bVar5;
  short sVar6;
  uint uVar7;
  int iVar8;
  byte bVar9;
  uint uVar10;
  short *unaff_EBX;
  int unaff_EBP;
  uint *unaff_ESI;
  int iVar11;
  short *psVar12;
  short *psVar13;
  bool bVar14;
  longdouble lVar15;
  
  uVar7 = *unaff_ESI;
  uVar10 = unaff_ESI[1];
  if (uVar7 != 0 || uVar10 != 0) {
    if ((int)uVar10 < 0) {
      bVar14 = uVar7 != 0;
      uVar7 = -uVar7;
      uVar10 = -(uint)bVar14 - uVar10;
    }
    iVar8 = 0;
    iVar11 = *(int *)(unaff_EBP + 8);
    if (iVar11 < 0) {
      iVar11 = 0;
    }
    if (3 < iVar11) {
      iVar11 = 4;
      uVar3 = uVar7;
      iVar4 = iVar8;
      do {
        iVar8 = iVar4;
        uVar7 = uVar3;
        bVar14 = 0xde0b6b2 < uVar10;
        uVar2 = uVar10 + 0xf21f494d;
        uVar10 = uVar2 - (uVar7 < 0xa7640000);
        uVar3 = uVar7 + 0x589c0000;
        iVar4 = iVar8 + 1;
      } while (bVar14 && (uVar7 < 0xa7640000) <= uVar2);
      uVar10 = uVar10 + 0xde0b6b3 + (uint)(0x589bffff < uVar7 + 0x589c0000);
    }
    *(uint *)(unaff_EBP + -0x10) = uVar7;
    *(uint *)(unaff_EBP + -0xc) = uVar10;
    lVar15 = (longdouble)*(longlong *)(unaff_EBP + -0x10);
    if (4 - iVar11 != 0) {
      lVar15 = lVar15 / (longdouble)(int)(&DAT_00412c88)[4 - iVar11];
    }
    Var1 = to_bcd(lVar15);
    *(unkbyte10 *)(unaff_EBP + -0x1a) = Var1;
    psVar12 = (short *)((int)unaff_EBX + 3);
    if (iVar8 != 0) {
      psVar13 = unaff_EBX + 2;
      *(char *)psVar12 = (char)iVar8 + '0';
      iVar8 = 9;
      goto LAB_00413413;
    }
    iVar8 = 9;
    do {
      bVar9 = *(byte *)(unaff_EBP + -0x1b + iVar8);
      bVar5 = bVar9 >> 4;
      psVar13 = psVar12;
      if (bVar5 != 0) goto LAB_0041341c;
      if ((bVar9 & 0xf) != 0) goto LAB_00413423;
      iVar8 = iVar8 + -1;
    } while (iVar8 != 0);
  }
  sVar6 = 0;
  bVar9 = 0;
  *(undefined1 *)((int)unaff_EBX + 3) = 0;
LAB_00413449:
  *unaff_EBX = sVar6;
  *(byte *)(unaff_EBX + 1) = bVar9;
  return;
LAB_00413423:
  while( true ) {
    psVar13 = (short *)((int)psVar12 + 1);
    *(byte *)psVar12 = (bVar9 & 0xf) + 0x30;
    iVar8 = iVar8 + -1;
    if (iVar8 == 0) break;
LAB_00413413:
    bVar9 = *(byte *)(unaff_EBP + -0x1b + iVar8);
    bVar5 = bVar9 >> 4;
LAB_0041341c:
    psVar12 = (short *)((int)psVar13 + 1);
    *(byte *)psVar13 = bVar5 + 0x30;
  }
  sVar6 = (short)psVar13 - ((short)unaff_EBX + 3 + (short)iVar11);
  do {
    *(char *)psVar13 = '\0';
    psVar13 = (short *)((int)psVar13 + -1);
  } while (*(char *)psVar13 == '0');
  bVar9 = (byte)(unaff_ESI[1] >> 0x1f);
  goto LAB_00413449;
}



undefined4 FUN_00413450(byte *param_1,longdouble *param_2,char param_3)

{
  byte bVar1;
  int iVar2;
  undefined2 extraout_var;
  byte *extraout_ECX;
  byte *extraout_ECX_00;
  byte *pbVar3;
  int extraout_EDX;
  ushort in_FPUStatusWord;
  ushort uVar4;
  longdouble lVar5;
  
  uVar4 = (in_FPUStatusWord >> 8 & 0x7f) << 8;
  lVar5 = (longdouble)0;
  FUN_004134fb();
  bVar1 = *param_1;
  if ((bVar1 == 0x2b) || (bVar1 == 0x2d)) {
    param_1 = param_1 + 1;
  }
  FUN_00413506();
  pbVar3 = extraout_ECX;
  if (*param_1 == DAT_004308af) {
    param_1 = param_1 + 1;
    FUN_00413506();
    pbVar3 = extraout_ECX_00;
  }
  if (pbVar3 != param_1) {
    if ((*param_1 & 0xdf) == 0x45) {
      param_1 = param_1 + 1;
      FUN_00413522();
    }
    FUN_004134fb();
    if (*param_1 == 0) {
      iVar2 = extraout_EDX;
      if (param_3 == '\x01') {
        iVar2 = extraout_EDX + 4;
      }
      FUN_00418f64(iVar2);
      if (bVar1 == 0x2d) {
        lVar5 = -lVar5;
      }
      if (param_3 == '\0') {
        *param_2 = lVar5;
      }
      else {
        *(longlong *)param_2 = (longlong)ROUND(lVar5);
      }
      if ((uVar4 & 9) == 0) {
        return CONCAT31((int3)(CONCAT22(extraout_var,uVar4) >> 8),1);
      }
    }
  }
  return 0;
}



void __stdcall FUN_004134fb(void)

{
  char cVar1;
  char *unaff_ESI;
  
  do {
    cVar1 = *unaff_ESI;
    if (cVar1 == '\0') {
      return;
    }
    unaff_ESI = unaff_ESI + 1;
  } while (cVar1 == ' ');
  return;
}



void __stdcall FUN_00413506(void)

{
  int unaff_EBP;
  char *unaff_ESI;
  
  while( true ) {
    if ((byte)(*unaff_ESI - 0x3aU) < 0xf6) break;
    *(uint *)(unaff_EBP + -4) = (uint)(byte)(*unaff_ESI - 0x30);
    unaff_ESI = unaff_ESI + 1;
  }
  return;
}



void __stdcall FUN_00413522(void)

{
  char cVar1;
  uint uVar2;
  char *unaff_ESI;
  
  uVar2 = 0;
  if ((*unaff_ESI == '+') || (*unaff_ESI == '-')) {
    unaff_ESI = unaff_ESI + 1;
  }
  do {
    cVar1 = *unaff_ESI;
    if ((byte)(cVar1 - 0x3aU) < 0xf6) {
      return;
    }
    unaff_ESI = unaff_ESI + 1;
    uVar2 = uVar2 * 10 + (uint)(byte)(cVar1 - 0x30);
  } while (uVar2 < 500);
  return;
}



void FUN_00414010(uint param_1,uint param_2,undefined2 *param_3,undefined2 *param_4)

{
  *param_3 = (short)(param_1 / (param_2 & 0xffff));
  *param_4 = (short)(param_1 % (param_2 & 0xffff));
  return;
}



void FUN_0041402c(undefined4 *param_1)

{
  int *piVar1;
  
  piVar1 = FUN_00416c94((int *)&DAT_00413bb4,'\x01',param_1);
  FUN_0041b508(piVar1);
  return;
}



void FUN_00414044(undefined4 *param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  undefined4 *in_FS_OFFSET;
  undefined1 *puStack_20;
  undefined1 *puStack_1c;
  undefined1 *puStack_18;
  byte *local_8;
  
  puStack_18 = &stack0xfffffffc;
  local_8 = (byte *)0x0;
  puStack_1c = &LAB_00414096;
  puStack_20 = (undefined1 *)*in_FS_OFFSET;
  *in_FS_OFFSET = &puStack_20;
  FUN_0041d2c4(param_1,(int *)&local_8);
  uVar1 = FUN_00416cd0((int)&DAT_00413bb4,'\x01',local_8,param_3,param_2);
  FUN_0041b508(uVar1);
  *in_FS_OFFSET = param_3;
  puStack_20 = &LAB_0041409d;
  FUN_0041b840((int *)&local_8);
  return;
}



int FUN_004140a4(char *param_1,char *param_2)

{
  char *pcVar1;
  char *pcVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  byte bVar6;
  byte bVar7;
  bool bVar8;
  
  uVar3 = 0;
  if (param_1 != (char *)0x0) {
    uVar3 = *(uint *)(param_1 + -4);
  }
  uVar5 = 0;
  if (param_2 != (char *)0x0) {
    uVar5 = *(uint *)(param_2 + -4);
  }
  uVar4 = uVar3;
  if (uVar5 < uVar3) {
    uVar4 = uVar5;
  }
  bVar8 = true;
LAB_004140c3:
  do {
    if (uVar4 != 0) {
      uVar4 = uVar4 - 1;
      pcVar2 = param_2 + 1;
      pcVar1 = param_1 + 1;
      bVar8 = *param_1 == *param_2;
      param_1 = pcVar1;
      param_2 = pcVar2;
      if (bVar8) goto LAB_004140c3;
    }
    if (bVar8) goto LAB_004140f1;
    bVar6 = param_1[-1];
    if ((0x60 < bVar6) && (bVar6 < 0x7b)) {
      bVar6 = bVar6 - 0x20;
    }
    bVar7 = param_2[-1];
    if ((0x60 < bVar7) && (bVar7 < 0x7b)) {
      bVar7 = bVar7 - 0x20;
    }
    bVar8 = bVar6 == bVar7;
    if (!bVar8) {
      uVar3 = (uint)bVar6;
      uVar5 = (uint)bVar7;
LAB_004140f1:
      return uVar3 - uVar5;
    }
  } while( true );
}



undefined4 FUN_004140f8(char *param_1,char *param_2)

{
  int iVar1;
  
  if (param_1 != param_2) {
    if (param_1 == (char *)0x0) {
      return 0;
    }
    if ((param_2 == (char *)0x0) || (*(int *)(param_1 + -4) != *(int *)(param_2 + -4))) {
      return 0;
    }
    iVar1 = FUN_004140a4(param_1,param_2);
    param_1 = (char *)0x0;
    if (iVar1 != 0) {
      return 0;
    }
  }
  return CONCAT31((int3)((uint)param_1 >> 8),1);
}



int FUN_0041411c(undefined *param_1,undefined *param_2)

{
  int iVar1;
  PCNZCH lpString2;
  int cchCount1;
  PCNZCH lpString1;
  
  iVar1 = FUN_0041bac0((int)param_2);
  lpString2 = FUN_0041bc74(param_2);
  cchCount1 = FUN_0041bac0((int)param_1);
  lpString1 = FUN_0041bc74(param_1);
  iVar1 = CompareStringA(0x400,1,lpString1,cchCount1,lpString2,iVar1);
  return iVar1 + -2;
}



void FUN_00414154(int param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  
  iVar1 = FUN_0041bac0(param_1);
  for (iVar2 = 1; (iVar2 <= iVar1 && (*(byte *)(param_1 + -1 + iVar2) < 0x21)); iVar2 = iVar2 + 1) {
  }
  if (iVar1 < iVar2) {
    FUN_0041b840(param_2);
  }
  else {
    for (; *(byte *)(param_1 + -1 + iVar1) < 0x21; iVar1 = iVar1 + -1) {
    }
    FUN_0041bcb8(param_1,iVar2,(iVar1 - iVar2) + 1,param_2);
  }
  return;
}



undefined4 FUN_004141a4(char *param_1)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  
  uVar3 = 0;
  iVar2 = FUN_0041bac0((int)param_1);
  if ((iVar2 != 0) &&
     (((cVar1 = *param_1, (byte)(cVar1 + 0xbfU) < 0x1a || (cVar1 == '_')) ||
      ((byte)(cVar1 + 0x9fU) < 0x1a)))) {
    iVar2 = FUN_0041bac0((int)param_1);
    if (1 < iVar2) {
      iVar2 = iVar2 + -1;
      iVar4 = 2;
      do {
        cVar1 = param_1[iVar4 + -1];
        if (((9 < (byte)(cVar1 - 0x30U)) && (0x19 < (byte)(cVar1 + 0xbfU))) &&
           ((cVar1 != '_' && (0x19 < (byte)(cVar1 + 0x9fU))))) {
          return 0;
        }
        iVar4 = iVar4 + 1;
        iVar2 = iVar2 + -1;
      } while (iVar2 != 0);
    }
    uVar3 = 1;
  }
  return uVar3;
}



void FUN_00414208(undefined4 param_1,int *param_2)

{
  undefined4 local_8;
  undefined1 local_4;
  
  local_4 = 0;
  local_8 = param_1;
  FUN_00414ad4(param_2,&DAT_00414234,&local_8,0);
  return;
}



undefined4 FUN_00414238(byte *param_1)

{
  undefined4 uVar1;
  int local_14;
  byte *local_10;
  undefined1 local_c;
  
  uVar1 = FUN_0041abd0(param_1,&local_14);
  if (local_14 != 0) {
    local_c = 0xb;
    local_10 = param_1;
    FUN_00414044((undefined4 *)PTR_PTR_004306d0,&local_10,0);
  }
  return uVar1;
}



undefined4 FUN_00414274(byte *param_1,undefined4 param_2,int param_3)

{
  undefined4 uVar1;
  int local_8;
  
  local_8 = param_3;
  uVar1 = FUN_0041abd0(param_1,&local_8);
  if (local_8 != 0) {
    uVar1 = param_2;
  }
  return uVar1;
}



void FUN_0041428c(undefined *param_1,uint param_2)

{
  LPCSTR lpFileName;
  DWORD dwDesiredAccess;
  DWORD dwShareMode;
  LPSECURITY_ATTRIBUTES lpSecurityAttributes;
  DWORD dwCreationDisposition;
  DWORD dwFlagsAndAttributes;
  HANDLE hTemplateFile;
  
  hTemplateFile = (HANDLE)0x0;
  dwFlagsAndAttributes = 0x80;
  dwCreationDisposition = 3;
  lpSecurityAttributes = (LPSECURITY_ATTRIBUTES)0x0;
  dwShareMode = *(DWORD *)(&DAT_0042d4f4 + ((param_2 & 0xf0) >> 4) * 4);
  dwDesiredAccess = *(DWORD *)(&DAT_0042d4e8 + (param_2 & 3) * 4);
  lpFileName = FUN_0041bc74(param_1);
  CreateFileA(lpFileName,dwDesiredAccess,dwShareMode,lpSecurityAttributes,dwCreationDisposition,
              dwFlagsAndAttributes,hTemplateFile);
  return;
}



void FUN_004142cc(undefined *param_1)

{
  LPCSTR lpFileName;
  DWORD dwDesiredAccess;
  DWORD dwShareMode;
  LPSECURITY_ATTRIBUTES lpSecurityAttributes;
  DWORD dwCreationDisposition;
  DWORD dwFlagsAndAttributes;
  HANDLE hTemplateFile;
  
  hTemplateFile = (HANDLE)0x0;
  dwFlagsAndAttributes = 0x80;
  dwCreationDisposition = 2;
  lpSecurityAttributes = (LPSECURITY_ATTRIBUTES)0x0;
  dwShareMode = 0;
  dwDesiredAccess = 0xc0000000;
  lpFileName = FUN_0041bc74(param_1);
  CreateFileA(lpFileName,dwDesiredAccess,dwShareMode,lpSecurityAttributes,dwCreationDisposition,
              dwFlagsAndAttributes,hTemplateFile);
  return;
}



DWORD FUN_004142f0(HANDLE param_1,LPVOID param_2,DWORD param_3)

{
  BOOL BVar1;
  DWORD local_10;
  
  local_10 = param_3;
  BVar1 = ReadFile(param_1,param_2,param_3,&local_10,(LPOVERLAPPED)0x0);
  if (BVar1 == 0) {
    local_10 = 0xffffffff;
  }
  return local_10;
}



DWORD FUN_0041431c(HANDLE param_1,LPCVOID param_2,DWORD param_3)

{
  BOOL BVar1;
  DWORD local_10;
  
  local_10 = param_3;
  BVar1 = WriteFile(param_1,param_2,param_3,&local_10,(LPOVERLAPPED)0x0);
  if (BVar1 == 0) {
    local_10 = 0xffffffff;
  }
  return local_10;
}



void FUN_00414348(HANDLE param_1,LONG param_2,DWORD param_3)

{
  SetFilePointer(param_1,param_2,(PLONG)0x0,param_3);
  return;
}



void FUN_00414354(HANDLE param_1)

{
  CloseHandle(param_1);
  return;
}



undefined * FUN_0041435c(undefined *param_1)

{
  int iVar1;
  undefined4 uVar2;
  undefined *puVar3;
  
  iVar1 = FUN_0041bac0((int)param_1);
  if (iVar1 == 0) {
    puVar3 = (undefined *)0x0;
  }
  else {
    puVar3 = param_1 + iVar1 + -1;
    uVar2 = FUN_004175d0(param_1,iVar1);
    if ((char)uVar2 == '\x02') {
      puVar3 = puVar3 + -1;
    }
  }
  return puVar3;
}



int FUN_0041438c(undefined *param_1,undefined *param_2)

{
  int iVar1;
  char *pcVar2;
  char *pcVar3;
  undefined4 uVar4;
  
  iVar1 = FUN_0041bac0((int)param_2);
  pcVar2 = FUN_0041bc74(param_1);
  do {
    if (iVar1 < 1) {
      return iVar1;
    }
    if ((param_2[iVar1 + -1] != '\0') &&
       (pcVar3 = FUN_00414558(pcVar2,param_2[iVar1 + -1]), pcVar3 != (char *)0x0)) {
      uVar4 = FUN_004175d0(param_2,iVar1);
      if ((char)uVar4 != '\x02') {
        return iVar1;
      }
      iVar1 = iVar1 + -1;
    }
    iVar1 = iVar1 + -1;
  } while( true );
}



void FUN_004143dc(undefined *param_1,int *param_2)

{
  int iVar1;
  
  iVar1 = FUN_0041438c(&DAT_00414410,param_1);
  FUN_0041bcb8((int)param_1,iVar1 + 1,0x7fffffff,param_2);
  return;
}



int FUN_00414488(char *param_1)

{
  char cVar1;
  int iVar2;
  
  iVar2 = -1;
  do {
    if (iVar2 == 0) break;
    iVar2 = iVar2 + -1;
    cVar1 = *param_1;
    param_1 = param_1 + 1;
  } while (cVar1 != '\0');
  return -2 - iVar2;
}



void FUN_004144a0(undefined4 *param_1,undefined4 *param_2,uint param_3)

{
  uint uVar1;
  undefined1 *puVar2;
  undefined4 *puVar3;
  undefined1 *puVar4;
  undefined4 *puVar5;
  
  if (param_2 < param_1) {
    puVar2 = (undefined1 *)((int)param_2 + (param_3 - 1));
    puVar4 = (undefined1 *)((int)param_1 + (param_3 - 1));
    for (uVar1 = param_3 & 3; uVar1 != 0; uVar1 = uVar1 - 1) {
      *puVar4 = *puVar2;
      puVar2 = puVar2 + -1;
      puVar4 = puVar4 + -1;
    }
    puVar3 = (undefined4 *)(puVar2 + -3);
    puVar5 = (undefined4 *)(puVar4 + -3);
    for (uVar1 = param_3 >> 2; uVar1 != 0; uVar1 = uVar1 - 1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + -1;
      puVar5 = puVar5 + -1;
    }
  }
  else if (param_1 != param_2) {
    for (uVar1 = param_3 >> 2; uVar1 != 0; uVar1 = uVar1 - 1) {
      *param_1 = *param_2;
      param_2 = param_2 + 1;
      param_1 = param_1 + 1;
    }
    for (uVar1 = param_3 & 3; uVar1 != 0; uVar1 = uVar1 - 1) {
      *(undefined1 *)param_1 = *(undefined1 *)param_2;
      param_2 = (undefined4 *)((int)param_2 + 1);
      param_1 = (undefined4 *)((int)param_1 + 1);
    }
    return;
  }
  return;
}



char * FUN_004144dc(char *param_1,char *param_2,int param_3)

{
  int iVar1;
  uint uVar2;
  char *pcVar3;
  bool bVar4;
  
  bVar4 = param_3 == 0;
  iVar1 = param_3;
  pcVar3 = param_2;
  if (!bVar4) {
    do {
      if (iVar1 == 0) break;
      iVar1 = iVar1 + -1;
      bVar4 = *pcVar3 == '\0';
      pcVar3 = pcVar3 + 1;
    } while (!bVar4);
    if (bVar4) {
      iVar1 = iVar1 + 1;
    }
  }
  pcVar3 = param_1;
  for (uVar2 = (uint)(param_3 - iVar1) >> 2; uVar2 != 0; uVar2 = uVar2 - 1) {
    *(undefined4 *)pcVar3 = *(undefined4 *)param_2;
    param_2 = param_2 + 4;
    pcVar3 = pcVar3 + 4;
  }
  for (uVar2 = param_3 - iVar1 & 3; uVar2 != 0; uVar2 = uVar2 - 1) {
    *pcVar3 = *param_2;
    param_2 = param_2 + 1;
    pcVar3 = pcVar3 + 1;
  }
  *pcVar3 = '\0';
  return param_1;
}



void FUN_00414510(char *param_1,char *param_2,int param_3)

{
  char *pcVar1;
  char cVar2;
  byte bVar3;
  int iVar4;
  undefined3 uVar5;
  int iVar6;
  uint uVar7;
  char *pcVar8;
  bool bVar9;
  
  iVar4 = 0;
  iVar6 = param_3;
  pcVar8 = param_2;
  if (param_3 != 0) {
    do {
      if (iVar6 == 0) break;
      iVar6 = iVar6 + -1;
      cVar2 = *pcVar8;
      pcVar8 = pcVar8 + 1;
    } while (cVar2 != '\0');
    iVar6 = param_3 - iVar6;
    bVar9 = true;
LAB_00414529:
    do {
      if (iVar6 != 0) {
        iVar6 = iVar6 + -1;
        pcVar1 = param_2 + 1;
        pcVar8 = param_1 + 1;
        bVar9 = *param_1 == *param_2;
        param_1 = pcVar8;
        param_2 = pcVar1;
        if (bVar9) goto LAB_00414529;
      }
      if (bVar9) {
        return;
      }
      bVar3 = param_1[-1];
      uVar5 = (undefined3)((uint)iVar4 >> 8);
      iVar4 = CONCAT31(uVar5,bVar3);
      if ((0x60 < bVar3) && (bVar3 < 0x7b)) {
        iVar4 = CONCAT31(uVar5,bVar3 - 0x20);
      }
      bVar3 = param_2[-1];
      uVar7 = (uint)bVar3;
      if ((0x60 < bVar3) && (bVar3 < 0x7b)) {
        uVar7 = (uint)(byte)(bVar3 - 0x20);
      }
      iVar4 = iVar4 - uVar7;
      bVar9 = true;
    } while (iVar4 == 0);
  }
  return;
}



char * FUN_00414558(char *param_1,char param_2)

{
  char *pcVar1;
  uint uVar2;
  char *pcVar3;
  bool bVar4;
  
  uVar2 = 0xffffffff;
  bVar4 = true;
  pcVar3 = param_1;
  do {
    if (uVar2 == 0) break;
    uVar2 = uVar2 - 1;
    bVar4 = *pcVar3 == '\0';
    pcVar3 = pcVar3 + 1;
  } while (!bVar4);
  uVar2 = ~uVar2;
  do {
    pcVar3 = param_1;
    if (uVar2 == 0) break;
    uVar2 = uVar2 - 1;
    pcVar3 = param_1 + 1;
    bVar4 = param_2 == *param_1;
    param_1 = pcVar3;
  } while (!bVar4);
  pcVar1 = (char *)0x0;
  if (bVar4) {
    pcVar1 = pcVar3 + -1;
  }
  return pcVar1;
}



char * FUN_00414578(char *param_1,char *param_2)

{
  char cVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  char *pcVar7;
  char *pcVar8;
  bool bVar9;
  
  if ((param_1 != (char *)0x0) && (param_2 != (char *)0x0)) {
    uVar2 = 0xffffffff;
    pcVar7 = param_2;
    do {
      if (uVar2 == 0) break;
      uVar2 = uVar2 - 1;
      cVar1 = *pcVar7;
      pcVar7 = pcVar7 + 1;
    } while (cVar1 != '\0');
    uVar3 = ~uVar2 - 1;
    if (uVar3 != 0) {
      uVar4 = 0xffffffff;
      pcVar7 = param_1;
      do {
        if (uVar4 == 0) break;
        uVar4 = uVar4 - 1;
        cVar1 = *pcVar7;
        pcVar7 = pcVar7 + 1;
      } while (cVar1 != '\0');
      iVar5 = ~uVar4 - uVar3;
      bVar9 = iVar5 == 0;
      if (uVar3 <= ~uVar4 && !bVar9) {
        do {
          pcVar7 = param_1;
          do {
            param_1 = pcVar7;
            if (iVar5 == 0) break;
            iVar5 = iVar5 + -1;
            param_1 = pcVar7 + 1;
            bVar9 = *param_2 == *pcVar7;
            pcVar7 = param_1;
          } while (!bVar9);
          iVar6 = ~uVar2 - 2;
          pcVar8 = param_1;
          pcVar7 = param_2;
          if (!bVar9) {
            return (char *)0x0;
          }
          do {
            if (iVar6 == 0) break;
            bVar9 = pcVar7[1] == *pcVar8;
            iVar6 = iVar6 + -1;
            pcVar8 = pcVar8 + 1;
            pcVar7 = pcVar7 + 1;
          } while (bVar9);
          if (bVar9) {
            return param_1 + -1;
          }
        } while( true );
      }
    }
  }
  return (char *)0x0;
}



int * AllocBuffer(int param_1)

{
  int *piVar1;
  
  piVar1 = (int *)FUN_0041a9c0(param_1 + 4);
  *piVar1 = param_1 + 4;
  return piVar1 + 1;
}



undefined4 FUN_004145e0(char *param_1)

{
  int iVar1;
  int *piVar2;
  undefined4 uVar3;
  
  if (param_1 == (char *)0x0) {
    return 0;
  }
  iVar1 = FUN_00414488(param_1);
  piVar2 = AllocBuffer(iVar1 + 1U);
  uVar3 = FUN_004144a0(piVar2,(undefined4 *)param_1,iVar1 + 1U);
  return uVar3;
}



void FreeAllocBuffer(int param_1)

{
  if (param_1 != 0) {
    FUN_0041a9d8(param_1 + -4);
  }
  return;
}



// WARNING: Type propagation algorithm not settling

void FUN_0041461c(int param_1,byte *param_2,uint param_3)

{
  undefined4 uVar1;
  undefined1 auStack_35 [33];
  undefined1 *local_14;
  undefined1 local_10;
  
  if (0x1f < param_3) {
    param_3 = 0x1f;
  }
  uVar1 = FUN_004175f4(param_2,param_3 - 1);
  if ((char)uVar1 == '\x01') {
    param_3 = param_3 - 1;
  }
  FUN_004144a0((undefined4 *)((int)auStack_35 + 1),(undefined4 *)param_2,param_3);
  auStack_35[param_3 + 1] = 0;
  local_10 = 6;
  local_14 = (undefined1 *)((int)auStack_35 + 1);
  FUN_00414044((undefined4 *)(&PTR_PTR_0042d508)[param_1],&local_14,0);
  return;
}



void FUN_00414674(int *param_1,VARIANTARG *param_2)

{
  FUN_0041c9d0(param_1,param_2);
  return;
}



void FUN_00414688(int *param_1)

{
  FUN_0041b840(param_1);
  return;
}



void FUN_00414694(byte *param_1,byte *param_2,byte *param_3,undefined4 param_4,undefined4 param_5,
                 int param_6)

{
  byte bVar1;
  char cVar2;
  uint uVar3;
  undefined3 uVar5;
  undefined4 uVar4;
  byte *pbVar6;
  byte *extraout_ECX;
  byte *extraout_ECX_00;
  byte *extraout_ECX_01;
  byte *pbVar7;
  byte *extraout_EDX;
  byte *extraout_EDX_00;
  byte *pbVar8;
  byte *pbVar9;
  byte *pbVar10;
  byte *pbVar11;
  bool bVar12;
  byte bVar13;
  int local_10;
  byte *local_c;
  byte *local_8;
  
  bVar13 = 0;
  pbVar6 = param_3 + param_6;
  uVar3 = 0;
  local_c = (byte *)0x0;
  local_10 = 0;
  local_8 = param_1;
LAB_004146b8:
  if (param_2 != (byte *)0x0) {
    do {
      if (param_3 == pbVar6) break;
      pbVar9 = param_3 + (uint)bVar13 * -2 + 1;
      bVar1 = *param_3;
      uVar5 = (undefined3)(uVar3 >> 8);
      uVar3 = CONCAT31(uVar5,bVar1);
      param_3 = pbVar9;
      if (bVar1 == 0x25) {
        if (pbVar9 == pbVar6) break;
        param_3 = pbVar9 + (uint)bVar13 * -2 + 1;
        uVar3 = CONCAT31(uVar5,*pbVar9);
        if (*pbVar9 != 0x25) goto code_r0x004146de;
      }
      pbVar9 = param_1 + (uint)bVar13 * -2 + 1;
      *param_1 = (byte)uVar3;
      param_2 = param_2 + -1;
      param_1 = pbVar9;
      if (param_2 == (byte *)0x0) break;
    } while( true );
  }
LAB_004146ca:
  FUN_00414a7c((int)param_1 - (int)local_8);
  return;
code_r0x004146de:
  pbVar9 = param_3 + -2;
  while( true ) {
    cVar2 = (char)uVar3;
    pbVar10 = param_3;
    if (cVar2 == '-') {
      if (param_3 == pbVar6) goto LAB_004146ca;
      pbVar10 = param_3 + (uint)bVar13 * -2 + 1;
      uVar3 = CONCAT31((int3)(uVar3 >> 8),*param_3);
    }
    uVar3 = FUN_00414776(uVar3,param_2,pbVar6);
    uVar5 = (undefined3)(uVar3 >> 8);
    pbVar6 = extraout_ECX;
    if ((char)uVar3 != ':') break;
    local_c = pbVar9;
    if (pbVar10 == extraout_ECX) goto LAB_004146ca;
    param_3 = pbVar10 + (uint)bVar13 * -2 + 1;
    uVar3 = CONCAT31(uVar5,*pbVar10);
    param_2 = extraout_EDX;
  }
  pbVar8 = extraout_EDX;
  param_3 = pbVar10;
  if ((char)uVar3 == '.') {
    if (pbVar10 == extraout_ECX) goto LAB_004146ca;
    param_3 = pbVar10 + (uint)bVar13 * -2 + 1;
    uVar3 = FUN_00414776(CONCAT31(uVar5,*pbVar10),extraout_EDX,extraout_ECX);
    pbVar6 = extraout_ECX_00;
    pbVar8 = extraout_EDX_00;
  }
  uVar4 = FUN_004147bf((char)uVar3,pbVar8,pbVar6);
  pbVar10 = pbVar9 + -(int)extraout_ECX_01;
  if (pbVar9 < extraout_ECX_01) {
    pbVar10 = (byte *)0x0;
  }
  pbVar9 = extraout_ECX_01;
  pbVar11 = param_3;
  if (cVar2 == '-') {
    bVar12 = pbVar8 < extraout_ECX_01;
    pbVar8 = pbVar8 + -(int)extraout_ECX_01;
    pbVar7 = extraout_ECX_01;
    if (bVar12) {
      pbVar7 = extraout_ECX_01 + (int)pbVar8;
      pbVar8 = (byte *)0x0;
    }
    for (; pbVar9 = (byte *)0x0, pbVar7 != (byte *)0x0; pbVar7 = pbVar7 + -1) {
      *param_1 = *pbVar11;
      pbVar11 = pbVar11 + (uint)bVar13 * -2 + 1;
      param_1 = param_1 + (uint)bVar13 * -2 + 1;
    }
  }
  pbVar7 = pbVar8 + -(int)pbVar10;
  if (pbVar8 < pbVar10) {
    pbVar10 = pbVar10 + (int)pbVar7;
    pbVar7 = (byte *)0x0;
  }
  uVar3 = CONCAT31((int3)((uint)uVar4 >> 8),0x20);
  for (; pbVar10 != (byte *)0x0; pbVar10 = pbVar10 + -1) {
    *param_1 = 0x20;
    param_1 = param_1 + (uint)bVar13 * -2 + 1;
  }
  param_2 = pbVar7 + -(int)pbVar9;
  if (pbVar7 < pbVar9) {
    pbVar9 = pbVar9 + (int)param_2;
    param_2 = (byte *)0x0;
  }
  for (; pbVar9 != (byte *)0x0; pbVar9 = pbVar9 + -1) {
    *param_1 = *pbVar11;
    pbVar11 = pbVar11 + (uint)bVar13 * -2 + 1;
    param_1 = param_1 + (uint)bVar13 * -2 + 1;
  }
  if (local_10 != 0) {
    uVar3 = FUN_00414688(&local_10);
  }
  goto LAB_004146b8;
}



uint FUN_00414776(uint param_1,undefined4 param_2,byte *param_3)

{
  uint uVar1;
  int unaff_EBP;
  byte *unaff_ESI;
  int unaff_EDI;
  
  if ((char)param_1 == '*') {
    uVar1 = *(uint *)(unaff_EBP + -8);
    if (uVar1 <= *(uint *)(unaff_EBP + 8)) {
      *(int *)(unaff_EBP + -8) = *(int *)(unaff_EBP + -8) + 1;
    }
    if (unaff_ESI == param_3) {
LAB_004146ca:
      uVar1 = FUN_00414a7c(unaff_EDI - *(int *)(unaff_EBP + -4));
      return uVar1;
    }
    param_1 = CONCAT31((int3)(uVar1 >> 8),*unaff_ESI);
  }
  else {
    while ((0x2f < (byte)param_1 && ((byte)param_1 < 0x3a))) {
      if (unaff_ESI == param_3) goto LAB_004146ca;
      param_1 = (uint)*unaff_ESI;
      unaff_ESI = unaff_ESI + 1;
    }
  }
  return param_1;
}



void FUN_004147bf(undefined1 param_1,undefined4 param_2,undefined4 param_3)

{
  int *piVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  int iVar4;
  int *piVar5;
  uint uVar6;
  char extraout_CL;
  uint uVar7;
  byte bVar8;
  int extraout_EDX;
  int unaff_EBP;
  int unaff_ESI;
  byte *pbVar9;
  
  uVar3 = 1;
  uVar6 = *(uint *)(unaff_EBP + -8);
  if (uVar6 <= *(uint *)(unaff_EBP + 8)) {
    *(int *)(unaff_EBP + -8) = *(int *)(unaff_EBP + -8) + 1;
    puVar2 = (undefined4 *)(*(int *)(unaff_EBP + 0xc) + uVar6 * 8);
                    // WARNING: Could not recover jumptable at 0x004147df. Too many branches
                    // WARNING: Treating indirect jump as call
    (**(code **)(&DAT_004147e6 + (uint)*(byte *)(puVar2 + 1) * 4))
              (*puVar2,param_2,CONCAT31((int3)((uint)param_3 >> 8),param_1) & 0xffffffdf);
    return;
  }
  do {
    iVar4 = FUN_00414a71(uVar3);
    piVar5 = (int *)FUN_0041461c(iVar4,*(byte **)(unaff_EBP + -0x14),
                                 *(int *)(unaff_EBP + -0x24) - (int)*(byte **)(unaff_EBP + -0x14));
    piVar1 = (int *)(unaff_EBP + -0x30);
    *piVar1 = *piVar5;
    *(int *)(unaff_EBP + -0x2c) = piVar5[1];
    if (extraout_CL == 'D') {
      if ((*(uint *)(unaff_EBP + -0x2c) & 0x80000000) != 0) {
        iVar4 = *piVar1;
        *piVar1 = -*piVar1;
        *(int *)(unaff_EBP + -0x2c) = *(int *)(unaff_EBP + -0x2c) + (uint)(iVar4 != 0);
        *(int *)(unaff_EBP + -0x2c) = -*(int *)(unaff_EBP + -0x2c);
        FUN_0041487f();
        *(undefined1 *)(unaff_ESI + -1) = 0x2d;
        return;
      }
code_r0x0041487f:
      uVar3 = 10;
LAB_00414884:
      pbVar9 = (byte *)(unaff_EBP + -0x51);
      do {
        uVar6 = FUN_0041941d(*piVar1,*(uint *)(unaff_EBP + -0x2c),uVar3,uVar3,0);
        bVar8 = (char)uVar6 + 0x30;
        if (0x39 < bVar8) {
          bVar8 = (char)uVar6 + 0x37;
        }
        pbVar9 = pbVar9 + -1;
        *pbVar9 = bVar8;
        iVar4 = FUN_00419328(*piVar1,*(uint *)(unaff_EBP + -0x2c),uVar3,uVar3,0);
        *piVar1 = iVar4;
        *(int *)(unaff_EBP + -0x2c) = extraout_EDX;
      } while (iVar4 != 0 || extraout_EDX != 0);
      uVar7 = (unaff_EBP + -0x51) - (int)pbVar9;
      uVar6 = *(uint *)(unaff_EBP + -0x20);
      if (uVar6 < 0x11) {
        iVar4 = uVar6 - uVar7;
        if (uVar7 <= uVar6 && iVar4 != 0) {
          do {
            pbVar9 = pbVar9 + -1;
            *pbVar9 = 0x30;
            iVar4 = iVar4 + -1;
          } while (iVar4 != 0);
        }
        return;
      }
      return;
    }
    if (extraout_CL == 'U') goto code_r0x0041487f;
    if (extraout_CL == 'X') {
      uVar3 = 0x10;
      goto LAB_00414884;
    }
    uVar3 = 0;
  } while( true );
}



void __stdcall FUN_0041487f(void)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  byte bVar4;
  int extraout_EDX;
  int *unaff_EBX;
  int unaff_EBP;
  byte *pbVar5;
  
  pbVar5 = (byte *)(unaff_EBP + -0x51);
  do {
    uVar1 = FUN_0041941d(*unaff_EBX,unaff_EBX[1],10,10,0);
    bVar4 = (char)uVar1 + 0x30;
    if (0x39 < bVar4) {
      bVar4 = (char)uVar1 + 0x37;
    }
    pbVar5 = pbVar5 + -1;
    *pbVar5 = bVar4;
    iVar2 = FUN_00419328(*unaff_EBX,unaff_EBX[1],10,10,0);
    *unaff_EBX = iVar2;
    unaff_EBX[1] = extraout_EDX;
  } while (iVar2 != 0 || extraout_EDX != 0);
  uVar3 = (unaff_EBP + -0x51) - (int)pbVar5;
  uVar1 = *(uint *)(unaff_EBP + -0x20);
  if (uVar1 < 0x11) {
    iVar2 = uVar1 - uVar3;
    if (uVar3 <= uVar1 && iVar2 != 0) {
      do {
        pbVar5 = pbVar5 + -1;
        *pbVar5 = 0x30;
        iVar2 = iVar2 + -1;
      } while (iVar2 != 0);
    }
    return;
  }
  return;
}



undefined4 FUN_00414a71(undefined4 param_1)

{
  int unaff_EBP;
  
  FUN_0041b840((int *)(unaff_EBP + -0x10));
  return param_1;
}



void FUN_00414a7c(undefined4 param_1)

{
  FUN_00414a71(param_1);
  return;
}



byte * FUN_00414a8c(byte *param_1,byte *param_2,byte *param_3,undefined4 param_4,undefined4 param_5)

{
  int iVar1;
  
  iVar1 = FUN_00414488((char *)param_3);
  iVar1 = FUN_00414694(param_1,param_2,param_3,param_4,param_5,iVar1);
  param_1[iVar1] = 0;
  return param_1;
}



void FUN_00414ac0(byte *param_1,undefined4 param_2,undefined4 param_3,int *param_4)

{
  FUN_00414ad4(param_4,param_1,param_2,param_3);
  return;
}



// WARNING: Removing unreachable block (ram,0x00414afe)

void FUN_00414ad4(int *param_1,byte *param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  byte local_100e [10];
  int *piStack_1004;
  undefined4 local_c;
  byte *local_8;
  
  piStack_1004 = param_1;
  local_c = param_3;
  local_8 = param_2;
  iVar1 = FUN_0041bac0((int)param_2);
  if (iVar1 < 0xc02) {
    iVar1 = FUN_0041bac0((int)local_8);
    uVar2 = FUN_00414694(local_100e,(byte *)0x1001,local_8,param_4,local_c,iVar1);
    uVar3 = 0x1002;
  }
  else {
    uVar2 = FUN_0041bac0((int)local_8);
    uVar3 = uVar2;
  }
  if ((int)uVar2 < (int)(uVar3 - 1)) {
    FUN_0041b928(param_1,(undefined4 *)local_100e,uVar2);
  }
  else {
    while ((int)(uVar3 - 1) <= (int)uVar2) {
      uVar3 = uVar3 * 2;
      FUN_0041b840(param_1);
      FUN_0041bd88(param_1,uVar3);
      iVar1 = FUN_0041bac0((int)local_8);
      uVar2 = FUN_00414694((byte *)*param_1,(byte *)(uVar3 - 1),local_8,param_4,local_c,iVar1);
    }
    FUN_0041bd88(param_1,uVar2);
  }
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_00414b9c(undefined4 *param_1,undefined4 param_2,undefined4 param_3,double param_4)

{
  ulonglong uVar1;
  int iVar2;
  undefined4 uVar3;
  int local_c;
  int iStack_8;
  
  uVar1 = (ulonglong)ROUND(param_4 * (double)_DAT_0042d510);
  local_c = (int)uVar1;
  iStack_8 = (int)(uVar1 >> 0x20);
  if ((longlong)uVar1 < 0) {
    uVar1 = CONCAT44(-(uint)(local_c != 0) - iStack_8,-local_c);
    uVar3 = (undefined4)(uVar1 % (ulonglong)DAT_0042d514);
    iVar2 = -(int)(uVar1 / DAT_0042d514);
  }
  else {
    iVar2 = (int)(uVar1 / DAT_0042d514);
    uVar3 = (undefined4)(uVar1 % (ulonglong)DAT_0042d514);
  }
  *param_1 = uVar3;
  param_1[1] = iVar2 + 0xa955a;
  return;
}



undefined1 FUN_00414bdc(ushort param_1,ushort param_2,ushort param_3,double *param_4,ushort param_5)

{
  undefined1 local_5;
  
  local_5 = 0;
  if ((((param_1 < 0x18) && (param_2 < 0x3c)) && (param_3 < 0x3c)) && (param_5 < 1000)) {
    *param_4 = (double)((float)(int)((uint)param_1 * 3600000 + (uint)param_2 * 60000 +
                                     (uint)param_3 * 1000 + (uint)param_5) / 8.64e+07);
    local_5 = 1;
  }
  return local_5;
}



void FUN_00414c4c(undefined2 *param_1,undefined2 *param_2,undefined2 *param_3,undefined2 *param_4,
                 undefined4 param_5,undefined4 param_6)

{
  undefined2 extraout_var;
  undefined2 extraout_var_00;
  undefined2 extraout_var_01;
  ushort *puVar1;
  uint local_10 [2];
  ushort local_8;
  ushort local_6;
  
  puVar1 = &local_8;
  FUN_00414b9c(local_10,param_2,param_3,(double)CONCAT44(param_6,param_5));
  FUN_00414010(local_10[0],CONCAT22(extraout_var,60000),&local_6,puVar1);
  FUN_00414010((uint)local_6,CONCAT22(extraout_var_00,0x3c),param_1,param_2);
  FUN_00414010((uint)local_8,CONCAT22(extraout_var_01,1000),param_3,param_4);
  return;
}



undefined4 FUN_00414ca8(uint param_1)

{
  uint uVar1;
  uint uVar2;
  
  if ((param_1 & 3) == 0) {
    uVar2 = param_1 & 0xffff;
    uVar1 = 100;
    if ((uVar2 % 100 != 0) || (uVar1 = 400, uVar2 % 400 == 0)) {
      return CONCAT31((int3)(uVar2 / uVar1 >> 8),1);
    }
  }
  return 0;
}



undefined1 FUN_00414ce4(uint param_1,ushort param_2,ushort param_3,double *param_4)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined1 local_7;
  
  local_7 = 0;
  uVar1 = FUN_00414ca8(param_1);
  iVar4 = (uVar1 & 0x7f) * 0x18;
  if (((((ushort)param_1 != 0) && ((ushort)param_1 < 10000)) && (param_2 != 0)) &&
     (((param_2 < 0xd && (param_3 != 0)) &&
      (param_3 <= *(ushort *)(iVar4 + 0x42d492 + (uint)param_2 * 2))))) {
    iVar2 = param_2 - 1;
    if (0 < iVar2) {
      iVar3 = 1;
      do {
        param_3 = param_3 + *(short *)(iVar4 + 0x42d492 + iVar3 * 2);
        iVar3 = iVar3 + 1;
        iVar2 = iVar2 + -1;
      } while (iVar2 != 0);
    }
    iVar2 = (param_1 & 0xffff) - 1;
    iVar4 = iVar2;
    if (iVar2 < 0) {
      iVar4 = (param_1 & 0xffff) + 2;
    }
    *param_4 = (double)(int)(((iVar2 * 0x16d + (iVar4 >> 2)) - iVar2 / 100) + iVar2 / 400 +
                             (uint)param_3 + -0xa955a);
    local_7 = 1;
  }
  return local_7;
}



void FUN_00414dac(uint param_1,ushort param_2,ushort param_3)

{
  char cVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_28;
  undefined1 *puStack_24;
  undefined1 *puStack_20;
  undefined4 *local_10;
  double local_c;
  
  puStack_20 = &stack0xfffffffc;
  local_10 = (undefined4 *)0x0;
  puStack_24 = &LAB_00414e0c;
  uStack_28 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_28;
  cVar1 = FUN_00414ce4(param_1,param_2,param_3,&local_c);
  if (cVar1 == '\0') {
    FUN_0041d2c4((undefined4 *)PTR_PTR_004306e0,(int *)&local_10);
    FUN_0041402c(local_10);
  }
  *in_FS_OFFSET = uStack_28;
  puStack_20 = &LAB_00414e13;
  puStack_24 = (undefined1 *)0x414e0b;
  FUN_0041b840((int *)&local_10);
  return;
}



void FUN_00414e20(short *param_1,ushort *param_2,short *param_3,undefined2 *param_4,
                 undefined4 param_5,undefined4 param_6)

{
  ushort uVar1;
  short sVar2;
  short sVar3;
  ushort uVar4;
  uint uVar5;
  undefined2 extraout_var;
  undefined2 extraout_var_00;
  short sVar6;
  undefined4 local_1c;
  int local_18;
  short local_14;
  ushort local_12;
  short *local_10;
  ushort *local_c;
  short *local_8;
  
  local_10 = param_3;
  local_c = param_2;
  local_8 = param_1;
  FUN_00414b9c(&local_1c,param_2,param_3,(double)CONCAT44(param_6,param_5));
  if (local_18 < 1) {
    *local_8 = 0;
    *local_c = 0;
    *local_10 = 0;
    *param_4 = 0;
  }
  else {
    *param_4 = (short)(local_18 % 7);
    sVar6 = 1;
    for (uVar5 = local_18 - 1; 0x23ab0 < (int)uVar5; uVar5 = uVar5 - 0x23ab1) {
      sVar6 = sVar6 + 400;
    }
    FUN_00414010(uVar5,CONCAT22((short)((uint)(local_18 % 7) >> 0x10),0x8eac),&local_14,&local_12);
    if (local_14 == 4) {
      local_14 = 3;
      local_12 = local_12 + 0x8eac;
    }
    sVar2 = local_14 * 100;
    FUN_00414010((uint)local_12,CONCAT22(extraout_var,0x5b5),&local_14,&local_12);
    sVar3 = local_14 * 4;
    FUN_00414010((uint)local_12,CONCAT22(extraout_var_00,0x16d),&local_14,&local_12);
    if (local_14 == 4) {
      local_14 = 3;
      local_12 = local_12 + 0x16d;
    }
    sVar6 = sVar6 + sVar2 + sVar3 + local_14;
    uVar5 = FUN_00414ca8(CONCAT22((short)((uint)param_4 >> 0x10),sVar6));
    uVar4 = 1;
    for (; uVar1 = *(ushort *)((uVar5 & 0x7f) * 0x18 + 0x42d492 + (uint)uVar4 * 2),
        uVar1 <= local_12; local_12 = local_12 - uVar1) {
      uVar4 = uVar4 + 1;
    }
    *local_8 = sVar6;
    *local_c = uVar4;
    *local_10 = local_12 + 1;
  }
  return;
}



void FUN_00414f64(short *param_1,ushort *param_2,short *param_3,undefined4 param_4,
                 undefined4 param_5)

{
  undefined4 uStack_8;
  
  uStack_8 = param_3;
  FUN_00414e20(param_1,param_2,param_3,(undefined2 *)((int)&uStack_8 + 2),param_4,param_5);
  return;
}



int FUN_00414f84(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                undefined4 param_5)

{
  undefined4 local_c;
  int local_8;
  
  FUN_00414b9c(&local_c,param_2,param_3,(double)CONCAT44(param_5,param_4));
  return local_8 % 7 + 1;
}



WORD __stdcall FUN_00414fac(void)

{
  _SYSTEMTIME local_10;
  
  GetLocalTime(&local_10);
  return local_10.wYear;
}



void FUN_00414fc0(undefined4 *param_1,uint param_2,undefined4 param_3,int param_4)

{
  uint uVar1;
  
  uVar1 = 0x100 - *(int *)(param_4 + -0x104);
  if ((int)param_2 < (int)uVar1) {
    uVar1 = param_2;
  }
  if (uVar1 != 0) {
    FUN_0041aae4(param_1,(undefined4 *)(param_4 + -0x100 + *(int *)(param_4 + -0x104)),uVar1);
  }
  *(int *)(param_4 + -0x104) = *(int *)(param_4 + -0x104) + uVar1;
  return;
}



void FUN_00415004(undefined4 *param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  uint uVar1;
  undefined4 extraout_ECX;
  
  uVar1 = FUN_0041bac0((int)param_1);
  FUN_00414fc0(param_1,uVar1,extraout_ECX,param_4);
  return;
}



void FUN_00415024(undefined4 param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  uint uVar1;
  undefined4 extraout_ECX;
  undefined4 local_24;
  undefined1 local_20;
  undefined4 local_1c;
  undefined1 local_18;
  byte local_14 [16];
  
  local_20 = 0;
  local_18 = 0;
  local_24 = param_2;
  local_1c = param_1;
  uVar1 = FUN_00414694(local_14,(byte *)0x10,&DAT_0042d518,1,&local_24,4);
  FUN_00414fc0((undefined4 *)local_14,uVar1,extraout_ECX,param_4);
  return;
}



void __cdecl FUN_00415070(int param_1)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = (int *)(param_1 + -4);
  iVar1 = *piVar2;
  while (*(char *)*piVar2 == *(char *)(param_1 + -5)) {
    *piVar2 = *piVar2 + 1;
  }
  *(int *)(param_1 + -0xc) = (*piVar2 - iVar1) + 1;
  return;
}



void __cdecl FUN_0041509c(int param_1)

{
  if (*(char *)(param_1 + -0x13) == '\0') {
    FUN_00414f64((short *)(param_1 + -0xe),(ushort *)(param_1 + -0x10),(short *)(param_1 + -0x12),
                 *(undefined4 *)(*(int *)(param_1 + 8) + 8),
                 *(undefined4 *)(*(int *)(param_1 + 8) + 0xc));
    *(undefined1 *)(param_1 + -0x13) = 1;
  }
  return;
}



void __cdecl FUN_004150d4(int param_1)

{
  if (*(char *)(param_1 + -0x1d) == '\0') {
    FUN_00414c4c((undefined2 *)(param_1 + -0x16),(undefined2 *)(param_1 + -0x18),
                 (undefined2 *)(param_1 + -0x1a),(undefined2 *)(param_1 + -0x1c),
                 *(undefined4 *)(*(int *)(param_1 + 8) + 8),
                 *(undefined4 *)(*(int *)(param_1 + 8) + 0xc));
    *(undefined1 *)(param_1 + -0x1d) = 1;
  }
  return;
}



void FUN_00415114(int param_1,int *param_2,undefined4 param_3,int param_4)

{
  LPCSTR lpFormat;
  LCID Locale;
  uint uVar1;
  undefined4 *in_FS_OFFSET;
  DWORD dwFlags;
  SYSTEMTIME *lpDate;
  CHAR *lpDateStr;
  int iVar2;
  int *piVar3;
  undefined4 uStack_130;
  undefined1 *puStack_12c;
  undefined1 *puStack_128;
  undefined4 local_11c;
  CHAR local_118 [256];
  SYSTEMTIME local_18;
  undefined *local_8;
  
  puStack_128 = &stack0xfffffffc;
  local_11c = 0;
  local_8 = (undefined *)0x0;
  puStack_12c = &LAB_00415266;
  uStack_130 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_130;
  FUN_0041b840(param_2);
  local_18.wYear = *(WORD *)(param_4 + -0xe);
  local_18.wMonth = *(WORD *)(param_4 + -0x10);
  local_18.wDay = *(WORD *)(param_4 + -0x12);
  FUN_0041b8d8((int *)&local_8,0x41527c);
  iVar2 = 0x100;
  lpDateStr = local_118;
  lpFormat = FUN_0041bc74(local_8);
  lpDate = &local_18;
  dwFlags = 4;
  Locale = GetThreadLocale();
  iVar2 = GetDateFormatA(Locale,dwFlags,lpDate,lpFormat,lpDateStr,iVar2);
  if (iVar2 != 0) {
    FUN_0041ba70(param_2,local_118,0x100);
    if (param_1 == 1) {
      if (DAT_0043096c == 4) {
        if (DAT_0043096e == 1) {
          iVar2 = FUN_0041bac0(*param_2);
          iVar2 = FUN_0041760c(*param_2,iVar2);
          if (iVar2 == 4) {
            iVar2 = FUN_004176f8(*param_2,3);
            FUN_0041b9f8(&local_11c,local_118 + iVar2 + -1);
            uVar1 = FUN_0041775c(local_11c,2);
            FUN_0041b928(param_2,(undefined4 *)(local_118 + iVar2 + -1),uVar1);
          }
        }
      }
      else if (DAT_0043096c == 0x11) {
        piVar3 = param_2;
        uVar1 = FUN_0041775c(*param_2,1);
        FUN_0041bcb8(*param_2,1,uVar1,piVar3);
      }
    }
  }
  *in_FS_OFFSET = uStack_130;
  puStack_128 = &LAB_0041526d;
  puStack_12c = (undefined1 *)0x41525d;
  FUN_0041b840(&local_11c);
  puStack_12c = (undefined1 *)0x415265;
  FUN_0041b840((int *)&local_8);
  return;
}



void FUN_00415280(int param_1,int *param_2,undefined4 param_3,int param_4)

{
  LPCSTR lpFormat;
  LCID Locale;
  undefined4 *in_FS_OFFSET;
  DWORD dwFlags;
  SYSTEMTIME *lpDate;
  CHAR *lpDateStr;
  int iVar1;
  int *piVar2;
  undefined4 uStack_12c;
  undefined1 *puStack_128;
  undefined1 *puStack_124;
  CHAR local_118 [256];
  SYSTEMTIME local_18;
  undefined *local_8;
  
  puStack_124 = &stack0xfffffffc;
  local_8 = (undefined *)0x0;
  puStack_128 = &LAB_0041535e;
  uStack_12c = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_12c;
  FUN_0041b840(param_2);
  local_18.wYear = *(WORD *)(param_4 + -0xe);
  local_18.wMonth = *(WORD *)(param_4 + -0x10);
  local_18.wDay = *(WORD *)(param_4 + -0x12);
  if (param_1 < 3) {
    FUN_0041b8d8((int *)&local_8,0x415374);
  }
  else {
    FUN_0041b8d8((int *)&local_8,0x415380);
  }
  iVar1 = 0x100;
  lpDateStr = local_118;
  lpFormat = FUN_0041bc74(local_8);
  lpDate = &local_18;
  dwFlags = 4;
  Locale = GetThreadLocale();
  iVar1 = GetDateFormatA(Locale,dwFlags,lpDate,lpFormat,lpDateStr,iVar1);
  if (iVar1 != 0) {
    FUN_0041ba70(param_2,local_118,0x100);
    if ((param_1 == 1) && (*(char *)*param_2 == '0')) {
      piVar2 = param_2;
      iVar1 = FUN_0041bac0(*param_2);
      FUN_0041bcb8(*param_2,2,iVar1 - 1,piVar2);
    }
  }
  *in_FS_OFFSET = uStack_12c;
  puStack_124 = &LAB_00415365;
  puStack_128 = (undefined1 *)0x41535d;
  FUN_0041b840((int *)&local_8);
  return;
}



void FUN_00415388(byte *param_1,undefined4 param_2,undefined1 *param_3,undefined1 *param_4)

{
  byte bVar1;
  byte bVar2;
  ushort uVar3;
  int iVar4;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  undefined1 *extraout_ECX_01;
  undefined1 *extraout_ECX_02;
  undefined1 *extraout_ECX_03;
  undefined1 *puVar5;
  undefined4 extraout_ECX_04;
  undefined4 extraout_ECX_05;
  undefined4 extraout_ECX_06;
  undefined4 extraout_ECX_07;
  undefined4 extraout_ECX_08;
  undefined4 extraout_ECX_09;
  undefined4 extraout_EDX;
  undefined4 extraout_EDX_00;
  undefined4 extraout_EDX_01;
  undefined4 extraout_EDX_02;
  undefined4 extraout_EDX_03;
  undefined4 extraout_EDX_04;
  undefined4 extraout_EDX_05;
  undefined4 extraout_EDX_06;
  undefined4 extraout_EDX_07;
  undefined4 extraout_EDX_08;
  undefined4 extraout_EDX_09;
  undefined4 extraout_EDX_10;
  undefined4 extraout_EDX_11;
  undefined4 extraout_EDX_12;
  byte bVar6;
  byte bVar7;
  byte *pbVar8;
  byte *pbVar9;
  undefined4 *in_FS_OFFSET;
  undefined1 *puVar10;
  undefined4 uStack_40;
  undefined1 *puStack_3c;
  undefined1 *puStack_38;
  undefined4 *local_2c;
  undefined4 *local_28;
  byte local_23;
  char local_22;
  undefined1 local_21;
  ushort local_20;
  ushort local_1e;
  ushort local_1c;
  ushort local_1a;
  undefined1 local_17;
  ushort local_16;
  ushort local_14;
  ushort local_12;
  int local_10;
  byte local_9;
  byte *local_8;
  
  puStack_38 = &stack0xfffffffc;
  local_2c = (undefined4 *)0x0;
  local_28 = (undefined4 *)0x0;
  puStack_3c = &LAB_00415ac9;
  uStack_40 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_40;
  local_8 = param_1;
  if ((param_1 != (byte *)0x0) && (*(int *)(param_4 + -0x108) < 2)) {
    *(int *)(param_4 + -0x108) = *(int *)(param_4 + -0x108) + 1;
    local_17 = 0;
    local_21 = 0;
    local_22 = '\0';
    bVar7 = 0x20;
    while (bVar1 = *local_8, pbVar8 = local_8, bVar1 != 0) {
      pbVar8 = local_8 + 1;
      local_9 = bVar1;
      if (((byte)(&DAT_0042d4c4)[(int)(uint)bVar1 >> 3] >> (bVar1 & 7) & 1) == 0) {
        bVar2 = bVar1;
        if ((byte)(bVar1 + 0x9f) < 0x1a) {
          bVar2 = bVar1 - 0x20;
        }
        bVar6 = bVar7;
        if ((((byte)(bVar2 + 0xbf) < 0x1a) && (bVar6 = bVar2, bVar2 == 0x4d)) && (bVar7 == 0x48)) {
          bVar2 = 0x4e;
          bVar6 = bVar2;
        }
        bVar7 = bVar6;
        local_8 = pbVar8;
        switch(bVar2) {
        case 0x22:
        case 0x27:
          while( true ) {
            bVar2 = *local_8;
            if (((bVar2 == 0) || (bVar2 == bVar1)) ||
               ((((byte)(&DAT_0042d4c4)[(int)(uint)bVar2 >> 3] >> (bVar2 & 7) & 1) != 0 &&
                (local_8 = local_8 + 1, *local_8 == 0)))) break;
            local_8 = local_8 + 1;
          }
          puVar5 = param_4;
          FUN_00414fc0((undefined4 *)pbVar8,(int)local_8 - (int)pbVar8,param_3,(int)param_4);
          param_3 = puVar5;
          if (*local_8 != 0) {
            local_8 = local_8 + 1;
          }
          break;
        default:
          puVar5 = param_4;
          FUN_00414fc0((undefined4 *)&local_9,1,param_3,(int)param_4);
          param_3 = puVar5;
          break;
        case 0x2f:
          puVar5 = param_4;
          FUN_00414fc0((undefined4 *)&DAT_004308b1,1,param_3,(int)param_4);
          param_3 = puVar5;
          break;
        case 0x3a:
          puVar5 = param_4;
          FUN_00414fc0((undefined4 *)&DAT_004308bc,1,param_3,(int)param_4);
          param_3 = puVar5;
          break;
        case 0x41:
          FUN_004150d4((int)&stack0xfffffffc);
          pbVar8 = local_8;
          pbVar9 = local_8 + -1;
          iVar4 = FUN_00414510((char *)pbVar9,"AM/PM",5);
          if (iVar4 == 0) {
            if (0xb < local_1a) {
              pbVar9 = pbVar8 + 2;
            }
            param_3 = param_4;
            FUN_00414fc0((undefined4 *)pbVar9,2,extraout_ECX_04,(int)param_4);
            local_8 = local_8 + 4;
            local_22 = '\x01';
          }
          else {
            iVar4 = FUN_00414510((char *)pbVar9,"A/P",3);
            if (iVar4 == 0) {
              if (0xb < local_1a) {
                pbVar9 = pbVar8 + 1;
              }
              param_3 = param_4;
              FUN_00414fc0((undefined4 *)pbVar9,1,extraout_ECX_05,(int)param_4);
              local_8 = local_8 + 2;
              local_22 = '\x01';
            }
            else {
              iVar4 = FUN_00414510((char *)pbVar9,"AMPM",4);
              if (iVar4 == 0) {
                if (local_1a < 0xc) {
                  param_3 = param_4;
                  FUN_00415004(DAT_004308c0,extraout_EDX_06,extraout_ECX_06,(int)param_4);
                }
                else {
                  param_3 = param_4;
                  FUN_00415004(DAT_004308c4,extraout_EDX_06,extraout_ECX_06,(int)param_4);
                }
                local_8 = local_8 + 3;
                local_22 = '\x01';
              }
              else {
                iVar4 = FUN_00414510((char *)pbVar9,"AAAA",4);
                if (iVar4 == 0) {
                  puVar5 = &stack0xfffffffc;
                  FUN_0041509c((int)&stack0xfffffffc);
                  param_3 = param_4;
                  iVar4 = FUN_00414f84(param_4,extraout_EDX_07,puVar5,*(undefined4 *)(param_4 + 8),
                                       *(undefined4 *)(param_4 + 0xc));
                  FUN_00415004(*(undefined4 **)(&DAT_00430948 + iVar4 * 4),extraout_EDX_08,
                               extraout_ECX_07,(int)param_3);
                  local_8 = local_8 + 3;
                }
                else {
                  iVar4 = FUN_00414510((char *)pbVar9,"AAA",3);
                  if (iVar4 == 0) {
                    puVar5 = &stack0xfffffffc;
                    FUN_0041509c((int)&stack0xfffffffc);
                    param_3 = param_4;
                    iVar4 = FUN_00414f84(param_4,extraout_EDX_09,puVar5,*(undefined4 *)(param_4 + 8)
                                         ,*(undefined4 *)(param_4 + 0xc));
                    FUN_00415004(*(undefined4 **)(&DAT_0043092c + iVar4 * 4),extraout_EDX_10,
                                 extraout_ECX_09,(int)param_3);
                    local_8 = local_8 + 2;
                  }
                  else {
                    param_3 = param_4;
                    FUN_00414fc0((undefined4 *)&local_9,1,extraout_ECX_08,(int)param_4);
                  }
                }
              }
            }
          }
          break;
        case 0x43:
          puVar5 = &stack0xfffffffc;
          FUN_00415070((int)&stack0xfffffffc);
          FUN_00415388(DAT_004308b4,extraout_EDX_11,puVar5,param_4);
          param_3 = &stack0xfffffffc;
          FUN_004150d4((int)&stack0xfffffffc);
          if (((local_1a != 0) || (local_1c != 0)) || (local_1e != 0)) {
            puVar5 = param_4;
            FUN_00414fc0((undefined4 *)&DAT_00415af8,1,param_3,(int)param_4);
            param_3 = param_4;
            FUN_00415388(DAT_004308cc,extraout_EDX_12,puVar5,param_4);
          }
          break;
        case 0x44:
          puVar5 = &stack0xfffffffc;
          FUN_00415070((int)&stack0xfffffffc);
          if (local_10 - 1U < 2) {
            puVar5 = &stack0xfffffffc;
            FUN_0041509c((int)&stack0xfffffffc);
            param_3 = param_4;
            FUN_00415024((uint)local_16,local_10,puVar5,(int)param_4);
          }
          else if (local_10 == 3) {
            param_3 = param_4;
            iVar4 = FUN_00414f84(param_4,extraout_EDX_02,puVar5,*(undefined4 *)(param_4 + 8),
                                 *(undefined4 *)(param_4 + 0xc));
            FUN_00415004(*(undefined4 **)(&DAT_0043092c + iVar4 * 4),extraout_EDX_03,extraout_ECX,
                         (int)param_3);
          }
          else if (local_10 == 4) {
            param_3 = param_4;
            iVar4 = FUN_00414f84(param_4,extraout_EDX_02,puVar5,*(undefined4 *)(param_4 + 8),
                                 *(undefined4 *)(param_4 + 0xc));
            FUN_00415004(*(undefined4 **)(&DAT_00430948 + iVar4 * 4),extraout_EDX_04,extraout_ECX_00
                         ,(int)param_3);
          }
          else if (local_10 == 5) {
            param_3 = param_4;
            FUN_00415388(DAT_004308b4,extraout_EDX_02,puVar5,param_4);
          }
          else {
            param_3 = param_4;
            FUN_00415388(DAT_004308b8,extraout_EDX_02,puVar5,param_4);
          }
          break;
        case 0x45:
          FUN_00415070((int)&stack0xfffffffc);
          puVar10 = &stack0xfffffffc;
          FUN_0041509c((int)&stack0xfffffffc);
          puVar5 = &stack0xfffffffc;
          param_3 = param_4;
          FUN_00415280(local_10,(int *)&local_2c,puVar10,(int)&stack0xfffffffc);
          FUN_00415004(local_2c,extraout_EDX_00,puVar5,(int)param_3);
          break;
        case 0x47:
          FUN_00415070((int)&stack0xfffffffc);
          puVar10 = &stack0xfffffffc;
          FUN_0041509c((int)&stack0xfffffffc);
          puVar5 = &stack0xfffffffc;
          param_3 = param_4;
          FUN_00415114(local_10,(int *)&local_28,puVar10,(int)&stack0xfffffffc);
          FUN_00415004(local_28,extraout_EDX,puVar5,(int)param_3);
          break;
        case 0x48:
          FUN_00415070((int)&stack0xfffffffc);
          puVar5 = &stack0xfffffffc;
          FUN_004150d4((int)&stack0xfffffffc);
          local_23 = 0;
          pbVar8 = local_8;
          while (bVar1 = *pbVar8, bVar1 != 0) {
            if (((byte)(&DAT_0042d4c4)[(int)(uint)bVar1 >> 3] >> (bVar1 & 7) & 1) == 0) {
              bVar1 = *pbVar8;
              if (bVar1 < 0x49) {
                if (bVar1 == 0x48) break;
                if ((bVar1 == 0x22) || (bVar1 == 0x27)) {
                  local_23 = local_23 ^ 1;
                }
                else if (bVar1 == 0x41) goto LAB_004156db;
              }
              else if (bVar1 == 0x61) {
LAB_004156db:
                if (local_23 == 0) {
                  iVar4 = FUN_00414510((char *)pbVar8,"AM/PM",5);
                  puVar5 = extraout_ECX_01;
                  if (((iVar4 == 0) ||
                      (iVar4 = FUN_00414510((char *)pbVar8,"A/P",3), puVar5 = extraout_ECX_02,
                      iVar4 == 0)) ||
                     (iVar4 = FUN_00414510((char *)pbVar8,"AMPM",4), puVar5 = extraout_ECX_03,
                     iVar4 == 0)) {
                    local_22 = '\x01';
                  }
                  break;
                }
              }
              else if (bVar1 == 0x68) break;
              pbVar8 = pbVar8 + 1;
            }
            else {
              if (pbVar8[1] == 0) break;
              pbVar8 = pbVar8 + 2;
            }
          }
          uVar3 = local_1a;
          if (local_22 != '\0') {
            if (local_1a == 0) {
              uVar3 = 0xc;
            }
            else if (0xc < local_1a) {
              uVar3 = local_1a - 0xc;
            }
          }
          if (2 < local_10) {
            local_10 = 2;
          }
          param_3 = param_4;
          FUN_00415024((uint)uVar3,local_10,puVar5,(int)param_4);
          break;
        case 0x4d:
          FUN_00415070((int)&stack0xfffffffc);
          puVar5 = &stack0xfffffffc;
          FUN_0041509c((int)&stack0xfffffffc);
          if (local_10 - 1U < 2) {
            param_3 = param_4;
            FUN_00415024((uint)local_14,local_10,puVar5,(int)param_4);
          }
          else if (local_10 - 1U == 2) {
            param_3 = param_4;
            FUN_00415004((undefined4 *)(&DAT_004308cc)[local_14],extraout_EDX_01,puVar5,(int)param_4
                        );
          }
          else {
            param_3 = param_4;
            FUN_00415004(*(undefined4 **)(&DAT_004308fc + (uint)local_14 * 4),extraout_EDX_01,puVar5
                         ,(int)param_4);
          }
          break;
        case 0x4e:
          FUN_00415070((int)&stack0xfffffffc);
          puVar5 = &stack0xfffffffc;
          FUN_004150d4((int)&stack0xfffffffc);
          if (2 < local_10) {
            local_10 = 2;
          }
          param_3 = param_4;
          FUN_00415024((uint)local_1c,local_10,puVar5,(int)param_4);
          break;
        case 0x53:
          FUN_00415070((int)&stack0xfffffffc);
          puVar5 = &stack0xfffffffc;
          FUN_004150d4((int)&stack0xfffffffc);
          if (2 < local_10) {
            local_10 = 2;
          }
          param_3 = param_4;
          FUN_00415024((uint)local_1e,local_10,puVar5,(int)param_4);
          break;
        case 0x54:
          puVar5 = &stack0xfffffffc;
          FUN_00415070((int)&stack0xfffffffc);
          if (local_10 == 1) {
            param_3 = param_4;
            FUN_00415388(DAT_004308c8,extraout_EDX_05,puVar5,param_4);
          }
          else {
            param_3 = param_4;
            FUN_00415388(DAT_004308cc,extraout_EDX_05,puVar5,param_4);
          }
          break;
        case 0x59:
          FUN_00415070((int)&stack0xfffffffc);
          puVar5 = &stack0xfffffffc;
          FUN_0041509c((int)&stack0xfffffffc);
          if (local_10 < 3) {
            param_3 = param_4;
            FUN_00415024((uint)local_12 % 100,2,100,(int)param_4);
          }
          else {
            param_3 = param_4;
            FUN_00415024((uint)local_12,4,puVar5,(int)param_4);
          }
          break;
        case 0x5a:
          FUN_00415070((int)&stack0xfffffffc);
          puVar5 = &stack0xfffffffc;
          FUN_004150d4((int)&stack0xfffffffc);
          if (3 < local_10) {
            local_10 = 3;
          }
          param_3 = param_4;
          FUN_00415024((uint)local_20,local_10,puVar5,(int)param_4);
        }
      }
      else {
        if (*pbVar8 == 0) break;
        local_8 = local_8 + 2;
        bVar7 = 0x20;
      }
    }
    local_8 = pbVar8;
    *(int *)(param_4 + -0x108) = *(int *)(param_4 + -0x108) + -1;
  }
  *in_FS_OFFSET = uStack_40;
  puStack_38 = &LAB_00415ad0;
  puStack_3c = (undefined1 *)0x415ac8;
  FUN_0041b864((int *)&local_2c,2);
  return;
}



void FUN_00415afc(int *param_1,byte *param_2,undefined1 *param_3)

{
  undefined4 local_104 [64];
  
  if (param_2 == (byte *)0x0) {
    FUN_00415388(&DAT_00415b54,0,param_3,&stack0xfffffffc);
  }
  else {
    FUN_00415388(param_2,param_2,param_3,&stack0xfffffffc);
  }
  FUN_0041b928(param_1,local_104,0);
  return;
}



void FUN_00415b58(int *param_1,undefined4 param_2,undefined1 *param_3)

{
  FUN_00415afc(param_1,DAT_004308b4,param_3);
  return;
}



void FUN_00415b70(byte *param_1,int *param_2,undefined1 *param_3)

{
  FUN_00415afc(param_2,param_1,param_3);
  return;
}



void FUN_00415b84(int param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  
  iVar2 = *param_2;
  while( true ) {
    iVar1 = FUN_0041bac0(param_1);
    if ((iVar1 < iVar2) || (*(char *)(param_1 + -1 + iVar2) != ' ')) break;
    iVar2 = iVar2 + 1;
  }
  *param_2 = iVar2;
  return;
}



bool FUN_00415ba8(int param_1,int *param_2,ushort *param_3,char *param_4)

{
  ushort uVar1;
  int iVar2;
  int iVar3;
  
  *param_4 = '\0';
  FUN_00415b84(param_1,param_2);
  iVar3 = *param_2;
  uVar1 = 0;
  while( true ) {
    iVar2 = FUN_0041bac0(param_1);
    if (((iVar2 < iVar3) || (9 < (byte)(*(char *)(param_1 + -1 + iVar3) - 0x30U))) || (999 < uVar1))
    break;
    uVar1 = uVar1 * 10 + (*(byte *)(param_1 + -1 + iVar3) - 0x30);
    iVar3 = iVar3 + 1;
  }
  iVar2 = *param_2;
  if (iVar2 < iVar3) {
    *param_4 = (char)iVar3 - (char)*param_2;
    *param_2 = iVar3;
    *param_3 = uVar1;
  }
  return iVar2 < iVar3;
}



void FUN_00415c3c(int param_1,int *param_2,undefined *param_3)

{
  uint uVar1;
  int iVar2;
  undefined4 *in_FS_OFFSET;
  undefined **ppuVar3;
  undefined4 uStack_24;
  undefined1 *puStack_20;
  undefined1 *puStack_1c;
  undefined *local_c;
  int local_8;
  
  puStack_1c = &stack0xfffffffc;
  local_c = (undefined *)0x0;
  puStack_20 = &LAB_00415cb5;
  uStack_24 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_24;
  local_8 = param_1;
  if (param_3 != (undefined *)0x0) {
    FUN_00415b84(param_1,param_2);
    ppuVar3 = &local_c;
    uVar1 = FUN_0041bac0((int)param_3);
    FUN_0041bcb8(local_8,*param_2,uVar1,(int *)ppuVar3);
    iVar2 = FUN_0041411c(param_3,local_c);
    if (iVar2 == 0) {
      iVar2 = FUN_0041bac0((int)param_3);
      *param_2 = *param_2 + iVar2;
    }
  }
  *in_FS_OFFSET = uStack_24;
  puStack_1c = &LAB_00415cbc;
  puStack_20 = (undefined1 *)0x415cb4;
  FUN_0041b840((int *)&local_c);
  return;
}



undefined4 FUN_00415cc8(int param_1,int *param_2,char param_3)

{
  int iVar1;
  undefined4 uVar2;
  
  uVar2 = 0;
  FUN_00415b84(param_1,param_2);
  iVar1 = FUN_0041bac0(param_1);
  if ((*param_2 <= iVar1) && (*(char *)(param_1 + -1 + *param_2) == param_3)) {
    *param_2 = *param_2 + 1;
    uVar2 = 1;
  }
  return uVar2;
}



undefined4 FUN_00415d00(int param_1)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = 1;
  while( true ) {
    iVar2 = FUN_0041bac0(param_1);
    if (iVar2 < iVar3) {
      return 0;
    }
    bVar1 = *(byte *)(param_1 + -1 + iVar3) & 0xdf;
    if (bVar1 == 0x44) {
      return 1;
    }
    if (bVar1 == 0x45) {
      return 2;
    }
    if (bVar1 == 0x4d) break;
    if (bVar1 == 0x59) {
      return 2;
    }
    iVar3 = iVar3 + 1;
  }
  return 0;
}



int FUN_00415d4c(int param_1,int *param_2)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  
  while( true ) {
    iVar3 = FUN_0041bac0(param_1);
    if ((iVar3 < *param_2) ||
       (cVar1 = *(char *)(param_1 + -1 + *param_2),
       iVar3 = CONCAT31((int3)((uint)*param_2 >> 8),cVar1 + -0x3a), (byte)(cVar1 - 0x30U) < 10))
    break;
    uVar2 = (uint)*(byte *)(param_1 + -1 + *param_2);
    if (((byte)(&DAT_0042d4c4)[(int)uVar2 >> 3] >> (uVar2 & 7) & 1) != 0) {
      *param_2 = *param_2 + 1;
    }
    *param_2 = *param_2 + 1;
  }
  return iVar3;
}



undefined4 FUN_00415d88(undefined *param_1)

{
  char *pcVar1;
  byte *pbVar2;
  PCNZCH pCVar3;
  int *piVar4;
  undefined4 *puVar5;
  int iVar6;
  
  iVar6 = 7;
  piVar4 = &DAT_00430974;
  puVar5 = &DAT_00430990;
  while( true ) {
    if (*piVar4 == 0) {
      return 0;
    }
    pcVar1 = FUN_0041bc74(param_1);
    pbVar2 = FUN_0041bc74((undefined *)*piVar4);
    pCVar3 = FUN_004177f4(pbVar2,pcVar1);
    if (pCVar3 != (PCNZCH)0x0) break;
    puVar5 = puVar5 + 1;
    piVar4 = piVar4 + 1;
    iVar6 = iVar6 + -1;
    if (iVar6 == 0) {
      return 0;
    }
  }
  return *puVar5;
}



int FUN_00415ddc(int param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  WORD WVar1;
  uint uVar2;
  
  if (DAT_0043096c == 0x12) {
    if (param_1 < 100) {
      WVar1 = FUN_00414fac();
      uVar2 = (int)*(uint *)(param_4 + -4) >> 0x1f;
      param_1 = param_1 + ((int)((uint)WVar1 + ((*(uint *)(param_4 + -4) ^ uVar2) - uVar2)) / 100) *
                          100;
    }
    if (0 < *(int *)(param_4 + -4)) {
      *(int *)(param_4 + -4) = -*(int *)(param_4 + -4);
    }
  }
  else {
    *(int *)(param_4 + -4) = *(int *)(param_4 + -4) + -1;
  }
  return *(int *)(param_4 + -4) + param_1;
}



void FUN_00415e38(int param_1,int *param_2,double *param_3)

{
  bool bVar1;
  WORD WVar2;
  undefined4 uVar3;
  PCNZCH pCVar4;
  undefined2 extraout_var;
  int iVar5;
  uint uVar6;
  undefined4 extraout_ECX;
  undefined4 extraout_EDX;
  ushort uVar7;
  uint uVar8;
  undefined4 *in_FS_OFFSET;
  undefined **ppuVar9;
  undefined4 uStack_44;
  undefined1 *puStack_40;
  undefined1 *puStack_3c;
  undefined *local_2c;
  undefined *local_28;
  int local_24;
  undefined *local_20;
  byte local_1c;
  byte local_1b;
  char local_1a;
  byte local_19;
  ushort local_18;
  ushort local_16;
  ushort local_14;
  ushort local_12;
  ushort local_10;
  char local_e;
  undefined1 local_d;
  double *local_c;
  int local_8;
  
  puStack_3c = &stack0xfffffffc;
  local_2c = (undefined *)0x0;
  local_28 = (undefined *)0x0;
  local_24 = 0;
  local_20 = (undefined *)0x0;
  puStack_40 = &LAB_0041614a;
  uStack_44 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_44;
  uVar8 = 0;
  local_16 = 0;
  local_18 = 0;
  local_1c = 0;
  local_d = 0;
  local_c = param_3;
  uVar3 = FUN_00415d00((int)DAT_004308b4);
  local_e = (char)uVar3;
  local_8 = 0;
  if (*DAT_004308b4 == 'g') {
    FUN_00415d4c(param_1,param_2);
    FUN_0041bcb8(param_1,1,*param_2 - 1,&local_24);
    FUN_00414154(local_24,(int *)&local_20);
    local_8 = FUN_00415d88(local_20);
  }
  else {
    pCVar4 = FUN_004177bc(&DAT_00416164,DAT_004308b4);
    if (0 < (int)pCVar4) {
      local_8 = DAT_00430990;
    }
  }
  bVar1 = FUN_00415ba8(param_1,param_2,&local_10,(char *)&local_19);
  if (((bVar1) && (uVar3 = FUN_00415cc8(param_1,param_2,DAT_004308b1), (char)uVar3 != '\0')) &&
     (bVar1 = FUN_00415ba8(param_1,param_2,&local_12,&local_1a), bVar1)) {
    uVar3 = FUN_00415cc8(param_1,param_2,DAT_004308b1);
    if ((char)uVar3 == '\0') {
      WVar2 = FUN_00414fac();
      uVar8 = CONCAT22(extraout_var,WVar2);
      if (local_e == '\x01') {
        local_18 = local_10;
        local_16 = local_12;
      }
      else {
        local_16 = local_10;
        local_18 = local_12;
      }
    }
    else {
      bVar1 = FUN_00415ba8(param_1,param_2,&local_14,(char *)&local_1b);
      if (!bVar1) goto LAB_0041612f;
      if (local_e == '\0') {
        uVar8 = (uint)local_14;
        local_1c = local_1b;
        local_16 = local_10;
        local_18 = local_12;
      }
      else if (local_e == '\x01') {
        uVar8 = (uint)local_14;
        local_1c = local_1b;
        local_16 = local_12;
        local_18 = local_10;
      }
      else if (local_e == '\x02') {
        uVar8 = (uint)local_10;
        local_1c = local_19;
        local_16 = local_12;
        local_18 = local_14;
      }
      if (local_8 < 1) {
        if (local_1c < 3) {
          WVar2 = FUN_00414fac();
          uVar7 = (short)uVar8 + (short)((int)((uint)WVar2 - (uint)DAT_0042d490) / 100) * 100;
          uVar8 = (uint)uVar7;
          if ((DAT_0042d490 != 0) && ((int)(uint)uVar7 < (int)((uint)WVar2 - (uint)DAT_0042d490))) {
            uVar8 = (uint)(ushort)(uVar7 + 100);
          }
        }
      }
      else {
        uVar8 = FUN_00415ddc(uVar8,extraout_EDX,extraout_ECX,(int)&stack0xfffffffc);
      }
    }
    FUN_00415cc8(param_1,param_2,DAT_004308b1);
    FUN_00415b84(param_1,param_2);
    if ((DAT_00430970 != '\0') && (iVar5 = FUN_0041bd40("ddd",DAT_004308b4), iVar5 != 0)) {
      if ((byte)(*DAT_004308c8 - 0x30U) < 10) {
        FUN_00415d4c(param_1,param_2);
      }
      else {
        do {
          while ((iVar5 = FUN_0041bac0(param_1), *param_2 <= iVar5 &&
                 (*(char *)(param_1 + -1 + *param_2) != ' '))) {
            *param_2 = *param_2 + 1;
          }
          FUN_00415b84(param_1,param_2);
          iVar5 = FUN_0041bac0(param_1);
          if (iVar5 < *param_2) break;
          ppuVar9 = &local_28;
          uVar6 = FUN_0041bac0((int)DAT_004308c0);
          FUN_0041bcb8(param_1,*param_2,uVar6,(int *)ppuVar9);
          iVar5 = FUN_0041411c(DAT_004308c0,local_28);
          if (iVar5 == 0) break;
          ppuVar9 = &local_2c;
          uVar6 = FUN_0041bac0((int)DAT_004308c4);
          FUN_0041bcb8(param_1,*param_2,uVar6,(int *)ppuVar9);
          iVar5 = FUN_0041411c(DAT_004308c4,local_2c);
        } while (iVar5 != 0);
      }
    }
    local_d = FUN_00414ce4(uVar8,local_16,local_18,local_c);
  }
LAB_0041612f:
  *in_FS_OFFSET = uStack_44;
  puStack_3c = &LAB_00416151;
  puStack_40 = (undefined1 *)0x416149;
  FUN_0041b864((int *)&local_2c,4);
  return;
}



undefined1 FUN_00416174(int param_1,int *param_2,double *param_3)

{
  char cVar1;
  bool bVar2;
  undefined1 uVar3;
  undefined4 uVar4;
  int iVar5;
  ushort local_1c;
  ushort local_1a;
  ushort local_18;
  ushort local_16;
  char local_14 [4];
  
  iVar5 = -1;
  cVar1 = FUN_00415c3c(param_1,param_2,DAT_004308c0);
  if ((cVar1 == '\0') && (cVar1 = FUN_00415c3c(param_1,param_2,&DAT_00416358), cVar1 == '\0')) {
    cVar1 = FUN_00415c3c(param_1,param_2,DAT_004308c4);
    if ((cVar1 != '\0') || (cVar1 = FUN_00415c3c(param_1,param_2,&DAT_00416364), cVar1 != '\0')) {
      iVar5 = 0xc;
    }
  }
  else {
    iVar5 = 0;
  }
  if (-1 < iVar5) {
    FUN_00415b84(param_1,param_2);
  }
  bVar2 = FUN_00415ba8(param_1,param_2,&local_1c,local_14);
  if (!bVar2) {
    return 0;
  }
  local_1a = 0;
  uVar4 = FUN_00415cc8(param_1,param_2,DAT_004308bc);
  if (((char)uVar4 != '\0') && (bVar2 = FUN_00415ba8(param_1,param_2,&local_1a,local_14), !bVar2)) {
    return 0;
  }
  local_18 = 0;
  uVar4 = FUN_00415cc8(param_1,param_2,DAT_004308bc);
  if (((char)uVar4 != '\0') && (bVar2 = FUN_00415ba8(param_1,param_2,&local_18,local_14), !bVar2)) {
    return 0;
  }
  local_16 = 0;
  uVar4 = FUN_00415cc8(param_1,param_2,DAT_004308af);
  if (((char)uVar4 != '\0') && (bVar2 = FUN_00415ba8(param_1,param_2,&local_16,local_14), !bVar2)) {
    return 0;
  }
  if (iVar5 < 0) {
    cVar1 = FUN_00415c3c(param_1,param_2,DAT_004308c0);
    if ((cVar1 == '\0') && (cVar1 = FUN_00415c3c(param_1,param_2,&DAT_00416358), cVar1 == '\0')) {
      cVar1 = FUN_00415c3c(param_1,param_2,DAT_004308c4);
      if ((cVar1 != '\0') || (cVar1 = FUN_00415c3c(param_1,param_2,&DAT_00416364), cVar1 != '\0')) {
        iVar5 = 0xc;
      }
    }
    else {
      iVar5 = 0;
    }
  }
  if (-1 < iVar5) {
    if (local_1c == 0) {
      return 0;
    }
    if (0xc < local_1c) {
      return 0;
    }
    if (local_1c == 0xc) {
      local_1c = 0;
    }
    local_1c = local_1c + (short)iVar5;
  }
  FUN_00415b84(param_1,param_2);
  uVar3 = FUN_00414bdc(local_1c,local_1a,local_18,param_3,local_16);
  return uVar3;
}



void FUN_00416368(int param_1)

{
  char cVar1;
  int iVar2;
  double local_18;
  int local_10;
  int local_c;
  undefined1 local_8;
  
  local_10 = 1;
  cVar1 = FUN_00415e38(param_1,&local_10,&local_18);
  if ((cVar1 != '\0') && (iVar2 = FUN_0041bac0(param_1), iVar2 < local_10)) {
    return;
  }
  local_8 = 0xb;
  local_c = param_1;
  FUN_00414044((undefined4 *)PTR_PTR_004306d4,&local_c,0);
  return;
}



void FUN_004163b8(int param_1)

{
  char cVar1;
  int iVar2;
  double local_18;
  int local_10;
  int local_c;
  undefined1 local_8;
  
  local_10 = 1;
  cVar1 = FUN_00416174(param_1,&local_10,&local_18);
  if ((cVar1 != '\0') && (iVar2 = FUN_0041bac0(param_1), iVar2 < local_10)) {
    return;
  }
  local_8 = 0xb;
  local_c = param_1;
  FUN_00414044((undefined4 *)PTR_PTR_004306d8,&local_c,0);
  return;
}



void FUN_00416408(int param_1)

{
  char cVar1;
  int iVar2;
  double local_2c;
  int local_24 [2];
  double local_1c;
  undefined4 local_14;
  undefined4 uStack_10;
  int local_c;
  undefined1 local_8;
  
  local_24[0] = 1;
  local_14 = 0;
  uStack_10 = 0;
  cVar1 = FUN_00415e38(param_1,local_24,&local_1c);
  if ((cVar1 == '\0') ||
     ((iVar2 = FUN_0041bac0(param_1), local_24[0] <= iVar2 &&
      (cVar1 = FUN_00416174(param_1,local_24,(double *)&local_14), cVar1 == '\0')))) {
    local_24[0] = 1;
    cVar1 = FUN_00416174(param_1,local_24,&local_2c);
    if ((cVar1 == '\0') || (iVar2 = FUN_0041bac0(param_1), local_24[0] <= iVar2)) {
      local_8 = 0xb;
      local_c = param_1;
      FUN_00414044((undefined4 *)PTR_PTR_004306dc,&local_c,0);
    }
  }
  return;
}



void FUN_004164cc(DWORD param_1,int *param_2)

{
  uint uVar1;
  CHAR local_104 [256];
  
  uVar1 = FormatMessageA(0x3000,(LPCVOID)0x0,param_1,0,local_104,0x100,(va_list *)0x0);
  for (; (0 < (int)uVar1 && (((byte)local_104[uVar1 - 1] < 0x21 || (local_104[uVar1 - 1] == 0x2e))))
      ; uVar1 = uVar1 - 1) {
  }
  FUN_0041b928(param_2,(undefined4 *)local_104,uVar1);
  return;
}



void FUN_00416518(LCID param_1,LCTYPE param_2,undefined4 *param_3,int *param_4)

{
  int iVar1;
  CHAR local_104 [256];
  
  iVar1 = GetLocaleInfoA(param_1,param_2,local_104,0x100);
  if (iVar1 < 1) {
    FUN_0041b894(param_4,param_3);
  }
  else {
    FUN_0041b928(param_4,(undefined4 *)local_104,iVar1 - 1);
  }
  return;
}



uint FUN_00416564(LCID param_1,LCTYPE param_2,uint param_3)

{
  int iVar1;
  uint local_10;
  
  local_10 = param_3;
  iVar1 = GetLocaleInfoA(param_1,param_2,(LPSTR)&local_10,2);
  if (0 < iVar1) {
    param_3 = CONCAT31((int3)((uint)iVar1 >> 8),(undefined1)local_10);
  }
  return param_3;
}



void FUN_0041658c(LCTYPE param_1,int param_2,int param_3,int *param_4,undefined4 param_5,int param_6
                 )

{
  FUN_00416518(*(LCID *)(param_6 + -4),param_1,(undefined4 *)0x0,param_4);
  if (*param_4 == 0) {
    FUN_0041d2c4(*(undefined4 **)(param_3 + param_2 * 4),param_4);
  }
  return;
}



void __stdcall FUN_004165c8(void)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_34;
  undefined1 *puStack_30;
  undefined1 *puStack_2c;
  undefined4 *local_1c;
  undefined4 *local_18;
  undefined4 *local_14;
  undefined4 *local_10;
  int local_c;
  LCID local_8;
  
  puStack_2c = &stack0xfffffffc;
  local_8 = 0;
  local_c = 0;
  local_10 = (undefined4 *)0x0;
  local_14 = (undefined4 *)0x0;
  local_18 = (undefined4 *)0x0;
  local_1c = (undefined4 *)0x0;
  puStack_30 = &LAB_004166db;
  uStack_34 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_34;
  local_8 = GetThreadLocale();
  iVar1 = 1;
  piVar2 = (int *)&DAT_004308d0;
  piVar3 = (int *)&DAT_00430900;
  do {
    FUN_0041658c(iVar1 + 0x43,iVar1 + -1,0x42d51c,(int *)&local_10,0xb,(int)&stack0xfffffffc);
    FUN_0041b894(piVar2,local_10);
    FUN_0041658c(iVar1 + 0x37,iVar1 + -1,0x42d54c,(int *)&local_14,0xb,(int)&stack0xfffffffc);
    FUN_0041b894(piVar3,local_14);
    iVar1 = iVar1 + 1;
    piVar3 = piVar3 + 1;
    piVar2 = piVar2 + 1;
  } while (iVar1 != 0xd);
  iVar1 = 1;
  piVar2 = (int *)&DAT_00430930;
  piVar3 = (int *)&DAT_0043094c;
  do {
    local_c = (iVar1 + 5) % 7;
    FUN_0041658c(local_c + 0x31,iVar1 + -1,0x42d57c,(int *)&local_18,6,(int)&stack0xfffffffc);
    FUN_0041b894(piVar2,local_18);
    FUN_0041658c(local_c + 0x2a,iVar1 + -1,0x42d598,(int *)&local_1c,6,(int)&stack0xfffffffc);
    FUN_0041b894(piVar3,local_1c);
    iVar1 = iVar1 + 1;
    piVar3 = piVar3 + 1;
    piVar2 = piVar2 + 1;
  } while (iVar1 != 8);
  *in_FS_OFFSET = uStack_34;
  puStack_2c = &LAB_004166e2;
  puStack_30 = (undefined1 *)0x4166da;
  FUN_0041b864((int *)&local_1c,4);
  return;
}



// lpCalInfoEnumProc parameter of EnumCalendarInfoA
// 

undefined4 __stdcall lpCalInfoEnumProc_004166ec(char *param_1)

{
  int iVar1;
  
  iVar1 = 1;
  while( true ) {
    if (*(int *)(&DAT_00430970 + iVar1 * 4) == 0) {
      FUN_0041b9f8((int *)(&DAT_00430970 + iVar1 * 4),param_1);
      return 1;
    }
    if (iVar1 == 7) break;
    iVar1 = iVar1 + 1;
  }
  return 0;
}



// lpCalInfoEnumProc parameter of EnumCalendarInfoA
// 

void __cdecl lpCalInfoEnumProc_00416728(char *param_1)

{
  undefined4 uVar1;
  int extraout_ECX;
  int iVar2;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_1c;
  undefined1 *puStack_18;
  undefined1 *puStack_14;
  byte *local_8;
  
  puStack_14 = &stack0xfffffffc;
  local_8 = (byte *)0x0;
  puStack_18 = &LAB_0041678d;
  uStack_1c = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_1c;
  for (iVar2 = 1; *(int *)(iVar2 * 4 + 0x43098c) != -1; iVar2 = iVar2 + 1) {
    if (iVar2 == 7) goto LAB_00416777;
  }
  FUN_0041b9f8((int *)&local_8,param_1);
  uVar1 = FUN_00414274(local_8,0,extraout_ECX);
  *(undefined4 *)(iVar2 * 4 + 0x43098c) = uVar1;
LAB_00416777:
  *in_FS_OFFSET = uStack_1c;
  puStack_14 = &LAB_00416794;
  puStack_18 = (undefined1 *)0x41678c;
  FUN_0041b840((int *)&local_8);
  return;
}



void __stdcall FUN_004167a0(void)

{
  LCID LVar1;
  CALID Calendar;
  undefined4 *puVar2;
  int extraout_ECX;
  int iVar3;
  undefined4 *in_FS_OFFSET;
  CALID Calendar_00;
  byte **ppbVar4;
  CALTYPE CVar5;
  undefined4 uStack_18;
  undefined1 *puStack_14;
  undefined1 *puStack_10;
  byte *local_8;
  
  puStack_10 = &stack0xfffffffc;
  local_8 = (byte *)0x0;
  puStack_14 = &LAB_00416837;
  uStack_18 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_18;
  ppbVar4 = &local_8;
  LVar1 = GetThreadLocale();
  FUN_00416518(LVar1,0x100b,(undefined4 *)&DAT_0041684c,(int *)ppbVar4);
  Calendar = FUN_00414274(local_8,1,extraout_ECX);
  if (Calendar - 3 < 3) {
    CVar5 = 4;
    Calendar_00 = Calendar;
    LVar1 = GetThreadLocale();
    EnumCalendarInfoA(lpCalInfoEnumProc_004166ec,LVar1,Calendar_00,CVar5);
    iVar3 = 7;
    puVar2 = &DAT_00430990;
    do {
      *puVar2 = 0xffffffff;
      puVar2 = puVar2 + 1;
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
    CVar5 = 3;
    LVar1 = GetThreadLocale();
    EnumCalendarInfoA(lpCalInfoEnumProc_00416728,LVar1,Calendar,CVar5);
  }
  *in_FS_OFFSET = uStack_18;
  puStack_10 = &LAB_0041683e;
  puStack_14 = (undefined1 *)0x416836;
  FUN_0041b840((int *)&local_8);
  return;
}



void FUN_00416850(undefined4 *param_1,int *param_2)

{
  char cVar1;
  bool bVar2;
  LCID LVar3;
  int iVar4;
  uint uVar5;
  int extraout_ECX;
  undefined4 extraout_EDX;
  undefined4 extraout_EDX_00;
  int iVar6;
  undefined4 *in_FS_OFFSET;
  byte **ppbVar7;
  undefined4 uStack_2c;
  undefined1 *puStack_28;
  undefined1 *puStack_24;
  undefined4 *local_14;
  undefined4 *local_10;
  undefined4 *local_c;
  byte *local_8;
  
  puStack_24 = &stack0xfffffffc;
  local_8 = (byte *)0x0;
  local_c = (undefined4 *)0x0;
  local_10 = (undefined4 *)0x0;
  local_14 = (undefined4 *)0x0;
  puStack_28 = &LAB_00416a12;
  uStack_2c = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_2c;
  iVar6 = 1;
  FUN_0041b840(param_2);
  ppbVar7 = &local_8;
  LVar3 = GetThreadLocale();
  FUN_00416518(LVar3,0x1009,(undefined4 *)&DAT_00416a28,(int *)ppbVar7);
  iVar4 = FUN_00414274(local_8,1,extraout_ECX);
  if (iVar4 - 3U < 3) {
    while (iVar4 = FUN_0041bac0((int)param_1), iVar6 <= iVar4) {
      uVar5 = (uint)*(byte *)((int)param_1 + iVar6 + -1);
      if (((byte)(&DAT_0042d4c4)[(int)uVar5 >> 3] >> (uVar5 & 7) & 1) == 0) {
        iVar4 = FUN_00414510((char *)((int)param_1 + iVar6 + -1),"gg",2);
        if (iVar4 == 0) {
          FUN_0041bac8(param_2,(undefined4 *)&DAT_00416a38);
          iVar6 = iVar6 + 1;
        }
        else {
          iVar4 = FUN_00414510((char *)((int)param_1 + iVar6 + -1),"yyyy",4);
          if (iVar4 == 0) {
            FUN_0041bac8(param_2,(undefined4 *)&DAT_00416a4c);
            iVar6 = iVar6 + 3;
          }
          else {
            iVar4 = FUN_00414510((char *)((int)param_1 + iVar6 + -1),"yy",2);
            if (iVar4 == 0) {
              FUN_0041bac8(param_2,(undefined4 *)&DAT_00416a60);
              iVar6 = iVar6 + 1;
            }
            else {
              cVar1 = *(char *)((int)param_1 + iVar6 + -1);
              if ((cVar1 == 'Y') || (cVar1 == 'y')) {
                FUN_0041bac8(param_2,(undefined4 *)&DAT_00416a6c);
              }
              else {
                FUN_0041b9e8((int *)&local_14,
                             CONCAT31((int3)((uint)extraout_EDX_00 >> 8),
                                      *(undefined1 *)((int)param_1 + iVar6 + -1)));
                FUN_0041bac8(param_2,local_14);
              }
            }
          }
        }
        iVar6 = iVar6 + 1;
      }
      else {
        FUN_0041bcb8((int)param_1,iVar6,2,(int *)&local_10);
        FUN_0041bac8(param_2,local_10);
        iVar6 = iVar6 + 2;
      }
    }
  }
  else {
    if ((DAT_0043096c == 4) || ((ushort)(DAT_0043096c - 0x11U) < 2)) {
      bVar2 = true;
    }
    else {
      bVar2 = false;
    }
    if (bVar2) {
      for (; iVar4 = FUN_0041bac0((int)param_1), iVar6 <= iVar4; iVar6 = iVar6 + 1) {
        cVar1 = *(char *)((int)param_1 + iVar6 + -1);
        if ((cVar1 != 'G') && (cVar1 != 'g')) {
          FUN_0041b9e8((int *)&local_c,
                       CONCAT31((int3)((uint)extraout_EDX >> 8),
                                *(undefined1 *)((int)param_1 + iVar6 + -1)));
          FUN_0041bac8(param_2,local_c);
        }
      }
    }
    else {
      FUN_0041b894(param_2,param_1);
    }
  }
  *in_FS_OFFSET = uStack_2c;
  puStack_24 = &LAB_00416a19;
  puStack_28 = (undefined1 *)0x416a11;
  FUN_0041b864((int *)&local_14,4);
  return;
}



undefined4 __stdcall FUN_00416a70(void)

{
  int iVar1;
  
  iVar1 = FUN_0041aa40();
  if (iVar1 != 0) {
    iVar1 = FUN_0041aa40();
    return *(undefined4 *)(iVar1 + 8);
  }
  return 0;
}



int FUN_00416a88(int param_1)

{
  if (param_1 != 0) {
    param_1 = param_1 + -0x1000;
  }
  return param_1;
}



void FUN_00416a94(int *param_1,LPCVOID param_2,byte *param_3,byte *param_4)

{
  DWORD DVar1;
  char *pcVar2;
  undefined4 uVar3;
  int iVar4;
  HINSTANCE hInstance;
  undefined *puVar5;
  UINT uID;
  byte *lpBuffer;
  byte local_45c [256];
  byte *local_35c;
  undefined1 local_358;
  char *local_354;
  undefined1 local_350;
  int local_34c;
  undefined1 local_348;
  char *local_344;
  undefined1 local_340;
  undefined *local_33c;
  undefined1 local_338;
  _MEMORY_BASIC_INFORMATION local_334;
  byte local_316 [256];
  byte local_216 [261];
  char local_111 [261];
  int local_c;
  byte *local_8;
  
  local_8 = param_3;
  VirtualQuery(param_2,&local_334,0x1c);
  if (local_334.State == 0x1000) {
    DVar1 = GetModuleFileNameA(local_334.AllocationBase,(LPSTR)local_216,0x105);
    if (DVar1 != 0) {
      local_c = (int)param_2 - (int)local_334.AllocationBase;
      goto LAB_00416b0a;
    }
  }
  GetModuleFileNameA(DAT_004309d0,(LPSTR)local_216,0x105);
  local_c = FUN_00416a88((int)param_2);
LAB_00416b0a:
  pcVar2 = FUN_00417894(local_216,'\\');
  FUN_004144dc(local_111,pcVar2 + 1,0x104);
  pcVar2 = "";
  puVar5 = &DAT_00416c14;
  uVar3 = FUN_0041afcc(param_1,(int)&DAT_004135b0);
  if ((char)uVar3 != '\0') {
    pcVar2 = FUN_0041bc74((undefined *)param_1[1]);
    iVar4 = FUN_00414488(pcVar2);
    if ((iVar4 != 0) && (pcVar2[iVar4 + -1] != '.')) {
      puVar5 = &LAB_00416c18;
    }
  }
  iVar4 = 0x100;
  lpBuffer = local_316;
  uID = *(UINT *)(PTR_PTR_0043073c + 4);
  hInstance = (HINSTANCE)FUN_0041d144((int)DAT_004309d0);
  LoadStringA(hInstance,uID,(LPSTR)lpBuffer,iVar4);
  FUN_0041ada0(*param_1,local_45c);
  local_35c = local_45c;
  local_358 = 4;
  local_354 = local_111;
  local_350 = 6;
  local_34c = local_c;
  local_348 = 5;
  local_340 = 6;
  local_338 = 6;
  local_344 = pcVar2;
  local_33c = puVar5;
  FUN_00414a8c(local_8,param_4,local_316,4,&local_35c);
  FUN_00414488((char *)local_8);
  return;
}



int * FUN_00416c94(int *param_1,char param_2,undefined4 *param_3)

{
  undefined4 *extraout_ECX;
  char extraout_DL;
  char cVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 in_stack_ffffffe4;
  undefined4 in_stack_ffffffe8;
  
  cVar1 = '\0';
  if (param_2 != '\0') {
    param_1 = (int *)FUN_0041b0d0((int)param_1,param_2,param_3,in_stack_ffffffe4,in_stack_ffffffe8);
    param_3 = extraout_ECX;
    cVar1 = extraout_DL;
  }
  FUN_0041b894(param_1 + 1,param_3);
  if (cVar1 != '\0') {
    FUN_0041b128(param_1);
    *in_FS_OFFSET = in_stack_ffffffe4;
  }
  return param_1;
}



void FUN_00416cd0(int param_1,char param_2,byte *param_3,undefined4 param_4,undefined4 param_5)

{
  byte *extraout_ECX;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_30;
  undefined1 *puStack_2c;
  undefined1 *puStack_28;
  undefined4 in_stack_ffffffdc;
  undefined4 in_stack_ffffffe0;
  undefined4 *local_8;
  
  local_8 = (undefined4 *)0x0;
  if (param_2 != '\0') {
    puStack_28 = (undefined1 *)0x416ce4;
    param_1 = FUN_0041b0d0(param_1,param_2,param_3,in_stack_ffffffdc,in_stack_ffffffe0);
    param_3 = extraout_ECX;
  }
  puStack_2c = &LAB_00416d2a;
  uStack_30 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_30;
  puStack_28 = &stack0xfffffffc;
  FUN_00414ac0(param_3,param_5,param_4,(int *)&local_8);
  FUN_0041b894((int *)(param_1 + 4),local_8);
  *in_FS_OFFSET = uStack_30;
  puStack_28 = &LAB_00416d31;
  puStack_2c = (undefined1 *)0x416d29;
  FUN_0041b840((int *)&local_8);
  return;
}



int * FUN_00416d50(int *param_1,char param_2,undefined4 *param_3)

{
  undefined4 *extraout_ECX;
  char extraout_DL;
  char cVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 in_stack_ffffffe4;
  undefined4 in_stack_ffffffe8;
  
  cVar1 = '\0';
  if (param_2 != '\0') {
    param_1 = (int *)FUN_0041b0d0((int)param_1,param_2,param_3,in_stack_ffffffe4,in_stack_ffffffe8);
    param_3 = extraout_ECX;
    cVar1 = extraout_DL;
  }
  FUN_0041d2c4(param_3,param_1 + 1);
  if (cVar1 != '\0') {
    FUN_0041b128(param_1);
    *in_FS_OFFSET = in_stack_ffffffe4;
  }
  return param_1;
}



void FUN_00416d8c(int param_1,char param_2,undefined4 *param_3,undefined4 param_4,undefined4 param_5
                 )

{
  undefined4 *extraout_ECX;
  undefined4 *in_FS_OFFSET;
  undefined4 **ppuVar1;
  undefined4 uStack_34;
  undefined1 *puStack_30;
  undefined1 *puStack_2c;
  undefined4 in_stack_ffffffd8;
  undefined4 in_stack_ffffffdc;
  byte *local_c;
  undefined4 *local_8;
  
  local_8 = (undefined4 *)0x0;
  local_c = (byte *)0x0;
  if (param_2 != '\0') {
    puStack_2c = (undefined1 *)0x416da2;
    param_1 = FUN_0041b0d0(param_1,param_2,param_3,in_stack_ffffffd8,in_stack_ffffffdc);
    param_3 = extraout_ECX;
  }
  puStack_30 = &LAB_00416df8;
  uStack_34 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_34;
  ppuVar1 = &local_8;
  puStack_2c = &stack0xfffffffc;
  FUN_0041d2c4(param_3,(int *)&local_c);
  FUN_00414ac0(local_c,param_5,param_4,(int *)ppuVar1);
  FUN_0041b894((int *)(param_1 + 4),local_8);
  *in_FS_OFFSET = uStack_34;
  puStack_2c = &LAB_00416dff;
  puStack_30 = (undefined1 *)0x416df7;
  FUN_0041b864((int *)&local_c,2);
  return;
}



int * FUN_00416e20(int *param_1,char param_2,undefined4 *param_3,int param_4)

{
  undefined4 *extraout_ECX;
  char extraout_DL;
  char cVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 in_stack_ffffffe0;
  undefined4 in_stack_ffffffe4;
  
  cVar1 = '\0';
  if (param_2 != '\0') {
    param_1 = (int *)FUN_0041b0d0((int)param_1,param_2,param_3,in_stack_ffffffe0,in_stack_ffffffe4);
    param_3 = extraout_ECX;
    cVar1 = extraout_DL;
  }
  FUN_0041b894(param_1 + 1,param_3);
  param_1[2] = param_4;
  if (cVar1 != '\0') {
    FUN_0041b128(param_1);
    *in_FS_OFFSET = in_stack_ffffffe0;
  }
  return param_1;
}



void __stdcall FUN_00416e94(void)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  int local_10;
  undefined1 local_c;
  
  iVar1 = FUN_0041aacc();
  for (iVar3 = 0; (iVar3 < 7 && (iVar1 != (&DAT_0042d5b4)[iVar3 * 2])); iVar3 = iVar3 + 1) {
  }
  if (iVar3 < 7) {
    piVar2 = FUN_00416c94((int *)&DAT_004136d0,'\x01',(undefined4 *)(&DAT_0042d5b8)[iVar3 * 2]);
  }
  else {
    local_c = 0;
    local_10 = iVar1;
    piVar2 = (int *)FUN_00416d8c((int)&DAT_004136d0,'\x01',(undefined4 *)PTR_PTR_004306e8,0,
                                 &local_10);
  }
  piVar2[3] = iVar1;
  return;
}



void FUN_00416f58(int param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 uVar1;
  int *piVar2;
  undefined1 *puStack_3c;
  undefined1 *puStack_38;
  undefined1 *puStack_34;
  byte *local_24;
  int local_20;
  undefined1 local_1c;
  undefined4 local_18;
  undefined1 local_14;
  undefined4 local_10;
  undefined1 local_c;
  int local_8;
  
  puStack_34 = &stack0xfffffffc;
  local_24 = (byte *)0x0;
  local_8 = 0;
  puStack_38 = &LAB_00416ff4;
  puStack_3c = (undefined1 *)*in_FS_OFFSET;
  *in_FS_OFFSET = &puStack_3c;
  if (param_1 == 0) {
    FUN_0041d2c4((undefined4 *)PTR_PTR_00430770,&local_8);
  }
  else {
    puStack_34 = &stack0xfffffffc;
    FUN_0041b8d8(&local_8,param_1);
  }
  local_20 = local_8;
  local_1c = 0xb;
  local_14 = 0xb;
  local_c = 0;
  piVar2 = &local_20;
  uVar1 = 2;
  local_18 = param_2;
  local_10 = param_3;
  FUN_0041d2c4((undefined4 *)PTR_PTR_0043077c,(int *)&local_24);
  FUN_00416cd0((int)&DAT_00413dd8,'\x01',local_24,uVar1,piVar2);
  *in_FS_OFFSET = uVar1;
  puStack_3c = &LAB_00416ffb;
  FUN_0041b840((int *)&local_24);
  FUN_0041b840(&local_8);
  return;
}



void FUN_00417004(undefined4 param_1,undefined4 param_2,undefined4 *param_3)

{
  *param_3 = param_2;
  FUN_0041b508(param_1);
  return;
}



void FUN_00417014(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  
  uVar1 = FUN_00416f58(param_1,param_2,param_3);
  FUN_00417004(uVar1,param_4,(undefined4 *)&stack0x00000008);
  return;
}



undefined4 FUN_0041705c(int *param_1)

{
  int iVar1;
  uint uVar2;
  
  iVar1 = *param_1;
  if (iVar1 < -0x3fffff6d) {
    if (iVar1 == -0x3fffff6e) {
LAB_004170c0:
      return CONCAT31((int3)((uint)iVar1 >> 8),6);
    }
    if (iVar1 < -0x3fffff71) {
      if (iVar1 == -0x3fffff72) {
        return 0xc0000007;
      }
      if (iVar1 == -0x3ffffffb) {
        return 0xb;
      }
      if (iVar1 == -0x3fffff74) {
        return 4;
      }
      iVar1 = iVar1 + 0x3fffff73;
      if (iVar1 == 0) {
        return 9;
      }
    }
    else {
      uVar2 = iVar1 + 0x3fffff71;
      iVar1 = iVar1 + 0x3fffff6f;
      if (uVar2 < 2) goto LAB_004170c0;
      if (iVar1 == 0) {
        return 8;
      }
    }
  }
  else if (iVar1 < -0x3fffff69) {
    if (iVar1 == -0x3fffff6a) {
      return 0xc000000c;
    }
    if (iVar1 == -0x3fffff6d) {
      return 9;
    }
    if (iVar1 == -0x3fffff6c) {
      return 3;
    }
    iVar1 = iVar1 + 0x3fffff6b;
    if (iVar1 == 0) {
      return 5;
    }
  }
  else {
    if (iVar1 == -0x3fffff03) {
      return 0xe;
    }
    iVar1 = iVar1 + 0x3ffffec6;
    if (iVar1 == 0) {
      return 0xd;
    }
  }
  return CONCAT31((int3)((uint)iVar1 >> 8),0x16);
}



void __cdecl FUN_004170f0(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  DWORD DVar3;
  undefined4 *in_FS_OFFSET;
  undefined4 uVar4;
  undefined4 *puVar5;
  undefined1 *puStack_188;
  undefined1 *puStack_184;
  undefined1 *puStack_180;
  byte *local_174;
  undefined4 local_170;
  undefined1 local_16c;
  int local_168;
  undefined1 local_164;
  undefined4 local_160;
  undefined1 local_15c;
  byte *local_158;
  undefined *local_154;
  int local_150;
  undefined4 local_14c;
  undefined1 local_148;
  int local_144;
  undefined1 local_140;
  int local_13c;
  undefined1 local_138;
  undefined4 local_134;
  undefined1 local_130;
  CHAR local_129 [261];
  _MEMORY_BASIC_INFORMATION local_24;
  int local_8;
  
  puStack_180 = &stack0xfffffffc;
  local_174 = (byte *)0x0;
  local_150 = 0;
  local_158 = (byte *)0x0;
  local_154 = (undefined *)0x0;
  local_8 = 0;
  puStack_184 = &LAB_004172ab;
  puStack_188 = (undefined1 *)*in_FS_OFFSET;
  *in_FS_OFFSET = &puStack_188;
  iVar1 = *(int *)(param_1 + -4);
  if (*(int *)(iVar1 + 0x14) == 0) {
    puStack_180 = &stack0xfffffffc;
    FUN_0041d2c4((undefined4 *)PTR_PTR_00430758,&local_8);
  }
  else {
    FUN_0041d2c4((undefined4 *)PTR_PTR_0043075c,&local_8);
  }
  uVar2 = *(undefined4 *)(iVar1 + 0x18);
  VirtualQuery(*(LPCVOID *)(iVar1 + 0xc),&local_24,0x1c);
  if (local_24.State == 0x1000) {
    DVar3 = GetModuleFileNameA(local_24.AllocationBase,local_129,0x105);
    if (DVar3 != 0) {
      local_14c = *(undefined4 *)(iVar1 + 0xc);
      local_148 = 5;
      FUN_0041ba70((int *)&local_154,local_129,0x105);
      FUN_004143dc(local_154,&local_150);
      local_144 = local_150;
      local_140 = 0xb;
      local_13c = local_8;
      local_138 = 0xb;
      local_130 = 5;
      puVar5 = &local_14c;
      uVar4 = 3;
      local_134 = uVar2;
      FUN_0041d2c4((undefined4 *)PTR_PTR_00430784,(int *)&local_158);
      FUN_00416cd0((int)&DAT_00413c10,'\x01',local_158,uVar4,puVar5);
      goto LAB_0041727a;
    }
  }
  local_170 = *(undefined4 *)(iVar1 + 0xc);
  local_16c = 5;
  local_168 = local_8;
  local_164 = 0xb;
  local_15c = 5;
  puVar5 = &local_170;
  uVar4 = 2;
  local_160 = uVar2;
  FUN_0041d2c4((undefined4 *)PTR_PTR_0043072c,(int *)&local_174);
  FUN_00416cd0((int)&DAT_00413c10,'\x01',local_174,uVar4,puVar5);
LAB_0041727a:
  *in_FS_OFFSET = uVar4;
  puStack_188 = &LAB_004172b2;
  FUN_0041b840((int *)&local_174);
  FUN_0041b864((int *)&local_158,3);
  FUN_0041b840(&local_8);
  return;
}



void FUN_004172bc(int *param_1)

{
  uint uVar1;
  int *piVar2;
  undefined4 uVar3;
  undefined4 *in_FS_OFFSET;
  undefined4 in_stack_ffffffd4;
  undefined1 *puStack_24;
  undefined1 *puStack_20;
  undefined1 *puStack_1c;
  byte *local_14;
  int local_10;
  undefined1 local_c;
  int *local_8;
  
  puStack_1c = &stack0xfffffffc;
  local_14 = (byte *)0x0;
  puStack_20 = &LAB_0041738f;
  puStack_24 = (undefined1 *)*in_FS_OFFSET;
  *in_FS_OFFSET = &puStack_24;
  local_8 = param_1;
  uVar1 = FUN_0041705c(param_1);
  uVar1 = uVar1 & 0xff;
  if (7 < uVar1 - 3) {
    if (uVar1 == 0xb) {
      in_stack_ffffffd4 = 0x417315;
      piVar2 = (int *)FUN_004170f0((int)&stack0xfffffffc);
      goto LAB_0041734a;
    }
    if (9 < uVar1 - 0xc) {
      local_10 = *local_8;
      local_c = 0;
      piVar2 = &local_10;
      in_stack_ffffffd4 = 0;
      FUN_0041d2c4((undefined4 *)PTR_PTR_0043076c,(int *)&local_14);
      piVar2 = (int *)FUN_00416cd0((int)&DAT_00413780,'\x01',local_14,in_stack_ffffffd4,piVar2);
      goto LAB_0041734a;
    }
  }
  piVar2 = FUN_00416c94(*(int **)(&DAT_0042d5d4 + uVar1 * 8),'\x01',
                        *(undefined4 **)(&DAT_0042d5d8 + uVar1 * 8));
LAB_0041734a:
  uVar3 = FUN_0041afcc(piVar2,(int)&DAT_00413728);
  if (((char)uVar3 != '\0') && (piVar2[3] = (int)local_8, *local_8 == 0xeefface)) {
    FUN_0041b894(piVar2 + 1,(undefined4 *)"C++ Exception");
  }
  *in_FS_OFFSET = in_stack_ffffffd4;
  puStack_24 = &LAB_00417396;
  FUN_0041b840((int *)&local_14);
  return;
}



void __stdcall FUN_004173c8(void)

{
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_18;
  undefined1 *puStack_14;
  undefined1 *puStack_10;
  undefined4 *local_c;
  undefined4 *local_8;
  
  puStack_10 = &stack0xfffffffc;
  local_8 = (undefined4 *)0x0;
  local_c = (undefined4 *)0x0;
  puStack_14 = &LAB_00417489;
  uStack_18 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_18;
  FUN_0041d2c4((undefined4 *)PTR_PTR_004306e4,(int *)&local_8);
  DAT_004309b4 = FUN_00416c94((int *)&DAT_00413674,'\x01',local_8);
  FUN_0041d2c4((undefined4 *)PTR_PTR_00430724,(int *)&local_c);
  DAT_004309b8 = FUN_00416c94((int *)&DAT_00413afc,'\x01',local_c);
  *(undefined1 **)PTR_DAT_0043063c = &LAB_00416f14;
  *(undefined1 **)PTR_DAT_00430638 = &LAB_004173b8;
  *(undefined **)PTR_DAT_00430648 = &DAT_004135b0;
  *(undefined1 **)PTR_DAT_00430640 = &LAB_004170dc;
  *(code **)PTR_DAT_00430644 = FUN_004172bc;
  *(code **)PTR_DAT_0043064c = FUN_00417014;
  *(undefined1 **)PTR_DAT_00430650 = &LAB_00417030;
  *in_FS_OFFSET = uStack_18;
  puStack_10 = &LAB_00417490;
  puStack_14 = (undefined1 *)0x417488;
  FUN_0041b864((int *)&local_c,2);
  return;
}



void __stdcall FUN_00417494(void)

{
  *(undefined1 *)(DAT_004309b4 + 3) = 1;
  (**(code **)(*DAT_004309b4 + -8))();
  DAT_004309b4 = (int *)0x0;
  *(undefined1 *)(DAT_004309b8 + 3) = 1;
  FUN_0041ae54(DAT_004309b8);
  DAT_004309b8 = (int *)0x0;
  *(undefined4 *)PTR_DAT_0043063c = 0;
  *(undefined4 *)PTR_DAT_00430638 = 0;
  *(undefined4 *)PTR_DAT_00430648 = 0;
  *(undefined4 *)PTR_DAT_00430640 = 0;
  *(undefined4 *)PTR_DAT_00430644 = 0;
  *(undefined4 *)PTR_DAT_0043064c = 0;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __stdcall FUN_00417500(void)

{
  BOOL BVar1;
  _OSVERSIONINFOA local_94;
  
  local_94.dwOSVersionInfoSize = 0x94;
  BVar1 = GetVersionExA(&local_94);
  if (BVar1 != 0) {
    _DAT_0042d47c = local_94.dwPlatformId;
    _DAT_0042d480 = local_94.dwMajorVersion;
    _DAT_0042d484 = local_94.dwMinorVersion;
    _DAT_0042d488 = local_94.dwBuildNumber;
    FUN_0041ba70((int *)&DAT_0042d48c,local_94.szCSDVersion,0x80);
  }
  return;
}



undefined4 FUN_00417558(byte *param_1,int param_2)

{
  uint uVar1;
  undefined4 uVar2;
  int iVar3;
  
  uVar2 = 0;
  if ((param_1 != (byte *)0x0) && (param_1[param_2] != 0)) {
    iVar3 = param_2;
    if (param_2 == 0) {
      if (((byte)(&DAT_0042d4c4)[(int)(uint)*param_1 >> 3] >> (*param_1 & 7) & 1) != 0) {
        uVar2 = 1;
      }
    }
    else {
      do {
        iVar3 = iVar3 + -1;
        if (iVar3 < 0) break;
      } while (((byte)(&DAT_0042d4c4)[(int)(uint)param_1[iVar3] >> 3] >> (param_1[iVar3] & 7) & 1)
               != 0);
      uVar1 = param_2 - iVar3 & 0x80000001;
      if ((int)uVar1 < 0) {
        uVar1 = (uVar1 - 1 | 0xfffffffe) + 1;
      }
      if (uVar1 == 0) {
        uVar2 = 2;
      }
      else if (((byte)(&DAT_0042d4c4)[(int)(uint)param_1[param_2] >> 3] >> (param_1[param_2] & 7) &
               1) != 0) {
        uVar2 = 1;
      }
    }
  }
  return uVar2;
}



undefined4 FUN_004175d0(undefined *param_1,int param_2)

{
  undefined4 uVar1;
  byte *pbVar2;
  
  uVar1 = 0;
  if (DAT_00430970 != '\0') {
    pbVar2 = FUN_0041bc74(param_1);
    uVar1 = FUN_00417558(pbVar2,param_2 + -1);
  }
  return uVar1;
}



undefined4 FUN_004175f4(byte *param_1,int param_2)

{
  undefined4 uVar1;
  
  uVar1 = 0;
  if (DAT_00430970 != '\0') {
    uVar1 = FUN_00417558(param_1,param_2);
  }
  return uVar1;
}



void FUN_0041760c(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = FUN_0041bac0(param_1);
  if (iVar1 < param_2) {
    param_2 = FUN_0041bac0(param_1);
  }
  FUN_00417634(param_1,param_2);
  return;
}



int FUN_00417634(int param_1,int param_2)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  
  iVar3 = 0;
  if (0 < param_2) {
    iVar1 = FUN_0041bac0(param_1);
    if ((param_2 <= iVar1) && (iVar3 = param_2, DAT_00430970 != '\0')) {
      iVar1 = 1;
      iVar3 = 0;
      if (0 < param_2) {
        do {
          uVar2 = (uint)*(byte *)(param_1 + -1 + iVar1);
          if (((byte)(&DAT_0042d4c4)[(int)uVar2 >> 3] >> (uVar2 & 7) & 1) != 0) {
            iVar1 = iVar1 + 1;
          }
          iVar1 = iVar1 + 1;
          iVar3 = iVar3 + 1;
        } while (iVar1 <= param_2);
      }
    }
  }
  return iVar3;
}



void FUN_00417684(int param_1,int param_2,int *param_3,int *param_4)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  
  iVar1 = FUN_0041bac0(param_1);
  iVar4 = 1;
  iVar2 = 1;
  while ((iVar2 < iVar1 && (iVar4 < param_2))) {
    iVar4 = iVar4 + 1;
    uVar3 = (uint)*(byte *)(param_1 + -1 + iVar2);
    if (((byte)(&DAT_0042d4c4)[(int)uVar3 >> 3] >> (uVar3 & 7) & 1) != 0) {
      iVar2 = iVar2 + 1;
    }
    iVar2 = iVar2 + 1;
  }
  if (((iVar4 == param_2) && (iVar2 < iVar1)) &&
     (uVar3 = (uint)*(byte *)(param_1 + -1 + iVar2),
     ((byte)(&DAT_0042d4c4)[(int)uVar3 >> 3] >> (uVar3 & 7) & 1) != 0)) {
    iVar2 = iVar2 + 1;
  }
  *param_3 = iVar4;
  *param_4 = iVar2;
  return;
}



int FUN_004176f8(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int local_10;
  int local_c;
  
  local_10 = 0;
  iVar2 = local_10;
  if ((((0 < param_2) && (iVar1 = FUN_0041bac0(param_1), iVar2 = local_10, param_2 <= iVar1)) &&
      (iVar2 = param_2, 1 < param_2)) && (iVar2 = param_2, DAT_00430970 != '\0')) {
    FUN_00417684(param_1,param_2 + -1,&local_c,&local_10);
    if ((local_c < param_2 + -1) || (iVar2 = FUN_0041bac0(param_1), iVar2 <= local_10)) {
      local_10 = 0;
      iVar2 = local_10;
    }
    else {
      iVar2 = local_10 + 1;
    }
  }
  local_10 = iVar2;
  return local_10;
}



int FUN_0041775c(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int local_10;
  int local_c;
  
  local_10 = 0;
  iVar1 = local_10;
  if (0 < param_2) {
    iVar1 = FUN_0041bac0(param_1);
    if (iVar1 < param_2) {
      param_2 = FUN_0041bac0(param_1);
    }
    iVar1 = param_2;
    if (DAT_00430970 != '\0') {
      FUN_00417684(param_1,param_2,&local_c,&local_10);
      iVar2 = FUN_0041bac0(param_1);
      iVar1 = local_10;
      if (iVar2 < local_10) {
        iVar1 = FUN_0041bac0(param_1);
      }
    }
  }
  local_10 = iVar1;
  return local_10;
}



PCNZCH FUN_004177bc(undefined *param_1,undefined *param_2)

{
  char *pcVar1;
  byte *pbVar2;
  PCNZCH pCVar3;
  PCNZCH pCVar4;
  
  pCVar4 = (PCNZCH)0x0;
  pcVar1 = FUN_0041bc74(param_1);
  pbVar2 = FUN_0041bc74(param_2);
  pCVar3 = FUN_004177f4(pbVar2,pcVar1);
  if (pCVar3 != (PCNZCH)0x0) {
    pCVar4 = pCVar3 + (1 - (int)pbVar2);
  }
  return pCVar4;
}



PCNZCH FUN_004177f4(byte *param_1,char *param_2)

{
  int iVar1;
  uint cchCount1;
  char *lpString1;
  undefined4 uVar2;
  int iVar3;
  
  if ((((param_1 != (byte *)0x0) && (*param_1 != 0)) && (param_2 != (char *)0x0)) &&
     (*param_2 != '\0')) {
    iVar1 = FUN_00414488((char *)param_1);
    cchCount1 = FUN_00414488(param_2);
    for (lpString1 = FUN_00414578((char *)param_1,param_2);
        (lpString1 != (PCNZCH)0x0 && (cchCount1 <= (uint)(iVar1 - ((int)lpString1 - (int)param_1))))
        ; lpString1 = FUN_00414578(lpString1 + 1,param_2)) {
      uVar2 = FUN_004175f4(param_1,(int)lpString1 - (int)param_1);
      if (((char)uVar2 != '\x02') &&
         (iVar3 = CompareStringA(0x400,0,lpString1,cchCount1,param_2,cchCount1), iVar3 == 2)) {
        return lpString1;
      }
      if ((char)uVar2 == '\x01') {
        lpString1 = lpString1 + 1;
      }
    }
  }
  return (PCNZCH)0x0;
}



char * FUN_00417894(byte *param_1,char param_2)

{
  char *pcVar1;
  char *pcVar2;
  char *pcVar3;
  
  pcVar2 = FUN_004178bc(param_1,param_2);
  pcVar3 = pcVar2;
  if (param_2 != '\0') {
    while (pcVar1 = pcVar3, pcVar1 != (char *)0x0) {
      pcVar3 = FUN_004178bc((byte *)(pcVar1 + 1),param_2);
      pcVar2 = pcVar1;
    }
  }
  return pcVar2;
}



char * FUN_004178bc(byte *param_1,char param_2)

{
  char *pcVar1;
  undefined4 uVar2;
  
  pcVar1 = FUN_00414558((char *)param_1,param_2);
  while( true ) {
    if (pcVar1 == (char *)0x0) {
      return (char *)0x0;
    }
    uVar2 = FUN_004175f4(param_1,(int)pcVar1 - (int)param_1);
    if ((char)uVar2 == '\0') break;
    if ((char)uVar2 == '\x01') {
      pcVar1 = pcVar1 + 1;
    }
    pcVar1 = FUN_00414558(pcVar1 + 1,param_2);
  }
  return pcVar1;
}



void __stdcall FUN_004178fc(void)

{
  LCID LVar1;
  int iVar2;
  char cVar3;
  byte local_28;
  _cpinfo local_24;
  
  DAT_00430968 = 0x409;
  DAT_0043096c = 9;
  DAT_0043096e = 1;
  LVar1 = GetThreadLocale();
  if (LVar1 != 0) {
    DAT_00430968 = LVar1;
  }
  if ((ushort)LVar1 != 0) {
    DAT_0043096c = (ushort)LVar1 & 0x3ff;
    DAT_0043096e = (undefined2)((LVar1 & 0xffff) >> 10);
  }
  iVar2 = GetSystemMetrics(0x4a);
  DAT_00430971 = iVar2 != 0;
  iVar2 = GetSystemMetrics(0x2a);
  DAT_00430970 = iVar2 != 0;
  if ((bool)DAT_00430970) {
    GetCPInfo(0,&local_24);
    for (iVar2 = 0;
        (iVar2 < 0xc && (local_24.LeadByte[iVar2] != '\0' || local_24.LeadByte[iVar2 + 1] != '\0'));
        iVar2 = iVar2 + 2) {
      local_28 = local_24.LeadByte[iVar2];
      if (local_28 <= local_24.LeadByte[iVar2 + 1]) {
        cVar3 = (local_24.LeadByte[iVar2 + 1] - local_28) + '\x01';
        do {
          (&DAT_0042d4c4)[(int)(uint)local_28 >> 3] =
               (&DAT_0042d4c4)[(int)(uint)local_28 >> 3] | '\x01' << (local_28 & 7);
          local_28 = local_28 + 1;
          cVar3 = cVar3 + -1;
        } while (cVar3 != '\0');
      }
    }
  }
  return;
}



void __stdcall FUN_004179b4(void)

{
  LCID LVar1;
  undefined4 uVar2;
  uint uVar3;
  int iVar4;
  int extraout_ECX;
  int extraout_ECX_00;
  undefined4 extraout_ECX_01;
  undefined4 extraout_ECX_02;
  int extraout_ECX_03;
  undefined4 extraout_ECX_04;
  undefined4 extraout_ECX_05;
  int extraout_ECX_06;
  int extraout_ECX_07;
  int extraout_ECX_08;
  undefined4 extraout_ECX_09;
  byte *unaff_EBX;
  undefined4 *in_FS_OFFSET;
  byte *local_44;
  byte *local_40;
  byte *local_3c;
  char *local_38;
  undefined4 *local_34;
  undefined4 *local_30;
  undefined4 *local_2c;
  undefined4 *local_28;
  undefined4 *local_24;
  byte *pbVar5;
  byte *local_1c;
  byte *local_18;
  undefined4 *local_14;
  undefined4 *local_c;
  undefined4 *local_8;
  
  local_14 = (undefined4 *)&stack0xfffffffc;
  iVar4 = 8;
  do {
    local_8 = (undefined4 *)0x0;
    iVar4 = iVar4 + -1;
  } while (iVar4 != 0);
  local_18 = &LAB_00417c7f;
  local_1c = (byte *)*in_FS_OFFSET;
  *in_FS_OFFSET = &local_1c;
  FUN_004178fc();
  FUN_004165c8();
  if (DAT_00430970 != '\0') {
    FUN_004167a0();
  }
  LVar1 = GetThreadLocale();
  local_24 = (undefined4 *)0x417a03;
  FUN_00416518(LVar1,0x14,(undefined4 *)0x0,(int *)&local_14);
  FUN_0041b894((int *)&DAT_004308a8,local_14);
  local_24 = (undefined4 *)0x417a25;
  FUN_00416518(LVar1,0x1b,(undefined4 *)&DAT_00417c94,(int *)&local_18);
  uVar2 = FUN_00414274(local_18,0,extraout_ECX);
  DAT_004308ac = (undefined1)uVar2;
  local_24 = (undefined4 *)0x417a49;
  FUN_00416518(LVar1,0x1c,(undefined4 *)&DAT_00417c94,(int *)&local_1c);
  uVar2 = FUN_00414274(local_1c,0,extraout_ECX_00);
  DAT_004308ad = (undefined1)uVar2;
  uVar3 = FUN_00416564(LVar1,0xf,CONCAT31((int3)((uint)extraout_ECX_01 >> 8),0x2c));
  DAT_004308ae = (undefined1)uVar3;
  uVar3 = FUN_00416564(LVar1,0xe,CONCAT31((int3)((uint)extraout_ECX_02 >> 8),0x2e));
  DAT_004308af = (undefined1)uVar3;
  pbVar5 = &stack0xffffffe0;
  local_24 = (undefined4 *)0x417a93;
  FUN_00416518(LVar1,0x19,(undefined4 *)&DAT_00417c94,(int *)pbVar5);
  uVar2 = FUN_00414274(pbVar5,0,extraout_ECX_03);
  DAT_004308b0 = (undefined1)uVar2;
  uVar3 = FUN_00416564(LVar1,0x1d,CONCAT31((int3)((uint)extraout_ECX_04 >> 8),0x2f));
  DAT_004308b1 = (undefined1)uVar3;
  local_24 = (undefined4 *)0x417aca;
  FUN_00416518(LVar1,0x1f,(undefined4 *)"m/d/yy",(int *)&local_28);
  FUN_00416850(local_28,(int *)&local_24);
  FUN_0041b894(&DAT_004308b4,local_24);
  local_24 = (undefined4 *)0x417af7;
  FUN_00416518(LVar1,0x20,(undefined4 *)"mmmm d, yyyy",(int *)&local_30);
  FUN_00416850(local_30,(int *)&local_2c);
  FUN_0041b894(&DAT_004308b8,local_2c);
  uVar3 = FUN_00416564(LVar1,0x1e,CONCAT31((int3)((uint)extraout_ECX_05 >> 8),0x3a));
  DAT_004308bc = (undefined1)uVar3;
  local_24 = (undefined4 *)0x417b37;
  FUN_00416518(LVar1,0x28,(undefined4 *)&DAT_00417cc8,(int *)&local_34);
  FUN_0041b894(&DAT_004308c0,local_34);
  local_24 = (undefined4 *)0x417b59;
  FUN_00416518(LVar1,0x29,(undefined4 *)&DAT_00417cd4,(int *)&local_38);
  FUN_0041b894(&DAT_004308c4,(undefined4 *)local_38);
  FUN_0041b840((int *)&local_c);
  FUN_0041b840((int *)&stack0xfffffff0);
  local_24 = (undefined4 *)0x417b8b;
  FUN_00416518(LVar1,0x25,(undefined4 *)&DAT_00417c94,(int *)&local_3c);
  iVar4 = FUN_00414274(local_3c,0,extraout_ECX_06);
  if (iVar4 == 0) {
    FUN_0041b8d8((int *)&local_8,0x417ce0);
  }
  else {
    FUN_0041b8d8((int *)&local_8,0x417cec);
  }
  local_24 = (undefined4 *)0x417bca;
  FUN_00416518(LVar1,0x23,(undefined4 *)&DAT_00417c94,(int *)&local_40);
  iVar4 = FUN_00414274(local_40,0,extraout_ECX_07);
  if (iVar4 == 0) {
    local_24 = (undefined4 *)0x417bed;
    FUN_00416518(LVar1,0x1005,(undefined4 *)&DAT_00417c94,(int *)&local_44);
    iVar4 = FUN_00414274(local_44,0,extraout_ECX_08);
    if (iVar4 == 0) {
      FUN_0041b8d8((int *)&stack0xfffffff0,0x417cf8);
    }
    else {
      FUN_0041b8d8((int *)&local_c,0x417d08);
    }
  }
  local_24 = local_8;
  local_28 = (undefined4 *)&DAT_00417d18;
  local_30 = (undefined4 *)0x417c34;
  FUN_0041bb80(&DAT_004308c8,4);
  local_30 = local_c;
  local_34 = local_8;
  local_38 = ":mm:ss";
  local_40 = (byte *)0x417c51;
  local_3c = unaff_EBX;
  FUN_0041bb80(&DAT_004308cc,4);
  local_40 = (byte *)0x417c5f;
  uVar3 = FUN_00416564(LVar1,0xc,CONCAT31((int3)((uint)extraout_ECX_09 >> 8),0x2c));
  DAT_004309ac = (undefined1)uVar3;
  *in_FS_OFFSET = local_3c;
  local_34 = (undefined4 *)&LAB_00417c86;
  local_38 = (char *)0x417c7e;
  FUN_0041b864((int *)&local_44,0x10);
  return;
}



void __stdcall FUN_00417d2c(void)

{
  DWORD DVar1;
  int *piVar2;
  undefined4 *in_FS_OFFSET;
  undefined4 in_stack_ffffffd0;
  undefined1 *puStack_28;
  undefined1 *puStack_24;
  undefined1 *puStack_20;
  int local_18;
  DWORD local_14;
  undefined1 local_10;
  int local_c;
  undefined1 local_8;
  
  puStack_20 = &stack0xfffffffc;
  local_18 = 0;
  puStack_24 = &LAB_00417db6;
  puStack_28 = (undefined1 *)*in_FS_OFFSET;
  *in_FS_OFFSET = &puStack_28;
  DVar1 = GetLastError();
  if (DVar1 == 0) {
    piVar2 = FUN_00416d50((int *)&DAT_00413ef0,'\x01',(undefined4 *)PTR_LAB_0043078c);
  }
  else {
    local_10 = 0;
    local_14 = DVar1;
    FUN_004164cc(DVar1,&local_18);
    local_c = local_18;
    local_8 = 0xb;
    in_stack_ffffffd0 = 1;
    piVar2 = (int *)FUN_00416d8c((int)&DAT_00413ef0,'\x01',(undefined4 *)PTR_PTR_00430788,1,
                                 &local_14);
  }
  piVar2[3] = DVar1;
  FUN_0041b508(piVar2);
  *in_FS_OFFSET = in_stack_ffffffd0;
  puStack_28 = &LAB_00417dbd;
  FUN_0041b840(&local_18);
  return;
}



int FUN_00417dc4(int param_1)

{
  if (param_1 == 0) {
    FUN_00417d2c();
  }
  return param_1;
}



void __stdcall FUN_00417dd4(void)

{
  undefined4 *puVar1;
  
  while (puVar1 = DAT_0042d69c, DAT_0042d69c != (undefined4 *)0x0) {
    DAT_0042d69c = (undefined4 *)*DAT_0042d69c;
    FUN_0041a9d8((int)puVar1);
  }
  return;
}



void __stdcall FUN_00417df4(void)

{
  HMODULE hModule;
  
  hModule = GetModuleHandleA("kernel32.dll");
  if (hModule != (HMODULE)0x0) {
    DAT_0042d474 = GetProcAddress(hModule,"GetDiskFreeSpaceExA");
  }
  if (DAT_0042d474 == (FARPROC)0x0) {
    DAT_0042d474 = (FARPROC)&LAB_00414414;
  }
  return;
}



bool FUN_00417ef8(int param_1)

{
  int iVar1;
  bool bVar2;
  int iVar3;
  int iVar4;
  
  bVar2 = false;
  if (*(int *)(param_1 + 0x2c) != 0) {
    iVar3 = FUN_0041cef8(*(int *)(param_1 + 0x28));
    for (iVar4 = 0;
        (iVar4 < iVar3 &&
        ((iVar1 = *(int *)(*(int *)(param_1 + 0x28) + iVar4 * 8), iVar1 == 0 ||
         (iVar1 == *(int *)(param_1 + 0x2c))))); iVar4 = iVar4 + 1) {
    }
    bVar2 = iVar3 <= iVar4;
  }
  return bVar2;
}



void FUN_00417f34(int param_1)

{
  bool bVar1;
  DWORD DVar2;
  
  EnterCriticalSection((LPCRITICAL_SECTION)(param_1 + 4));
  if (*(char *)(param_1 + 0x34) == '\0') {
    DVar2 = GetCurrentThreadId();
    *(DWORD *)(param_1 + 0x2c) = DVar2;
    bVar1 = FUN_00417ef8(param_1);
    if (!bVar1) {
      WaitForSingleObject(*(HANDLE *)(param_1 + 0x1c),0xffffffff);
    }
    *(undefined4 *)(param_1 + 0x24) = *(undefined4 *)(param_1 + 0x20);
    *(undefined4 *)(param_1 + 0x20) = 0;
    *(undefined4 *)(param_1 + 0x2c) = 0;
    *(undefined1 *)(param_1 + 0x34) = 1;
  }
  *(int *)(param_1 + 0x20) = *(int *)(param_1 + 0x20) + 1;
  return;
}



void FUN_00417f80(undefined4 *param_1)

{
  int *piVar1;
  
  piVar1 = (int *)*param_1;
  *param_1 = 0;
  FUN_0041ae54(piVar1);
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __stdcall FUN_004183a0(void)

{
  undefined4 *in_FS_OFFSET;
  bool bVar1;
  undefined4 uStack_10;
  undefined1 *puStack_c;
  undefined1 *puStack_8;
  
  puStack_8 = &stack0xfffffffc;
  puStack_c = &LAB_00418408;
  uStack_10 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_10;
  bVar1 = _DAT_004309b0 == 0;
  _DAT_004309b0 = _DAT_004309b0 + -1;
  if (bVar1) {
    FUN_0041b648((int *)&DAT_00417f90);
    FUN_0041b664((int *)&LAB_0041807c);
    if (DAT_004309c6 != '\0') {
      FUN_0041b894((int *)&PTR_DAT_0042d4e4,(undefined4 *)&LAB_0041841c);
    }
    FUN_004173c8();
    FUN_004179b4();
    FUN_00417500();
    FUN_00417df4();
  }
  *in_FS_OFFSET = uStack_10;
  return;
}



void FUN_00418726(undefined4 *param_1,char *param_2)

{
  char cVar1;
  char *pcVar2;
  char *pcVar3;
  char cVar4;
  
  param_1[5] = param_1 + 0x53;
  *param_1 = 0;
  param_1[1] = 0xd7b0;
  param_1[2] = 0x80;
  param_1[3] = 0;
  param_1[4] = 0;
  param_1[6] = &LAB_00418598;
  param_1[7] = 0;
  param_1[8] = 0;
  param_1[9] = 0;
  pcVar3 = (char *)(param_1 + 0x12);
  if (param_2 != (char *)0x0) {
    cVar4 = -0x7e;
    do {
      pcVar2 = pcVar3;
      cVar1 = *param_2;
      *pcVar2 = cVar1;
      if (cVar1 == '\0') {
        return;
      }
      cVar1 = param_2[1];
      param_2 = param_2 + 2;
      pcVar2[1] = cVar1;
      if (cVar1 == '\0') {
        return;
      }
      cVar4 = cVar4 + -1;
      pcVar3 = pcVar2 + 2;
    } while (cVar4 != '\0');
    pcVar3 = pcVar2 + 1;
  }
  *pcVar3 = '\0';
  return;
}



void FUN_004187a1(int param_1)

{
  int iVar1;
  
  if (*(int *)(param_1 + 4) == 0xd7b2) {
    iVar1 = (**(code **)(param_1 + 0x20))();
    if (iVar1 != 0) {
LAB_004187c1:
      FUN_0041aab0(iVar1);
      return;
    }
  }
  else if (*(int *)(param_1 + 4) != 0xd7b1) {
    iVar1 = 0x67;
    goto LAB_004187c1;
  }
  return;
}



void FUN_0041893f(undefined *param_1,char *param_2,int param_3)

{
  bool bVar1;
  char cVar2;
  int iVar3;
  char *pcVar5;
  int iVar6;
  byte bVar7;
  int iVar4;
  
  bVar7 = 0;
  if ((*(int *)(param_1 + 4) == 0xd7b1) ||
     ((param_1 == &DAT_00430a34 && (FUN_004189fb(&DAT_00430a34), DAT_00430a38 == 0xd7b1)))) {
    if (0 < param_3) {
      pcVar5 = (char *)(*(int *)(param_1 + 0xc) + *(int *)(param_1 + 0x14));
      iVar4 = *(int *)(param_1 + 0x10) - *(int *)(param_1 + 0xc);
      do {
        iVar3 = iVar4 + -1;
        if (iVar4 < 1) {
          *(char **)(param_1 + 0xc) = pcVar5;
          cVar2 = FUN_0041d41c(param_1);
          iVar3 = *(int *)(param_1 + 0x10) - *(int *)(param_1 + 0xc);
          pcVar5 = (char *)(*(int *)(param_1 + 0xc) + *(int *)(param_1 + 0x14));
        }
        else {
          cVar2 = *pcVar5;
          pcVar5 = pcVar5 + 1;
        }
        if (cVar2 == '\x1a') {
          return;
        }
        if (cVar2 == '\r') {
          pcVar5 = pcVar5 + -1;
          break;
        }
        *param_2 = cVar2;
        iVar6 = param_3 + -1;
        bVar1 = 0 < param_3;
        iVar4 = iVar3;
        param_3 = iVar6;
        param_2 = param_2 + (uint)bVar7 * -2 + 1;
      } while (iVar6 != 0 && bVar1);
      *(int *)(param_1 + 0xc) = (int)pcVar5 - *(int *)(param_1 + 0x14);
    }
  }
  else {
    FUN_0041aab0(0x68);
  }
  return;
}



void FUN_004189c0(undefined *param_1,undefined4 param_2)

{
  int iVar1;
  
  if (*(int *)(param_1 + 4) != 0xd7b0) {
    if (3 < *(int *)(param_1 + 4) - 0xd7b0U) {
      FUN_0041aab0(0x66);
      return;
    }
    FUN_0041d3e0(param_1);
  }
  *(undefined4 *)(param_1 + 4) = param_2;
  iVar1 = (**(code **)(param_1 + 0x18))(param_1);
  if (iVar1 != 0) {
    FUN_0041aab0(iVar1);
  }
  return;
}



void FUN_004189fb(undefined *param_1)

{
  FUN_004189c0(param_1,0xd7b1);
  return;
}



void FUN_00418a02(undefined *param_1)

{
  FUN_004189c0(param_1,0xd7b2);
  return;
}



undefined * FUN_00418b48(undefined *param_1,undefined1 *param_2,int param_3)

{
  int iVar1;
  int iVar2;
  undefined1 *puVar3;
  byte bVar4;
  
  bVar4 = 0;
  if ((*(int *)(param_1 + 4) == 0xd7b2) ||
     ((param_1 == &DAT_00430c00 && (FUN_00418a02(&DAT_00430c00), iRam00430c04 == 0xd7b2)))) {
    do {
      iVar1 = *(int *)(param_1 + 0xc);
      iVar2 = *(int *)(param_1 + 8) - *(int *)(param_1 + 0xc);
      if (param_3 < iVar2) {
        *(int *)(param_1 + 0xc) = *(int *)(param_1 + 0xc) + param_3;
        puVar3 = (undefined1 *)(*(int *)(param_1 + 0x14) + iVar1);
        for (; param_3 != 0; param_3 = param_3 + -1) {
          *puVar3 = *param_2;
          param_2 = param_2 + (uint)bVar4 * -2 + 1;
          puVar3 = puVar3 + (uint)bVar4 * -2 + 1;
        }
        return param_1;
      }
      *(int *)(param_1 + 0xc) = *(int *)(param_1 + 0xc) + iVar2;
      param_3 = param_3 - iVar2;
      puVar3 = (undefined1 *)(*(int *)(param_1 + 0x14) + iVar1);
      for (; iVar2 != 0; iVar2 = iVar2 + -1) {
        *puVar3 = *param_2;
        param_2 = param_2 + (uint)bVar4 * -2 + 1;
        puVar3 = puVar3 + (uint)bVar4 * -2 + 1;
      }
      iVar1 = (**(code **)(param_1 + 0x1c))();
    } while (iVar1 == 0);
    FUN_0041aab0(iVar1);
  }
  else {
    param_1 = (undefined *)FUN_0041aab0(0x69);
  }
  return param_1;
}



undefined * FUN_00418bb3(undefined *param_1,int param_2)

{
  int iVar1;
  int iVar2;
  undefined *puVar3;
  undefined1 *puVar4;
  byte bVar5;
  
  bVar5 = 0;
  while (0x40 < param_2) {
    param_2 = param_2 + -0x40;
    FUN_00418b48(param_1,&DAT_0042d6a0,0x40);
    iVar1 = FUN_00401202();
    if (*(int *)(iVar1 + 0xc) != 0) {
      return param_1;
    }
  }
  if (0 < param_2) {
    puVar3 = &DAT_0042d6a0;
    if ((*(int *)(param_1 + 4) == 0xd7b2) ||
       ((param_1 == &DAT_00430c00 && (FUN_00418a02(&DAT_00430c00), iRam00430c04 == 0xd7b2)))) {
      do {
        iVar1 = *(int *)(param_1 + 0xc);
        iVar2 = *(int *)(param_1 + 8) - *(int *)(param_1 + 0xc);
        if (param_2 < iVar2) {
          *(int *)(param_1 + 0xc) = *(int *)(param_1 + 0xc) + param_2;
          puVar4 = (undefined1 *)(*(int *)(param_1 + 0x14) + iVar1);
          for (; param_2 != 0; param_2 = param_2 + -1) {
            *puVar4 = *puVar3;
            puVar3 = puVar3 + (uint)bVar5 * -2 + 1;
            puVar4 = puVar4 + (uint)bVar5 * -2 + 1;
          }
          return param_1;
        }
        *(int *)(param_1 + 0xc) = *(int *)(param_1 + 0xc) + iVar2;
        param_2 = param_2 - iVar2;
        puVar4 = (undefined1 *)(*(int *)(param_1 + 0x14) + iVar1);
        for (; iVar2 != 0; iVar2 = iVar2 + -1) {
          *puVar4 = *puVar3;
          puVar3 = puVar3 + (uint)bVar5 * -2 + 1;
          puVar4 = puVar4 + (uint)bVar5 * -2 + 1;
        }
        iVar1 = (**(code **)(param_1 + 0x1c))();
      } while (iVar1 == 0);
      FUN_0041aab0(iVar1);
    }
    else {
      param_1 = (undefined *)FUN_0041aab0(0x69);
    }
    return param_1;
  }
  return param_1;
}



void FUN_00418c13(undefined *param_1)

{
  undefined *puVar1;
  
  puVar1 = FUN_00418b48(param_1,&DAT_0042d6e0,2);
  FUN_004187a1((int)puVar1);
  return;
}



void FUN_00418c90(undefined *param_1,char *param_2)

{
  char cVar1;
  uint uVar2;
  char *pcVar3;
  
  uVar2 = 0xffffffff;
  pcVar3 = param_2;
  do {
    if (uVar2 == 0) break;
    uVar2 = uVar2 - 1;
    cVar1 = *pcVar3;
    pcVar3 = pcVar3 + 1;
  } while (cVar1 != '\0');
  FUN_00418bb3(param_1,-(~uVar2 - 1));
  FUN_00418b48(param_1,param_2,~uVar2 - 1);
  return;
}



void FUN_00418eb4(int param_1,undefined1 *param_2)

{
  int iVar1;
  short *psVar2;
  short *psVar3;
  char *pcVar4;
  longdouble in_ST0;
  longdouble lVar5;
  unkbyte10 local_e;
  
  *param_2 = 0x30;
  lVar5 = ROUND(in_ST0 * (longdouble)1e+17);
  if ((longdouble)1e+18 <= lVar5) {
    lVar5 = lVar5 - (longdouble)1e+18;
    *param_2 = 0x31;
  }
  local_e = to_bcd(lVar5);
  iVar1 = 8;
  psVar2 = (short *)(param_2 + 1);
  do {
    psVar3 = psVar2 + 1;
    *psVar2 = (CONCAT11(*(byte *)((int)&local_e + iVar1),*(byte *)((int)&local_e + iVar1) >> 4) &
              0xfff) + 0x3030;
    iVar1 = iVar1 + -1;
    psVar2 = psVar3;
  } while (-1 < iVar1);
  iVar1 = param_1 + -0x12;
  if (param_1 < 0x12) {
    pcVar4 = (char *)((int)psVar3 + iVar1);
    if ('4' < *pcVar4) {
      while( true ) {
        pcVar4 = pcVar4 + -1;
        *pcVar4 = *pcVar4 + '\x01';
        if (*pcVar4 < ':') break;
        *pcVar4 = '0';
      }
    }
  }
  else {
    for (; iVar1 != 0; iVar1 = iVar1 + -1) {
      *(undefined1 *)psVar3 = 0x30;
      psVar3 = (short *)((int)psVar3 + 1);
    }
  }
  return;
}



int __stdcall FUN_00418f1e(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  unkbyte10 in_ST0;
  int local_10;
  int iStack_c;
  short sStack_8;
  
  iVar3 = 0;
  while (((sStack_8 = (short)((unkuint10)in_ST0 >> 0x40), sStack_8 != 0 ||
          (iStack_c = (int)((unkuint10)in_ST0 >> 0x20), iStack_c != 0)) ||
         (local_10 = (int)in_ST0, local_10 != 0))) {
    iVar1 = (int)(short)((uint)((short)(sStack_8 + -0x3fff) * 0x4d10) >> 0x10);
    iVar2 = -iVar1;
    if (iVar2 == 0) {
      return iVar3;
    }
    iVar3 = iVar3 + iVar1;
    FUN_00418f64(iVar2);
  }
  return iVar3;
}



void FUN_00418f64(int param_1)

{
  uint uVar1;
  
  if (param_1 < 0) {
    uVar1 = -param_1;
    if (0x13ff < (int)uVar1) {
      return;
    }
    if ((uVar1 >> 5 != 0) && (uVar1 >> 9 != 0)) {
      return;
    }
  }
  else if ((param_1 != 0) && (0x13ff < param_1)) {
    return;
  }
  return;
}



int FUN_00419238(int param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  return param_1 * param_4;
}



int FUN_0041927e(int param_1,uint param_2,undefined4 param_3,uint param_4,uint param_5)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  bool bVar4;
  bool bVar5;
  bool bVar6;
  
  if ((param_5 == 0) && ((param_2 == 0 || (param_4 == 0)))) {
    param_1 = (int)(CONCAT44(param_2,param_1) / (ulonglong)param_4);
  }
  else {
    bVar6 = (int)param_2 < 0;
    if (bVar6) {
      bVar4 = param_1 != 0;
      param_1 = -param_1;
      param_2 = -(uint)bVar4 - param_2;
    }
    if ((int)param_5 < 0) {
      bVar4 = param_4 != 0;
      param_4 = -param_4;
      param_5 = -(uint)bVar4 - param_5;
      bVar6 = !bVar6;
    }
    iVar1 = 0x40;
    uVar3 = 0;
    uVar2 = 0;
    do {
      bVar4 = param_1 < 0;
      param_1 = param_1 * 2;
      bVar5 = (int)param_2 < 0;
      param_2 = param_2 << 1 | (uint)bVar4;
      bVar4 = (int)uVar2 < 0;
      uVar2 = uVar2 << 1 | (uint)bVar5;
      uVar3 = uVar3 << 1 | (uint)bVar4;
      if ((param_5 <= uVar3) && ((param_5 < uVar3 || (param_4 <= uVar2)))) {
        bVar4 = uVar2 < param_4;
        uVar2 = uVar2 - param_4;
        uVar3 = (uVar3 - param_5) - (uint)bVar4;
        param_1 = param_1 + 1;
      }
      iVar1 = iVar1 + -1;
    } while (iVar1 != 0);
    if (bVar6) {
      param_1 = -param_1;
    }
  }
  return param_1;
}



int FUN_004192f6(uint param_1,uint param_2,undefined4 param_3,uint param_4,uint param_5)

{
  int iVar1;
  
  if (((param_5 & param_4) == 0xffffffff) && ((param_1 | param_2) == 0x80000000)) {
    return 0x7fffffff;
  }
  iVar1 = FUN_0041927e(param_1,param_2,param_3,param_4,param_5);
  return iVar1;
}



int FUN_00419328(int param_1,uint param_2,undefined4 param_3,uint param_4,uint param_5)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  bool bVar4;
  bool bVar5;
  
  if ((param_5 == 0) && ((param_2 == 0 || (param_4 == 0)))) {
    param_1 = (int)(CONCAT44(param_2,param_1) / (ulonglong)param_4);
  }
  else {
    iVar1 = 0x40;
    uVar3 = 0;
    uVar2 = 0;
    do {
      bVar4 = param_1 < 0;
      param_1 = param_1 * 2;
      bVar5 = (int)param_2 < 0;
      param_2 = param_2 << 1 | (uint)bVar4;
      bVar4 = (int)uVar2 < 0;
      uVar2 = uVar2 << 1 | (uint)bVar5;
      uVar3 = uVar3 << 1 | (uint)bVar4;
      if ((param_5 <= uVar3) && ((param_5 < uVar3 || (param_4 <= uVar2)))) {
        bVar4 = uVar2 < param_4;
        uVar2 = uVar2 - param_4;
        uVar3 = (uVar3 - param_5) - (uint)bVar4;
        param_1 = param_1 + 1;
      }
      iVar1 = iVar1 + -1;
    } while (iVar1 != 0);
  }
  return param_1;
}



uint FUN_00419371(int param_1,uint param_2,undefined4 param_3,uint param_4,uint param_5)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  bool bVar4;
  bool bVar5;
  bool bVar6;
  
  if ((param_5 == 0) && ((param_2 == 0 || (param_4 == 0)))) {
    uVar2 = (uint)(CONCAT44(param_2,param_1) % (ulonglong)param_4);
  }
  else {
    bVar6 = (int)param_2 < 0;
    if (bVar6) {
      bVar4 = param_1 != 0;
      param_1 = -param_1;
      param_2 = -(uint)bVar4 - param_2;
    }
    if ((int)param_5 < 0) {
      bVar4 = param_4 != 0;
      param_4 = -param_4;
      param_5 = -(uint)bVar4 - param_5;
    }
    iVar1 = 0x40;
    uVar3 = 0;
    uVar2 = 0;
    do {
      bVar4 = param_1 < 0;
      param_1 = param_1 * 2;
      bVar5 = (int)param_2 < 0;
      param_2 = param_2 << 1 | (uint)bVar4;
      bVar4 = (int)uVar2 < 0;
      uVar2 = uVar2 << 1 | (uint)bVar5;
      uVar3 = uVar3 << 1 | (uint)bVar4;
      if ((param_5 <= uVar3) && ((param_5 < uVar3 || (param_4 <= uVar2)))) {
        bVar4 = uVar2 < param_4;
        uVar2 = uVar2 - param_4;
        uVar3 = (uVar3 - param_5) - (uint)bVar4;
        param_1 = param_1 + 1;
      }
      iVar1 = iVar1 + -1;
    } while (iVar1 != 0);
    if (bVar6) {
      uVar2 = -uVar2;
    }
  }
  return uVar2;
}



uint FUN_004193eb(uint param_1,uint param_2,undefined4 param_3,uint param_4,uint param_5)

{
  uint uVar1;
  
  if (((param_5 & param_4) == 0xffffffff) && ((param_1 | param_2) == 0x80000000)) {
    return 0x7fffffff;
  }
  uVar1 = FUN_00419371(param_1,param_2,param_3,param_4,param_5);
  return uVar1;
}



uint FUN_0041941d(int param_1,uint param_2,undefined4 param_3,uint param_4,uint param_5)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  bool bVar4;
  bool bVar5;
  
  if ((param_5 == 0) && ((param_2 == 0 || (param_4 == 0)))) {
    uVar2 = (uint)(CONCAT44(param_2,param_1) % (ulonglong)param_4);
  }
  else {
    iVar1 = 0x40;
    uVar3 = 0;
    uVar2 = 0;
    do {
      bVar4 = param_1 < 0;
      param_1 = param_1 * 2;
      bVar5 = (int)param_2 < 0;
      param_2 = param_2 << 1 | (uint)bVar4;
      bVar4 = (int)uVar2 < 0;
      uVar2 = uVar2 << 1 | (uint)bVar5;
      uVar3 = uVar3 << 1 | (uint)bVar4;
      if ((param_5 <= uVar3) && ((param_5 < uVar3 || (param_4 <= uVar2)))) {
        bVar4 = uVar2 < param_4;
        uVar2 = uVar2 - param_4;
        uVar3 = (uVar3 - param_5) - (uint)bVar4;
        param_1 = param_1 + 1;
      }
      iVar1 = iVar1 + -1;
    } while (iVar1 != 0);
  }
  return uVar2;
}



WORD __stdcall FUN_0041967c(void)

{
  WORD WVar1;
  _STARTUPINFOA _Stack_48;
  
  GetStartupInfoA(&_Stack_48);
  WVar1 = 10;
  if (((byte)_Stack_48.dwFlags & 1) != 0) {
    WVar1 = _Stack_48.wShowWindow;
  }
  return WVar1;
}



int * __stdcall FUN_004196a0(void)

{
  undefined4 *puVar1;
  int *piVar2;
  int iVar3;
  
  if (DAT_00430e38 == (int *)0x0) {
    puVar1 = LocalAlloc(0,0x644);
    if (puVar1 == (undefined4 *)0x0) {
      return (int *)0x0;
    }
    *puVar1 = DAT_00430e34;
    iVar3 = 0;
    DAT_00430e34 = puVar1;
    do {
      piVar2 = puVar1 + iVar3 * 4 + 1;
      *piVar2 = (int)DAT_00430e38;
      iVar3 = iVar3 + 1;
      DAT_00430e38 = piVar2;
    } while (iVar3 != 100);
  }
  piVar2 = DAT_00430e38;
  DAT_00430e38 = (int *)*DAT_00430e38;
  return piVar2;
}



void FUN_004196f0(int param_1)

{
  *(int *)param_1 = param_1;
  *(int *)(param_1 + 4) = param_1;
  return;
}



undefined4 FUN_004196f8(int *param_1,int *param_2)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = FUN_004196a0();
  if (piVar2 == (int *)0x0) {
    return 0;
  }
  piVar2[2] = *param_2;
  piVar2[3] = param_2[1];
  iVar1 = *param_1;
  *piVar2 = iVar1;
  piVar2[1] = (int)param_1;
  *(int **)(iVar1 + 4) = piVar2;
  *param_1 = (int)piVar2;
  return CONCAT31((int3)((uint)piVar2 >> 8),1);
}



void FUN_00419728(int *param_1)

{
  int *piVar1;
  int iVar2;
  
  piVar1 = (int *)param_1[1];
  iVar2 = *param_1;
  *piVar1 = iVar2;
  *(int **)(iVar2 + 4) = piVar1;
  *param_1 = (int)DAT_00430e38;
  DAT_00430e38 = param_1;
  return;
}



void FUN_00419740(int *param_1,int *param_2,int *param_3)

{
  int *piVar1;
  undefined4 uVar2;
  int *piVar3;
  
  piVar3 = (int *)*param_1;
  *param_3 = *param_2;
  param_3[1] = param_2[1];
  do {
    piVar1 = (int *)*piVar3;
    if (piVar3[2] + piVar3[3] == *param_3) {
      FUN_00419728(piVar3);
      *param_3 = piVar3[2];
      param_3[1] = param_3[1] + piVar3[3];
    }
    else if (piVar3[2] == *param_3 + param_3[1]) {
      FUN_00419728(piVar3);
      param_3[1] = param_3[1] + piVar3[3];
    }
    piVar3 = piVar1;
  } while (param_1 != piVar1);
  uVar2 = FUN_004196f8(param_1,param_3);
  if ((char)uVar2 == '\0') {
    *param_3 = 0;
  }
  return;
}



undefined4 FUN_004197b4(int *param_1,uint *param_2)

{
  uint uVar1;
  uint uVar2;
  int *piVar3;
  int local_18;
  int local_14;
  
  piVar3 = param_1;
  while( true ) {
    uVar1 = *param_2;
    uVar2 = piVar3[2];
    if ((uVar2 <= uVar1) && (uVar1 + param_2[1] <= uVar2 + piVar3[3])) break;
    piVar3 = (int *)*piVar3;
    if (param_1 == piVar3) {
      return 0;
    }
  }
  if (uVar1 == uVar2) {
    piVar3[2] = piVar3[2] + param_2[1];
    uVar2 = param_2[1];
    piVar3[3] = piVar3[3] - uVar2;
    if (piVar3[3] == 0) {
      uVar2 = FUN_00419728(piVar3);
    }
  }
  else if (*param_2 + param_2[1] == uVar2 + piVar3[3]) {
    piVar3[3] = piVar3[3] - param_2[1];
  }
  else {
    local_18 = *param_2 + param_2[1];
    local_14 = (uVar2 + piVar3[3]) - local_18;
    piVar3[3] = *param_2 - uVar2;
    uVar2 = FUN_004196f8(piVar3,&local_18);
    if ((char)uVar2 == '\0') {
      return 0;
    }
  }
  return CONCAT31((int3)(uVar2 >> 8),1);
}



void FUN_00419844(int param_1,int *param_2)

{
  LPVOID pvVar1;
  undefined4 uVar2;
  SIZE_T dwSize;
  
  if (param_1 < 0x100000) {
    dwSize = 0x100000;
  }
  else {
    dwSize = param_1 + 0xffffU & 0xffff0000;
  }
  param_2[1] = dwSize;
  pvVar1 = VirtualAlloc((LPVOID)0x0,dwSize,0x2000,1);
  *param_2 = (int)pvVar1;
  if (pvVar1 != (LPVOID)0x0) {
    uVar2 = FUN_004196f8(&DAT_00430e3c,param_2);
    if ((char)uVar2 == '\0') {
      VirtualFree((LPVOID)*param_2,0,0x8000);
      *param_2 = 0;
    }
  }
  return;
}



void FUN_004198a8(LPVOID param_1,int param_2,int *param_3)

{
  LPVOID pvVar1;
  undefined4 uVar2;
  uint dwSize;
  
  param_3[1] = 0x100000;
  pvVar1 = VirtualAlloc(param_1,0x100000,0x2000,4);
  *param_3 = (int)pvVar1;
  if (pvVar1 == (LPVOID)0x0) {
    dwSize = param_2 + 0xffffU & 0xffff0000;
    param_3[1] = dwSize;
    pvVar1 = VirtualAlloc(param_1,dwSize,0x2000,4);
    *param_3 = (int)pvVar1;
  }
  if (*param_3 != 0) {
    uVar2 = FUN_004196f8(&DAT_00430e3c,param_3);
    if ((char)uVar2 == '\0') {
      VirtualFree((LPVOID)*param_3,0,0x8000);
      *param_3 = 0;
    }
  }
  return;
}



void FUN_00419920(LPVOID param_1,int param_2,undefined4 *param_3)

{
  int *piVar1;
  LPVOID lpAddress;
  BOOL BVar2;
  int *piVar3;
  LPVOID local_1c;
  uint local_18;
  
  local_1c = (LPVOID)0xffffffff;
  local_18 = 0;
  piVar1 = DAT_00430e3c;
  while (piVar3 = piVar1, (int **)piVar3 != &DAT_00430e3c) {
    piVar1 = (int *)*piVar3;
    lpAddress = (LPVOID)piVar3[2];
    if ((param_1 <= lpAddress) &&
       ((uint)((int)lpAddress + piVar3[3]) <= (uint)(param_2 + (int)param_1))) {
      if (lpAddress < local_1c) {
        local_1c = lpAddress;
      }
      if (local_18 < (uint)((int)lpAddress + piVar3[3])) {
        local_18 = (int)lpAddress + piVar3[3];
      }
      BVar2 = VirtualFree(lpAddress,0,0x8000);
      if (BVar2 == 0) {
        DAT_00430e18 = 1;
      }
      FUN_00419728(piVar3);
    }
  }
  *param_3 = 0;
  if (local_18 != 0) {
    *param_3 = local_1c;
    param_3[1] = local_18 - (int)local_1c;
  }
  return;
}



void FUN_004199d8(uint param_1,int param_2,undefined4 *param_3)

{
  LPVOID pvVar1;
  LPVOID pvVar2;
  LPVOID lpAddress;
  LPVOID pvVar3;
  undefined4 *puVar4;
  LPVOID pvVar5;
  
  pvVar3 = (LPVOID)(param_1 & 0xfffff000);
  pvVar2 = (LPVOID)(param_1 + param_2 + 0xfff & 0xfffff000);
  *param_3 = pvVar3;
  param_3[1] = (int)pvVar2 - (int)pvVar3;
  puVar4 = DAT_00430e3c;
  while( true ) {
    if ((undefined4 **)puVar4 == &DAT_00430e3c) {
      return;
    }
    pvVar1 = (LPVOID)puVar4[2];
    lpAddress = pvVar1;
    if (pvVar1 < pvVar3) {
      lpAddress = pvVar3;
    }
    pvVar5 = (LPVOID)(puVar4[3] + (int)pvVar1);
    if (pvVar2 < (LPVOID)(puVar4[3] + (int)pvVar1)) {
      pvVar5 = pvVar2;
    }
    if ((lpAddress < pvVar5) &&
       (pvVar1 = VirtualAlloc(lpAddress,(int)pvVar5 - (int)lpAddress,0x1000,4),
       pvVar1 == (LPVOID)0x0)) break;
    puVar4 = (undefined4 *)*puVar4;
  }
  *param_3 = 0;
  return;
}



void FUN_00419a6c(int param_1,int param_2,undefined4 *param_3)

{
  LPVOID pvVar1;
  BOOL BVar2;
  LPVOID lpAddress;
  LPVOID pvVar3;
  LPVOID pvVar4;
  undefined4 *puVar5;
  LPVOID pvVar6;
  
  pvVar4 = (LPVOID)(param_1 + 0xfffU & 0xfffff000);
  pvVar3 = (LPVOID)(param_1 + param_2 & 0xfffff000);
  *param_3 = pvVar4;
  param_3[1] = (int)pvVar3 - (int)pvVar4;
  for (puVar5 = DAT_00430e3c; (undefined4 **)puVar5 != &DAT_00430e3c; puVar5 = (undefined4 *)*puVar5
      ) {
    pvVar1 = (LPVOID)puVar5[2];
    lpAddress = pvVar1;
    if (pvVar1 < pvVar4) {
      lpAddress = pvVar4;
    }
    pvVar6 = (LPVOID)(puVar5[3] + (int)pvVar1);
    if (pvVar3 < (LPVOID)(puVar5[3] + (int)pvVar1)) {
      pvVar6 = pvVar3;
    }
    if (lpAddress < pvVar6) {
      BVar2 = VirtualFree(lpAddress,(int)pvVar6 - (int)lpAddress,0x4000);
      if (BVar2 == 0) {
        DAT_00430e18 = 2;
      }
    }
  }
  return;
}



void FUN_00419aec(int param_1,int *param_2)

{
  int *piVar1;
  uint uVar2;
  int local_18 [2];
  
  uVar2 = param_1 + 0x3fffU & 0xffffc000;
  piVar1 = DAT_00430e4c;
  do {
    for (; (int **)piVar1 != &DAT_00430e4c; piVar1 = (int *)*piVar1) {
      if ((int)uVar2 <= piVar1[3]) {
        FUN_004199d8(piVar1[2],uVar2,param_2);
        if (*param_2 == 0) {
          return;
        }
        piVar1[2] = piVar1[2] + param_2[1];
        piVar1[3] = piVar1[3] - param_2[1];
        if (piVar1[3] != 0) {
          return;
        }
        FUN_00419728(piVar1);
        return;
      }
    }
    FUN_00419844(uVar2,param_2);
    if (*param_2 == 0) {
      return;
    }
    FUN_00419740((int *)&DAT_00430e4c,param_2,local_18);
    piVar1 = DAT_00430e4c;
  } while (local_18[0] != 0);
  FUN_00419920((LPVOID)*param_2,param_2[1],local_18);
  *param_2 = 0;
  return;
}



void FUN_00419b7c(LPVOID param_1,int param_2,int *param_3)

{
  int *piVar1;
  uint uVar2;
  LPVOID local_20;
  int local_1c;
  int local_18 [2];
  
  uVar2 = param_2 + 0x3fffU & 0xffffc000;
  piVar1 = DAT_00430e4c;
LAB_00419ba2:
  do {
    for (; ((int **)piVar1 != &DAT_00430e4c && (param_1 != (LPVOID)piVar1[2]));
        piVar1 = (int *)*piVar1) {
    }
    if (param_1 == (LPVOID)piVar1[2]) {
      if ((int)uVar2 <= piVar1[3]) goto LAB_00419c4f;
      FUN_004198a8((LPVOID)(piVar1[2] + piVar1[3]),uVar2 - piVar1[3],(int *)&local_20);
      if (local_20 != (LPVOID)0x0) {
        FUN_00419740((int *)&DAT_00430e4c,(int *)&local_20,local_18);
        piVar1 = DAT_00430e4c;
        if (local_18[0] == 0) {
          FUN_00419920(local_20,local_1c,local_18);
          *param_3 = 0;
          return;
        }
        goto LAB_00419ba2;
      }
    }
    FUN_004198a8(param_1,uVar2,(int *)&local_20);
    if (local_20 == (LPVOID)0x0) {
LAB_00419c4f:
      if ((param_1 != (LPVOID)piVar1[2]) || (piVar1[3] < (int)uVar2)) {
        *param_3 = 0;
        return;
      }
      FUN_004199d8((uint)piVar1[2],uVar2,param_3);
      if (*param_3 == 0) {
        return;
      }
      piVar1[2] = piVar1[2] + param_3[1];
      piVar1[3] = piVar1[3] - param_3[1];
      if (piVar1[3] != 0) {
        return;
      }
      FUN_00419728(piVar1);
      return;
    }
    FUN_00419740((int *)&DAT_00430e4c,(int *)&local_20,local_18);
    piVar1 = DAT_00430e4c;
    if (local_18[0] == 0) {
      FUN_00419920(local_20,local_1c,local_18);
      *param_3 = 0;
      return;
    }
  } while( true );
}



void FUN_00419ca0(int param_1,int param_2,int *param_3)

{
  uint uVar1;
  uint uVar2;
  LPVOID local_1c;
  int local_18;
  LPVOID local_14;
  int local_10;
  
  uVar1 = param_1 + 0x3fffU & 0xffffc000;
  uVar2 = param_2 + param_1 & 0xffffc000;
  if (uVar1 < uVar2) {
    FUN_00419a6c(uVar1,uVar2 - uVar1,param_3);
    FUN_00419740(&DAT_00430e4c,param_3,(int *)&local_1c);
    if (local_1c != (LPVOID)0x0) {
      FUN_00419920(local_1c,local_18,&local_14);
      local_1c = local_14;
      local_18 = local_10;
    }
    if (local_1c != (LPVOID)0x0) {
      FUN_004197b4(&DAT_00430e4c,(uint *)&local_1c);
    }
  }
  else {
    *param_3 = 0;
  }
  return;
}



void __stdcall FUN_00419d2c(void)

{
  int iVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_10;
  
  uStack_10 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_10;
  InitializeCriticalSection((LPCRITICAL_SECTION)&lpCriticalSection_00430e1c);
  if (DAT_00430a2d != '\0') {
    EnterCriticalSection((LPCRITICAL_SECTION)&lpCriticalSection_00430e1c);
  }
  FUN_004196f0(0x430e3c);
  FUN_004196f0(0x430e4c);
  FUN_004196f0(0x430e78);
  DAT_00430e74 = LocalAlloc(0,0xff8);
  if (DAT_00430e74 != (HLOCAL)0x0) {
    iVar1 = 3;
    do {
      *(undefined4 *)((int)DAT_00430e74 + iVar1 * 4 + -0xc) = 0;
      iVar1 = iVar1 + 1;
    } while (iVar1 != 0x401);
    DAT_00430e60 = &DAT_00430e5c;
    DAT_00430e5c = &DAT_00430e5c;
    DAT_00430e68 = &DAT_00430e5c;
    DAT_00430e14 = 1;
  }
  *in_FS_OFFSET = uStack_10;
  if (DAT_00430a2d != '\0') {
    uStack_10 = 0x419de1;
    LeaveCriticalSection((LPCRITICAL_SECTION)&lpCriticalSection_00430e1c);
  }
  return;
}



void __stdcall FUN_00419df0(void)

{
  undefined4 *puVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_14;
  
  if (DAT_00430e14 == '\0') {
    return;
  }
  uStack_14 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_14;
  if (DAT_00430a2d != '\0') {
    EnterCriticalSection((LPCRITICAL_SECTION)&lpCriticalSection_00430e1c);
  }
  DAT_00430e14 = 0;
  LocalFree(DAT_00430e74);
  DAT_00430e74 = (HLOCAL)0x0;
  for (puVar1 = DAT_00430e3c; (undefined4 **)puVar1 != &DAT_00430e3c; puVar1 = (undefined4 *)*puVar1
      ) {
    VirtualFree((LPVOID)puVar1[2],0,0x8000);
  }
  FUN_004196f0(0x430e3c);
  FUN_004196f0(0x430e4c);
  FUN_004196f0(0x430e78);
  puVar1 = DAT_00430e34;
  while (puVar1 != (undefined4 *)0x0) {
    DAT_00430e34 = (undefined4 *)*puVar1;
    LocalFree(puVar1);
    puVar1 = DAT_00430e34;
  }
  DAT_00430e34 = puVar1;
  *in_FS_OFFSET = uStack_14;
  if (DAT_00430a2d != '\0') {
    uStack_14 = 0x419ebb;
    LeaveCriticalSection((LPCRITICAL_SECTION)&lpCriticalSection_00430e1c);
  }
  uStack_14 = 0x419ec5;
  DeleteCriticalSection((LPCRITICAL_SECTION)&lpCriticalSection_00430e1c);
  return;
}



void FUN_00419ed0(int *param_1)

{
  int *piVar1;
  int iVar2;
  
  if (param_1 == DAT_00430e68) {
    DAT_00430e68 = (int *)param_1[1];
  }
  piVar1 = (int *)param_1[1];
  iVar2 = param_1[2];
  if (iVar2 < 0x1001) {
    if (param_1 != piVar1) {
      if (iVar2 < 0) {
        iVar2 = iVar2 + 3;
      }
      *(int **)(DAT_00430e74 + -0xc + (iVar2 >> 2) * 4) = piVar1;
      iVar2 = *param_1;
      *piVar1 = iVar2;
      *(int **)(iVar2 + 4) = piVar1;
      return;
    }
    if (iVar2 < 0) {
      iVar2 = iVar2 + 3;
    }
    *(undefined4 *)(DAT_00430e74 + -0xc + (iVar2 >> 2) * 4) = 0;
  }
  else {
    iVar2 = *param_1;
    *piVar1 = iVar2;
    *(int **)(iVar2 + 4) = piVar1;
  }
  return;
}



undefined4 * FUN_00419f34(uint param_1)

{
  undefined4 *puVar1;
  
  puVar1 = DAT_00430e78;
  while( true ) {
    if ((undefined4 **)puVar1 == &DAT_00430e78) {
      DAT_00430e18 = 3;
      return (undefined4 *)0x0;
    }
    if (((uint)puVar1[2] <= param_1) && (param_1 < (uint)(puVar1[2] + puVar1[3]))) break;
    puVar1 = (undefined4 *)*puVar1;
  }
  return puVar1;
}



void FUN_00419f64(uint *param_1,uint param_2)

{
  uint *puVar1;
  
  puVar1 = (uint *)((param_2 - 4) + (int)param_1);
  if (0xf < (int)param_2) {
    *puVar1 = 0x80000007;
    FUN_0041a120(param_1,param_2 - 4);
    return;
  }
  if (3 < (int)param_2) {
    *param_1 = param_2 | 0x80000002;
    *puVar1 = param_2 | 0x80000002;
  }
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_00419f94(int param_1)

{
  _DAT_00430e08 = _DAT_00430e08 + 1;
  _DAT_00430e0c = _DAT_00430e0c + ((*(uint *)(param_1 + -4) & 0x7ffffffc) - 4);
  FUN_0041a588(param_1);
  return;
}



void FUN_00419fb8(uint *param_1,uint param_2)

{
  if (0xb < (int)param_2) {
    *param_1 = param_2 | 2;
    FUN_00419f94((int)(param_1 + 1));
    return;
  }
  if (3 < (int)param_2) {
    *param_1 = param_2 | 0x80000002;
  }
  *(uint *)((int)param_1 + param_2) = *(uint *)((int)param_1 + param_2) & 0xfffffffe;
  return;
}



uint FUN_00419fe0(int param_1)

{
  uint uVar1;
  uint *puVar2;
  uint uVar3;
  
  uVar1 = *(uint *)(param_1 + -4);
  if ((uVar1 & 0x80000002) != 0x80000002) {
    DAT_00430e18 = 4;
  }
  uVar3 = uVar1 & 0x7ffffffc;
  puVar2 = (uint *)(param_1 - uVar3);
  if (((uVar1 ^ *puVar2) & 0xfffffffe) != 0) {
    DAT_00430e18 = 5;
  }
  if ((*puVar2 & 1) != 0) {
    uVar1 = puVar2[-1];
    if (uVar1 != ((int *)((int)puVar2 - uVar1))[2]) {
      DAT_00430e18 = 6;
    }
    FUN_00419ed0((int *)((int)puVar2 - uVar1));
    uVar3 = uVar3 + uVar1;
  }
  return uVar3;
}



uint FUN_0041a050(uint *param_1)

{
  uint uVar1;
  uint *puVar2;
  uint uVar3;
  
  uVar3 = 0;
  uVar1 = *param_1;
  if ((uVar1 & 0x80000000) != 0) {
    uVar3 = uVar1 & 0x7ffffffc;
    param_1 = (uint *)((int)param_1 + uVar3);
    uVar1 = *param_1;
  }
  if ((uVar1 & 2) == 0) {
    FUN_00419ed0((int *)param_1);
    uVar3 = uVar3 + param_1[2];
    puVar2 = (uint *)((int)param_1 + param_1[2]);
    *puVar2 = *puVar2 & 0xfffffffe;
  }
  return uVar3;
}



undefined4 FUN_0041a088(uint *param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  undefined4 uVar3;
  uint *puVar4;
  uint local_18;
  int local_14;
  
  piVar2 = FUN_00419f34((uint)param_1);
  iVar1 = piVar2[2];
  if ((iVar1 + piVar2[3]) - (param_2 + (int)param_1) < 0xd) {
    param_2 = (iVar1 + piVar2[3]) - (int)param_1;
  }
  if ((int)param_1 - iVar1 < 0xc) {
    FUN_00419ca0(iVar1,(int)param_1 + (param_2 - piVar2[2]),(int *)&local_18);
  }
  else {
    FUN_00419ca0((int)(param_1 + 1),param_2 + -4,(int *)&local_18);
  }
  if (local_18 == 0) {
    uVar3 = 0;
  }
  else {
    FUN_00419f64(param_1,local_18 - (int)param_1);
    puVar4 = (uint *)(local_18 + local_14);
    if (puVar4 < (uint *)(piVar2[2] + piVar2[3])) {
      FUN_00419fb8(puVar4,(int)param_1 + (param_2 - (int)puVar4));
    }
    uVar3 = FUN_004197b4(piVar2,&local_18);
    uVar3 = CONCAT31((int3)((uint)uVar3 >> 8),1);
  }
  return uVar3;
}



void FUN_0041a120(uint *param_1,uint param_2)

{
  uint *puVar1;
  uint uVar2;
  uint *puVar3;
  undefined4 uVar4;
  
  param_1[2] = param_2;
  *(uint *)((int)param_1 + (param_2 - 4)) = param_2;
  if ((int)param_2 < 0x1001) {
    if ((int)param_2 < 0) {
      param_2 = param_2 + 3;
    }
    puVar1 = *(uint **)(DAT_00430e74 + -0xc + ((int)param_2 >> 2) * 4);
    if (puVar1 == (uint *)0x0) {
      *(uint **)(DAT_00430e74 + -0xc + ((int)param_2 >> 2) * 4) = param_1;
      param_1[1] = (uint)param_1;
      *param_1 = (uint)param_1;
    }
    else {
      uVar2 = *puVar1;
      param_1[1] = (uint)puVar1;
      *param_1 = uVar2;
      *puVar1 = (uint)param_1;
      *(uint **)(uVar2 + 4) = param_1;
    }
  }
  else {
    if ((0x3bff < (int)param_2) && (uVar4 = FUN_0041a088(param_1,param_2), (char)uVar4 != '\0')) {
      return;
    }
    puVar1 = DAT_00430e68;
    uVar2 = *DAT_00430e68;
    puVar3 = param_1;
    param_1[1] = (uint)DAT_00430e68;
    DAT_00430e68 = puVar3;
    *param_1 = uVar2;
    *puVar1 = (uint)param_1;
    *(uint **)(uVar2 + 4) = param_1;
  }
  return;
}



void __stdcall FUN_0041a1a8(void)

{
  if (0 < (int)DAT_00430e6c) {
    if ((int)DAT_00430e6c < 0xc) {
      DAT_00430e18 = 7;
    }
    else {
      *DAT_00430e70 = DAT_00430e6c | 2;
      FUN_00419f94((int)(DAT_00430e70 + 1));
      DAT_00430e70 = (uint *)0x0;
      DAT_00430e6c = 0;
    }
  }
  return;
}



undefined4 FUN_0041a1f4(uint *param_1)

{
  undefined4 uVar1;
  uint uVar2;
  uint local_1c;
  uint local_18;
  uint local_14;
  int local_10;
  
  local_1c = *param_1;
  local_18 = param_1[1];
  FUN_0041a1a8();
  FUN_00419740(&DAT_00430e78,(int *)&local_1c,(int *)&local_14);
  if (local_14 == 0) {
    uVar1 = 0;
  }
  else {
    if (local_14 < local_1c) {
      uVar2 = FUN_00419fe0(local_1c);
      local_1c = local_1c - uVar2;
      local_18 = local_18 + uVar2;
    }
    if ((uint *)(local_1c + local_18) < (uint *)(local_14 + local_10)) {
      uVar2 = FUN_0041a050((uint *)(local_1c + local_18));
      local_18 = local_18 + uVar2;
    }
    if ((uint *)(local_14 + local_10) == (uint *)(local_1c + local_18)) {
      FUN_00419f64((uint *)(local_1c + local_18) + -1,4);
      local_18 = local_18 - 4;
    }
    DAT_00430e70 = local_1c;
    DAT_00430e6c = local_18;
    uVar1 = CONCAT31((int3)(local_18 >> 8),1);
  }
  return uVar1;
}



undefined4 FUN_0041a280(int param_1)

{
  undefined4 uVar1;
  uint local_c [2];
  
  FUN_00419aec(param_1 + 4,(int *)local_c);
  if ((local_c[0] != 0) && (uVar1 = FUN_0041a1f4(local_c), (char)uVar1 != '\0')) {
    return CONCAT31((int3)((uint)uVar1 >> 8),1);
  }
  return 0;
}



undefined4 FUN_0041a2ac(LPVOID param_1,int param_2)

{
  undefined4 uVar1;
  uint local_10 [2];
  
  FUN_00419b7c(param_1,param_2 + 4,(int *)local_10);
  if ((local_10[0] != 0) && (uVar1 = FUN_0041a1f4(local_10), (char)uVar1 != '\0')) {
    return CONCAT31((int3)((uint)uVar1 >> 8),1);
  }
  return 0;
}



int FUN_0041a2e0(int param_1)

{
  int iVar1;
  int iVar2;
  
  if (param_1 < 0) {
    param_1 = param_1 + 3;
  }
  iVar2 = param_1 >> 2;
  if (iVar2 < 0x401) {
    do {
      iVar1 = *(int *)(DAT_00430e74 + -0xc + iVar2 * 4);
      if (iVar1 != 0) {
        return iVar1;
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 != 0x401);
  }
  return 0;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

uint * FUN_0041a30c(uint param_1)

{
  uint *puVar1;
  uint *puVar2;
  undefined4 uVar3;
  uint uVar4;
  
  while ((puVar1 = DAT_00430e68, puVar2 = DAT_00430e60, (int)DAT_00430e60[2] < (int)param_1 &&
         (uVar4 = DAT_00430e68[2], puVar2 = DAT_00430e68, (int)uVar4 < (int)param_1))) {
    DAT_00430e68[2] = param_1;
    puVar2 = puVar1;
    do {
      puVar2 = (uint *)puVar2[1];
    } while ((int)puVar2[2] < (int)param_1);
    DAT_00430e68[2] = uVar4;
    puVar1 = puVar2;
    if ((puVar2 != DAT_00430e68) ||
       (((int)param_1 < 0x1001 &&
        (puVar2 = (uint *)FUN_0041a2e0(param_1), puVar1 = DAT_00430e68, puVar2 != (uint *)0x0))))
    break;
    uVar3 = FUN_0041a280(param_1);
    puVar2 = DAT_00430e70;
    if ((char)uVar3 == '\0') {
      return (uint *)0x0;
    }
    if ((int)param_1 <= DAT_00430e6c) {
      DAT_00430e6c = DAT_00430e6c - param_1;
      if (DAT_00430e6c < 0xc) {
        param_1 = param_1 + DAT_00430e6c;
        DAT_00430e6c = 0;
      }
      DAT_00430e70 = (uint *)((int)DAT_00430e70 + param_1);
      *puVar2 = param_1 | 2;
      _DAT_00430e08 = _DAT_00430e08 + 1;
      _DAT_00430e0c = _DAT_00430e0c + (param_1 - 4);
      return puVar2 + 1;
    }
  }
  DAT_00430e68 = puVar1;
  FUN_00419ed0((int *)puVar2);
  uVar4 = puVar2[2];
  if ((int)(uVar4 - param_1) < 0xc) {
    if (puVar2 == DAT_00430e68) {
      DAT_00430e68 = (uint *)puVar2[1];
    }
    *(uint *)((int)puVar2 + uVar4) = *(uint *)((int)puVar2 + uVar4) & 0xfffffffe;
  }
  else {
    FUN_0041a120((uint *)((int)puVar2 + param_1),uVar4 - param_1);
    uVar4 = param_1;
  }
  *puVar2 = uVar4 | 2;
  _DAT_00430e08 = _DAT_00430e08 + 1;
  _DAT_00430e0c = _DAT_00430e0c + (uVar4 - 4);
  return puVar2 + 1;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

uint * FUN_0041a400(int param_1)

{
  uint *puVar1;
  char cVar2;
  uint uVar3;
  uint *puVar4;
  uint uVar5;
  undefined4 *in_FS_OFFSET;
  undefined4 uVar6;
  undefined1 *puVar7;
  
  if ((DAT_00430e14 == '\0') && (cVar2 = FUN_00419d2c(), cVar2 == '\0')) {
    return (uint *)0x0;
  }
  puVar7 = &LAB_0041a574;
  uVar6 = *in_FS_OFFSET;
  *in_FS_OFFSET = &stack0xffffffdc;
  if (DAT_00430a2d != '\0') {
    EnterCriticalSection((LPCRITICAL_SECTION)&lpCriticalSection_00430e1c);
  }
  puVar4 = DAT_00430e70;
  uVar5 = param_1 + 7U & 0xfffffffc;
  if ((int)uVar5 < 0xc) {
    uVar5 = 0xc;
  }
  if ((int)uVar5 < 0x1001) {
    uVar3 = uVar5;
    if ((int)uVar5 < 0) {
      uVar3 = uVar5 + 3;
    }
    puVar1 = *(uint **)(DAT_00430e74 + -0xc + ((int)uVar3 >> 2) * 4);
    if (puVar1 != (uint *)0x0) {
      *(uint *)((int)puVar1 + uVar5) = *(uint *)((int)puVar1 + uVar5) & 0xfffffffe;
      puVar4 = (uint *)puVar1[1];
      if (puVar1 == puVar4) {
        uVar3 = uVar5;
        if ((int)uVar5 < 0) {
          uVar3 = uVar5 + 3;
        }
        *(undefined4 *)(DAT_00430e74 + -0xc + ((int)uVar3 >> 2) * 4) = 0;
      }
      else {
        uVar3 = uVar5;
        if ((int)uVar5 < 0) {
          uVar3 = uVar5 + 3;
        }
        *(uint **)(DAT_00430e74 + -0xc + ((int)uVar3 >> 2) * 4) = puVar4;
        uVar3 = *puVar1;
        *(uint **)(uVar3 + 4) = puVar4;
        *puVar4 = uVar3;
      }
      *puVar1 = puVar1[2] | 2;
      _DAT_00430e08 = _DAT_00430e08 + 1;
      _DAT_00430e0c = _DAT_00430e0c + (uVar5 - 4);
      FUN_0041b5a8(uVar6,(int)puVar7);
      return puVar1 + 1;
    }
  }
  if ((int)uVar5 <= DAT_00430e6c) {
    DAT_00430e6c = DAT_00430e6c - uVar5;
    if (DAT_00430e6c < 0xc) {
      uVar5 = uVar5 + DAT_00430e6c;
      DAT_00430e6c = 0;
    }
    DAT_00430e70 = (uint *)((int)DAT_00430e70 + uVar5);
    *puVar4 = uVar5 | 2;
    _DAT_00430e08 = _DAT_00430e08 + 1;
    _DAT_00430e0c = _DAT_00430e0c + (uVar5 - 4);
    FUN_0041b5a8(uVar6,(int)puVar7);
    return puVar4 + 1;
  }
  FUN_0041a30c(uVar5);
  puVar4 = (uint *)0x0;
  *in_FS_OFFSET = uVar6;
  if (DAT_00430a2d != '\0') {
    LeaveCriticalSection((LPCRITICAL_SECTION)&lpCriticalSection_00430e1c);
  }
  return puVar4;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_0041a588(int param_1)

{
  uint uVar1;
  char cVar2;
  uint *puVar3;
  undefined4 uVar4;
  uint uVar5;
  uint *puVar6;
  undefined4 *in_FS_OFFSET;
  undefined4 uVar7;
  undefined1 *puVar8;
  
  DAT_00430e18 = 0;
  if ((DAT_00430e14 == '\0') && (cVar2 = FUN_00419d2c(), cVar2 == '\0')) {
    DAT_00430e18 = 8;
    return 8;
  }
  puVar8 = &LAB_0041a71a;
  uVar7 = *in_FS_OFFSET;
  *in_FS_OFFSET = &stack0xffffffe0;
  if (DAT_00430a2d != '\0') {
    EnterCriticalSection((LPCRITICAL_SECTION)&lpCriticalSection_00430e1c);
  }
  puVar6 = (uint *)(param_1 + -4);
  uVar5 = *puVar6;
  if ((uVar5 & 2) == 0) {
    DAT_00430e18 = 9;
    goto LAB_0041a6f1;
  }
  _DAT_00430e08 = _DAT_00430e08 + -1;
  _DAT_00430e0c = _DAT_00430e0c - ((uVar5 & 0x7ffffffc) - 4);
  if ((uVar5 & 1) != 0) {
    uVar1 = *(uint *)(param_1 + -8);
    if (((int)uVar1 < 0xc) || ((uVar1 & 0x80000003) != 0)) {
      DAT_00430e18 = 10;
      goto LAB_0041a6f1;
    }
    puVar6 = (uint *)((int)puVar6 - uVar1);
    if (uVar1 != puVar6[2]) {
      DAT_00430e18 = 10;
      goto LAB_0041a6f1;
    }
    uVar5 = uVar5 + uVar1;
    FUN_00419ed0((int *)puVar6);
  }
  uVar5 = uVar5 & 0x7ffffffc;
  puVar3 = (uint *)((int)puVar6 + uVar5);
  if (puVar3 == DAT_00430e70) {
    DAT_00430e70 = (uint *)((int)DAT_00430e70 - uVar5);
    DAT_00430e6c = DAT_00430e6c + uVar5;
    if (0x3c00 < DAT_00430e6c) {
      FUN_0041a1a8();
    }
    FUN_0041b5a8(uVar7,(int)puVar8);
    return 0;
  }
  if ((*puVar3 & 2) == 0) {
    if (((puVar3[1] == 0) || (*puVar3 == 0)) || ((int)puVar3[2] < 0xc)) {
      DAT_00430e18 = 0xb;
      goto LAB_0041a6f1;
    }
    uVar5 = uVar5 + puVar3[2];
    FUN_00419ed0((int *)puVar3);
  }
  else {
    if ((*puVar3 & 0x7ffffffc) < 4) {
      DAT_00430e18 = 0xb;
      goto LAB_0041a6f1;
    }
    *puVar3 = *puVar3 | 1;
  }
  FUN_0041a120(puVar6,uVar5);
LAB_0041a6f1:
  uVar4 = 0;
  *in_FS_OFFSET = uVar7;
  if (DAT_00430a2d != '\0') {
    LeaveCriticalSection((LPCRITICAL_SECTION)&lpCriticalSection_00430e1c);
  }
  return uVar4;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_0041a72c(int param_1,int param_2)

{
  uint uVar1;
  byte *pbVar2;
  undefined4 uVar3;
  uint *puVar4;
  uint *puVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint local_18;
  int local_14;
  
  uVar6 = param_2 + 7U & 0xfffffffc;
  if ((int)uVar6 < 0xc) {
    uVar6 = 0xc;
  }
  puVar5 = (uint *)(param_1 + -4);
  uVar8 = *puVar5 & 0x7ffffffc;
  pbVar2 = (byte *)((int)puVar5 + uVar8);
  uVar7 = uVar6;
  if ((int)uVar8 < (int)uVar6) {
    do {
      local_14 = uVar6 - uVar8;
      if ((byte *)((int)puVar5 + uVar8) == DAT_00430e70) {
        if (local_14 <= DAT_00430e6c) {
          DAT_00430e6c = DAT_00430e6c - local_14;
          DAT_00430e70 = DAT_00430e70 + local_14;
          if (DAT_00430e6c < 0xc) {
            DAT_00430e70 = DAT_00430e70 + DAT_00430e6c;
            uVar6 = uVar6 + DAT_00430e6c;
            DAT_00430e6c = 0;
          }
          _DAT_00430e0c = _DAT_00430e0c + (uVar6 - uVar8);
          uVar7 = *puVar5;
          *puVar5 = uVar6 | uVar7 & 0x80000003;
          return CONCAT31((int3)((uVar7 & 0x80000003) >> 8),1);
        }
        FUN_0041a1a8();
      }
      puVar4 = (uint *)((int)puVar5 + uVar8);
      if ((*puVar4 & 2) == 0) {
        uVar1 = puVar4[2];
        if (local_14 <= (int)uVar1) {
          FUN_00419ed0((int *)puVar4);
          uVar1 = uVar1 - local_14;
          if ((int)uVar1 < 0xc) {
            puVar4 = (uint *)((int)puVar5 + uVar6 + uVar1);
            *puVar4 = *puVar4 & 0xfffffffe;
            uVar7 = uVar6 + uVar1;
          }
          else {
            FUN_0041a120((uint *)((int)puVar5 + uVar6),uVar1);
          }
          goto LAB_0041a8db;
        }
        puVar4 = (uint *)((int)puVar4 + uVar1);
        local_14 = local_14 - uVar1;
      }
    } while (((*puVar4 & 0x80000000) != 0) &&
            (uVar3 = FUN_0041a2ac((byte *)((*puVar4 & 0x7ffffffc) + (int)puVar4),local_14),
            (char)uVar3 != '\0'));
    uVar3 = 0;
  }
  else {
    local_18 = uVar8 - uVar6;
    if (pbVar2 == DAT_00430e70) {
      DAT_00430e70 = DAT_00430e70 + -local_18;
      DAT_00430e6c = DAT_00430e6c + local_18;
      if (DAT_00430e6c < 0xc) {
        DAT_00430e70 = DAT_00430e70 + local_18;
        DAT_00430e6c = DAT_00430e6c - local_18;
        uVar7 = uVar8;
      }
    }
    else {
      if ((*pbVar2 & 2) == 0) {
        local_18 = local_18 + *(int *)(pbVar2 + 8);
        FUN_00419ed0((int *)pbVar2);
      }
      uVar7 = uVar8;
      if (0xb < (int)local_18) {
        *(uint *)((int)puVar5 + uVar6) = local_18 | 2;
        FUN_00419f94((int)((uint *)((int)puVar5 + uVar6) + 1));
        uVar7 = uVar6;
      }
    }
LAB_0041a8db:
    _DAT_00430e0c = _DAT_00430e0c + (uVar7 - uVar8);
    uVar6 = *puVar5;
    *puVar5 = uVar7 | uVar6 & 0x80000003;
    uVar3 = CONCAT31((int3)((uVar6 & 0x80000003) >> 8),1);
  }
  return uVar3;
}



undefined4 FUN_0041a8fc(undefined4 *param_1,uint param_2)

{
  char cVar1;
  undefined4 uVar2;
  uint *puVar3;
  uint uVar4;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_20;
  
  if ((DAT_00430e14 == '\0') && (cVar1 = FUN_00419d2c(), cVar1 == '\0')) {
    return 0;
  }
  uStack_20 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_20;
  if (DAT_00430a2d != '\0') {
    EnterCriticalSection((LPCRITICAL_SECTION)&lpCriticalSection_00430e1c);
  }
  uVar2 = FUN_0041a72c((int)param_1,param_2);
  if ((char)uVar2 == '\0') {
    puVar3 = FUN_0041a400(param_2);
    uVar4 = (param_1[-1] & 0x7ffffffc) - 4;
    if ((int)param_2 < (int)uVar4) {
      uVar4 = param_2;
    }
    if (puVar3 != (uint *)0x0) {
      FUN_0041aae4(param_1,puVar3,uVar4);
      FUN_0041a588((int)param_1);
    }
  }
  uVar2 = 0;
  *in_FS_OFFSET = uStack_20;
  if (DAT_00430a2d != '\0') {
    uStack_20 = 0x41a9ac;
    LeaveCriticalSection((LPCRITICAL_SECTION)&lpCriticalSection_00430e1c);
  }
  return uVar2;
}



void FUN_0041a9c0(int param_1)

{
  int iVar1;
  
  if (param_1 != 0) {
    iVar1 = (*(code *)PTR_FUN_0042d704)();
    if (iVar1 == 0) {
      FUN_0041aa4c(1);
      return;
    }
  }
  return;
}



void FUN_0041a9d8(int param_1)

{
  int iVar1;
  
  if (param_1 != 0) {
    iVar1 = (*(code *)PTR_FUN_0042d708)();
    if (iVar1 != 0) {
      FUN_0041aa4c(CONCAT31((int3)((uint)iVar1 >> 8),2));
      return;
    }
  }
  return;
}



void FUN_0041a9f0(int *param_1,int param_2)

{
  int iVar1;
  
  iVar1 = *param_1;
  if (iVar1 != 0) {
    if (param_2 == 0) {
      *param_1 = 0;
      iVar1 = (*(code *)PTR_FUN_0042d708)(iVar1);
      if (iVar1 == 0) {
        return;
      }
      FUN_0041aa4c(CONCAT31((int3)((uint)iVar1 >> 8),2));
      return;
    }
    iVar1 = (*(code *)PTR_FUN_0042d70c)(iVar1);
    if (iVar1 != 0) {
      *param_1 = iVar1;
      return;
    }
LAB_0041aa21:
    FUN_0041aa4c(1);
    return;
  }
  if (param_2 != 0) {
    iVar1 = (*(code *)PTR_FUN_0042d704)(param_2);
    if (iVar1 == 0) goto LAB_0041aa21;
    *param_1 = iVar1;
  }
  return;
}



undefined4 __stdcall FUN_0041aa40(void)

{
  int iVar1;
  
  iVar1 = FUN_00401202();
  return *(undefined4 *)(iVar1 + 8);
}



void FUN_0041aa4c(uint param_1)

{
  uint uVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 in_stack_00000000;
  
  uVar1 = param_1 & 0x7f;
  if (DAT_004309e4 != (code *)0x0) {
    uVar1 = (*DAT_004309e4)(uVar1,in_stack_00000000);
  }
  uVar2 = CONCAT31((int3)(uVar1 - 1 >> 8),*(undefined1 *)(uVar1 + 0x41aa74));
  if ((int)(uVar1 - 1) < 0) {
    iVar3 = FUN_00401202();
    uVar2 = *(undefined4 *)(iVar3 + 0xc);
  }
  FUN_0041b834(uVar2);
  return;
}



void FUN_0041aab0(undefined4 param_1)

{
  int iVar1;
  
  iVar1 = FUN_00401202();
  *(undefined4 *)(iVar1 + 0xc) = param_1;
  return;
}



undefined4 __stdcall FUN_0041aacc(void)

{
  undefined4 uVar1;
  int iVar2;
  
  iVar2 = FUN_00401202();
  uVar1 = *(undefined4 *)(iVar2 + 0xc);
  *(undefined4 *)(iVar2 + 0xc) = 0;
  return uVar1;
}



void FUN_0041aae4(undefined4 *param_1,undefined4 *param_2,uint param_3)

{
  int iVar1;
  uint uVar2;
  undefined4 *puVar3;
  undefined1 *puVar4;
  undefined4 *puVar5;
  undefined1 *puVar6;
  
  iVar1 = (int)param_3 >> 2;
  if (param_1 < param_2) {
    puVar3 = (undefined4 *)((int)param_1 + (param_3 - 4));
    puVar5 = (undefined4 *)((int)param_2 + (param_3 - 4));
    if (-1 < iVar1) {
      for (; iVar1 != 0; iVar1 = iVar1 + -1) {
        *puVar5 = *puVar3;
        puVar3 = puVar3 + -1;
        puVar5 = puVar5 + -1;
      }
      puVar4 = (undefined1 *)((int)puVar3 + 3);
      puVar6 = (undefined1 *)((int)puVar5 + 3);
      for (uVar2 = param_3 & 3; uVar2 != 0; uVar2 = uVar2 - 1) {
        *puVar6 = *puVar4;
        puVar4 = puVar4 + -1;
        puVar6 = puVar6 + -1;
      }
    }
  }
  else if ((param_2 != param_1) && (-1 < iVar1)) {
    for (; iVar1 != 0; iVar1 = iVar1 + -1) {
      *param_2 = *param_1;
      param_1 = param_1 + 1;
      param_2 = param_2 + 1;
    }
    for (uVar2 = param_3 & 3; uVar2 != 0; uVar2 = uVar2 - 1) {
      *(undefined1 *)param_2 = *(undefined1 *)param_1;
      param_1 = (undefined4 *)((int)param_1 + 1);
      param_2 = (undefined4 *)((int)param_2 + 1);
    }
    return;
  }
  return;
}



void __stdcall FUN_0041ab24(void)

{
  if (DAT_004309fc != (code *)0x0) {
    (*DAT_004309fc)();
  }
  FUN_0041b834(0xd2);
  return;
}



int * FUN_0041ab40(int *param_1,int *param_2,uint param_3)

{
  uint uVar1;
  
  uVar1 = param_3 >> 2;
  do {
    if (uVar1 == 0) {
LAB_0041ab70:
      uVar1 = param_3 & 3;
      if (((uVar1 != 0) && ((char)*param_1 == (char)*param_2)) &&
         ((uVar1 == 1 ||
          ((*(char *)((int)param_1 + 1) == *(char *)((int)param_2 + 1) &&
           ((uVar1 == 2 || (*(char *)((int)param_1 + 2) == *(char *)((int)param_2 + 2))))))))) {
        return (int *)0x0;
      }
      return param_1;
    }
    if (*param_1 != *param_2) {
      return param_1;
    }
    if (uVar1 == 1) {
      param_1 = param_1 + 1;
      param_2 = param_2 + 1;
      goto LAB_0041ab70;
    }
    if (param_1[1] != param_2[1]) {
      return param_1;
    }
    param_1 = param_1 + 2;
    param_2 = param_2 + 2;
    uVar1 = uVar1 - 2;
  } while( true );
}



void FUN_0041abb0(undefined4 *param_1,uint param_2,undefined1 param_3)

{
  int iVar1;
  uint uVar2;
  
  iVar1 = (int)param_2 >> 2;
  if (-1 < iVar1) {
    for (; iVar1 != 0; iVar1 = iVar1 + -1) {
      *param_1 = CONCAT22(CONCAT11(param_3,param_3),CONCAT11(param_3,param_3));
      param_1 = param_1 + 1;
    }
    for (uVar2 = param_2 & 3; uVar2 != 0; uVar2 = uVar2 - 1) {
      *(undefined1 *)param_1 = param_3;
      param_1 = (undefined4 *)((int)param_1 + 1);
    }
  }
  return;
}



void FUN_0041abd0(byte *param_1,int *param_2)

{
  bool bVar1;
  uint uVar2;
  byte bVar3;
  byte bVar4;
  byte *pbVar5;
  byte *pbVar6;
  int iVar7;
  
  pbVar6 = (byte *)0x0;
  if (param_1 == (byte *)0x0) {
LAB_0041ac4d:
    pbVar6 = pbVar6 + 1;
  }
  else {
    uVar2 = 0;
    pbVar6 = param_1;
    do {
      pbVar5 = pbVar6;
      bVar3 = *pbVar5;
      pbVar6 = pbVar5 + 1;
    } while (bVar3 == 0x20);
    bVar1 = false;
    if (bVar3 != 0x2d) {
      if (bVar3 == 0x2b) goto LAB_0041ac5d;
      if (((bVar3 != 0x24) && (bVar3 != 0x78)) && (bVar3 != 0x58)) {
        if (bVar3 == 0x30) {
          bVar3 = *pbVar6;
          pbVar6 = pbVar5 + 2;
          if ((bVar3 != 0x78) && (bVar3 != 0x58)) goto joined_r0x0041ac1a;
          goto LAB_0041ac62;
        }
        goto LAB_0041ac1e;
      }
LAB_0041ac62:
      bVar3 = *pbVar6;
      pbVar6 = pbVar6 + 1;
      if (bVar3 == 0) goto LAB_0041ac4d;
      do {
        if (0x60 < bVar3) {
          bVar3 = bVar3 - 0x20;
        }
        bVar4 = bVar3 - 0x30;
        if (9 < bVar4) {
          if (5 < (byte)(bVar3 + 0xbf)) goto LAB_0041ac56;
          bVar4 = bVar3 - 0x37;
        }
        if (0xfffffff < uVar2) goto LAB_0041ac56;
        uVar2 = uVar2 * 0x10 + (uint)bVar4;
        bVar3 = *pbVar6;
        pbVar6 = pbVar6 + 1;
      } while (bVar3 != 0);
      goto LAB_0041ac44;
    }
    bVar1 = true;
LAB_0041ac5d:
    bVar3 = *pbVar6;
    pbVar6 = pbVar5 + 2;
LAB_0041ac1e:
    if (bVar3 != 0) {
      do {
        if ((9 < (byte)(bVar3 - 0x30)) || (0xccccccc < uVar2)) goto LAB_0041ac56;
        uVar2 = uVar2 * 10 + (uint)(byte)(bVar3 - 0x30);
        bVar3 = *pbVar6;
        pbVar6 = pbVar6 + 1;
joined_r0x0041ac1a:
      } while (bVar3 != 0);
      if (bVar1) {
        if ((-uVar2 == 0 || 0 < (int)uVar2) || ((int)-uVar2 < 0)) goto LAB_0041ac44;
      }
      else if (-1 < (int)uVar2) {
LAB_0041ac44:
        iVar7 = 0;
        goto LAB_0041ac47;
      }
    }
  }
LAB_0041ac56:
  iVar7 = (int)pbVar6 - (int)param_1;
LAB_0041ac47:
  *param_2 = iVar7;
  return;
}



undefined4 __stdcall FUN_0041ac9c(void)

{
  int iVar1;
  uint uVar2;
  undefined4 uVar3;
  
  uVar3 = 0;
  iVar1 = GetKeyboardType(0);
  if (iVar1 == 7) {
    uVar2 = GetKeyboardType(1);
    if (((uVar2 & 0xff00) == 0xd00) || ((uVar2 & 0xff00) == 0x400)) {
      uVar3 = 1;
    }
  }
  return uVar3;
}



void __stdcall FUN_0041accc(void)

{
  LSTATUS LVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 uVar2;
  DWORD local_10;
  uint local_c;
  HKEY local_8;
  
  local_c = (uint)DAT_0042d6e4;
  LVar1 = RegOpenKeyExA((HKEY)0x80000002,"SOFTWARE\\Borland\\Delphi\\RTL",0,1,&local_8);
  if (LVar1 == 0) {
    uVar2 = *in_FS_OFFSET;
    *in_FS_OFFSET = &stack0xffffffe4;
    local_10 = 4;
    RegQueryValueExA(local_8,"FPUMaskValue",(LPDWORD)0x0,(LPDWORD)0x0,(LPBYTE)&local_c,&local_10);
    *in_FS_OFFSET = uVar2;
    RegCloseKey(local_8);
    return;
  }
  DAT_0042d6e4 = DAT_0042d6e4 & 0xffc0 | (ushort)local_c & 0x3f;
  return;
}



void __stdcall FUN_0041ad90(void)

{
  return;
}



undefined4 FUN_0041ad9c(undefined4 *param_1)

{
  return *param_1;
}



void FUN_0041ada0(int param_1,byte *param_2)

{
  int iVar1;
  byte *pbVar2;
  
  pbVar2 = *(byte **)(param_1 + -0x2c);
  for (iVar1 = **(byte **)(param_1 + -0x2c) + 1; iVar1 != 0; iVar1 = iVar1 + -1) {
    *param_2 = *pbVar2;
    pbVar2 = pbVar2 + 1;
    param_2 = param_2 + 1;
  }
  return;
}



undefined1 FUN_0041adb4(int param_1,int param_2)

{
  uint uVar1;
  short sVar2;
  
  sVar2 = 0;
  if (param_2 != 0) {
    uVar1 = (uint)**(byte **)(param_1 + -0x2c);
    if (uVar1 == *(uint *)(param_2 + -4)) {
      do {
        sVar2 = (((*(byte **)(param_1 + -0x2c))[uVar1] ^ *(byte *)(param_2 + -1 + uVar1)) & 0xdf) *
                0x100;
        if ((char)((ushort)sVar2 >> 8) != '\0') goto LAB_0041add7;
        uVar1 = uVar1 - 1;
      } while (uVar1 != 0);
      sVar2 = sVar2 + 1;
    }
  }
LAB_0041add7:
  return (char)sVar2;
}



undefined4 FUN_0041addc(int param_1)

{
  undefined4 uVar1;
  
  uVar1 = 0;
  if (*(undefined4 **)(param_1 + -0x24) != (undefined4 *)0x0) {
    uVar1 = **(undefined4 **)(param_1 + -0x24);
  }
  return uVar1;
}



void FUN_0041ade8(int param_1)

{
  int *piVar1;
  
  piVar1 = (int *)FUN_0041a9c0(*(int *)(param_1 + -0x28));
  FUN_0041ae60(param_1,piVar1);
  return;
}



void FUN_0041adfc(int *param_1)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = param_1;
  do {
    iVar1 = *(int *)(*piVar2 + -0x40);
    piVar2 = *(int **)(*piVar2 + -0x24);
    if (iVar1 != 0) {
      FUN_0041c0b0((int)param_1,iVar1);
    }
  } while (piVar2 != (int *)0x0);
  FUN_0041a9d8((int)param_1);
  return;
}



void FUN_0041ae24(int *param_1,char param_2,undefined4 param_3)

{
  char extraout_DL;
  char cVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 in_stack_00000000;
  undefined4 in_stack_fffffff0;
  undefined4 in_stack_fffffff4;
  
  cVar1 = '\0';
  if (param_2 != '\0') {
    param_1 = (int *)FUN_0041b0d0((int)param_1,param_2,param_3,in_stack_fffffff0,in_stack_fffffff4);
    cVar1 = extraout_DL;
  }
  if (cVar1 != '\0') {
    FUN_0041b128(param_1);
    *in_FS_OFFSET = in_stack_00000000;
  }
  return;
}



void FUN_0041ae44(int *param_1,char param_2)

{
  int *piVar1;
  char extraout_DL;
  
  piVar1 = FUN_0041b130(param_1,param_2);
  if ('\0' < extraout_DL) {
    FUN_0041b120(piVar1);
  }
  return;
}



void FUN_0041ae54(int *param_1)

{
  if (param_1 != (int *)0x0) {
    (**(code **)(*param_1 + -4))(param_1,1);
  }
  return;
}



// WARNING: Removing unreachable block (ram,0x0041ae95)
// WARNING: Removing unreachable block (ram,0x0041ae9b)
// WARNING: Removing unreachable block (ram,0x0041aea2)
// WARNING: Removing unreachable block (ram,0x0041aea8)
// WARNING: Removing unreachable block (ram,0x0041aeae)

void FUN_0041ae60(int param_1,int *param_2)

{
  uint uVar1;
  uint uVar2;
  
  *param_2 = param_1;
  uVar2 = *(uint *)(param_1 + -0x28);
  uVar1 = uVar2 >> 2;
  while( true ) {
    param_2 = param_2 + 1;
    uVar1 = uVar1 - 1;
    if (uVar1 == 0) break;
    *param_2 = 0;
  }
  for (uVar2 = uVar2 & 3; uVar2 != 0; uVar2 = uVar2 - 1) {
    *(undefined1 *)param_2 = 0;
    param_2 = (int *)((int)param_2 + 1);
  }
  for (; *(int **)(param_1 + -0x24) != (int *)0x0; param_1 = **(int **)(param_1 + -0x24)) {
  }
  return;
}



void FUN_0041aeb8(int *param_1,undefined *UNRECOVERED_JUMPTABLE,int *param_3)

{
  if ((undefined *)0xfeffffff < UNRECOVERED_JUMPTABLE) {
    FUN_0041d334(param_3,*(int **)(((uint)UNRECOVERED_JUMPTABLE & 0xffffff) + (int)param_1));
    return;
  }
  if ((undefined *)0xfdffffff < UNRECOVERED_JUMPTABLE) {
                    // WARNING: Could not recover jumptable at 0x0041aecf. Too many branches
                    // WARNING: Treating indirect jump as call
    (**(code **)((int)(short)UNRECOVERED_JUMPTABLE + *param_1))();
    return;
  }
                    // WARNING: Could not recover jumptable at 0x0041aed1. Too many branches
                    // WARNING: Treating indirect jump as call
  (*(code *)UNRECOVERED_JUMPTABLE)();
  return;
}



void FUN_0041aee8(int *param_1,int *param_2,int *param_3)

{
  int *piVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_24;
  undefined1 *puStack_20;
  undefined1 *puStack_1c;
  int *local_c;
  int *local_8;
  
  puStack_1c = &stack0xfffffffc;
  local_c = (int *)0x0;
  puStack_20 = &LAB_0041af76;
  uStack_24 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_24;
  local_8 = param_2;
  piVar1 = FUN_0041af88(*param_1,param_2);
  if (piVar1 == (int *)0x0) {
    *param_3 = 0;
  }
  else {
    if (piVar1[5] == 0) {
      FUN_0041aeb8(param_1,(undefined *)piVar1[6],(int *)&local_c);
      FUN_0041d334(param_3,local_c);
    }
    else {
      *param_3 = (int)param_1 + piVar1[5];
    }
    if ((*param_3 != 0) && (piVar1[5] != 0)) {
      (**(code **)(*(int *)*param_3 + 4))();
    }
  }
  *in_FS_OFFSET = uStack_24;
  puStack_1c = &LAB_0041af7d;
  puStack_20 = (undefined1 *)0x41af75;
  FUN_0041d31c((int *)&local_c);
  return;
}



int * FUN_0041af88(int param_1,int *param_2)

{
  int *piVar1;
  int iVar2;
  
  do {
    piVar1 = *(int **)(param_1 + -0x48);
    if (piVar1 != (int *)0x0) {
      iVar2 = *piVar1;
      piVar1 = piVar1 + 1;
      do {
        if ((((*param_2 == *piVar1) && (param_2[1] == piVar1[1])) && (param_2[2] == piVar1[2])) &&
           (param_2[3] == piVar1[3])) {
          return piVar1;
        }
        piVar1 = piVar1 + 7;
        iVar2 = iVar2 + -1;
      } while (iVar2 != 0);
    }
    if (*(int **)(param_1 + -0x24) == (int *)0x0) {
      return (int *)0x0;
    }
    param_1 = **(int **)(param_1 + -0x24);
  } while( true );
}



undefined4 FUN_0041afcc(int *param_1,int param_2)

{
  int iVar1;
  undefined4 uVar2;
  
  uVar2 = 0;
  if (param_1 != (int *)0x0) {
    while (iVar1 = *param_1, iVar1 != param_2) {
      param_1 = *(int **)(iVar1 + -0x24);
      if (param_1 == (int *)0x0) {
        return 0;
      }
    }
    uVar2 = CONCAT31((int3)((uint)iVar1 >> 8),1);
  }
  return uVar2;
}



void FUN_0041afe4(int param_1)

{
  uint uVar1;
  ushort unaff_BX;
  ushort *puVar2;
  bool bVar3;
  
  do {
    puVar2 = *(ushort **)(param_1 + -0x30);
    if (puVar2 != (ushort *)0x0) {
      uVar1 = (uint)*puVar2;
      bVar3 = puVar2 + 1 == (ushort *)0x0;
      puVar2 = puVar2 + 1;
      do {
        if (uVar1 == 0) break;
        uVar1 = uVar1 - 1;
        bVar3 = unaff_BX == *puVar2;
        puVar2 = puVar2 + 1;
      } while (!bVar3);
      if (bVar3) {
        return;
      }
    }
    if (*(int **)(param_1 + -0x24) == (int *)0x0) {
      return;
    }
    param_1 = **(int **)(param_1 + -0x24);
  } while( true );
}



void FUN_0041b014(int *param_1,undefined4 param_2,undefined4 param_3)

{
  code *UNRECOVERED_JUMPTABLE;
  undefined1 in_ZF;
  
  FUN_0041afe4(*param_1);
  if (!(bool)in_ZF) {
                    // WARNING: Could not recover jumptable at 0x0041b021. Too many branches
                    // WARNING: Treating indirect jump as call
    (*UNRECOVERED_JUMPTABLE)(param_1);
    return;
  }
  FUN_0041ab24();
  return;
}



undefined4 thunk_FUN_0041b030(int param_1,int param_2)

{
  while( true ) {
    if (param_1 == param_2) {
      return CONCAT31((int3)((uint)param_1 >> 8),1);
    }
    if (*(int **)(param_1 + -0x24) == (int *)0x0) break;
    param_1 = **(int **)(param_1 + -0x24);
  }
  return 0;
}



undefined4 FUN_0041b030(int param_1,int param_2)

{
  while( true ) {
    if (param_1 == param_2) {
      return CONCAT31((int3)((uint)param_1 >> 8),1);
    }
    if (*(int **)(param_1 + -0x24) == (int *)0x0) break;
    param_1 = **(int **)(param_1 + -0x24);
  }
  return 0;
}



undefined4 FUN_0041b040(int param_1)

{
  return *(undefined4 *)(param_1 + -0x3c);
}



undefined4 __stdcall FUN_0041b044(void)

{
  return 0x8000ffff;
}



int FUN_0041b080(int *param_1,char *param_2)

{
  undefined2 *puVar1;
  int *piVar2;
  int iVar3;
  char cVar4;
  int *piVar5;
  int iVar6;
  
  iVar3 = 0;
  iVar6 = 0;
  cVar4 = *param_2;
  piVar2 = param_1;
  do {
    puVar1 = *(undefined2 **)(*piVar2 + -0x38);
    if (puVar1 != (undefined2 *)0x0) {
      iVar6 = CONCAT22((short)((uint)iVar6 >> 0x10),*puVar1);
      piVar5 = (int *)(puVar1 + 3);
      do {
        iVar3 = CONCAT31((int3)((uint)iVar3 >> 8),*(char *)((int)piVar5 + 6));
        if (*(char *)((int)piVar5 + 6) == cVar4) {
          while (((*(byte *)((int)piVar5 + iVar3 + 6) ^ param_2[iVar3]) & 0xdf) == 0) {
            iVar3 = iVar3 + -1;
            if (iVar3 == 0) {
              return *piVar5 + (int)param_1;
            }
          }
          cVar4 = *param_2;
          iVar3 = CONCAT31((int3)((uint)iVar3 >> 8),*(undefined1 *)((int)piVar5 + 6));
        }
        piVar5 = (int *)((int)piVar5 + iVar3 + 7);
        iVar6 = iVar6 + -1;
      } while (iVar6 != 0);
    }
    piVar2 = *(int **)(*piVar2 + -0x24);
  } while (piVar2 != (int *)0x0);
  return 0;
}



void FUN_0041b0d0(int param_1,char param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5)

{
  int *in_FS_OFFSET;
  
  if (-1 < param_2) {
    (**(code **)(param_1 + -0xc))();
  }
  *in_FS_OFFSET = (int)&param_4;
  return;
}



void FUN_0041b120(int *param_1)

{
  (**(code **)(*param_1 + -8))();
  return;
}



int * FUN_0041b128(int *param_1)

{
  (**(code **)(*param_1 + -0x1c))();
  return param_1;
}



int * FUN_0041b130(int *param_1,char param_2)

{
  if (param_2 < '\x01') {
    return param_1;
  }
  (**(code **)(*param_1 + -0x18))();
  return param_1;
}



void __stdcall FUN_0041b140(void)

{
  if (1 < DAT_0042d6e8) {
    RaiseException(0xeedfadf,0,0,(ULONG_PTR *)0x0);
  }
  return;
}



undefined4 FUN_0041b15c(undefined4 param_1,ULONG_PTR param_2)

{
  ULONG_PTR UStack_c;
  undefined4 uStack_8;
  undefined4 uStack_4;
  
  if (DAT_0042d6e8 != '\0') {
    UStack_c = param_2;
    uStack_8 = param_1;
    uStack_4 = param_1;
    RaiseException(0xeedfae4,0,2,&UStack_c);
    param_1 = uStack_4;
  }
  return param_1;
}



undefined4 __cdecl FUN_0041b17c(undefined4 param_1)

{
  RaiseException(0xeedfae0,0,1,(ULONG_PTR *)&stack0x00000000);
  return param_1;
}



void FUN_0041b190(undefined4 param_1)

{
  if (1 < DAT_0042d6e8) {
    FUN_0041b17c(param_1);
    return;
  }
  return;
}



void FUN_0041b1a4(undefined4 param_1)

{
  if (1 < DAT_0042d6e8) {
    FUN_0041b17c(param_1);
    return;
  }
  return;
}



int FUN_0041b1b8(int param_1,undefined4 param_2,char *param_3)

{
  if (((param_3 != (char *)0x0) && (param_1 = *(int *)(param_3 + 1), *param_3 != -0x17)) &&
     (*param_3 == -0x15)) {
    param_1 = (int)(char)param_1;
  }
  return param_1;
}



int FUN_0041b1d8(int param_1,undefined4 param_2,char *param_3)

{
  ULONG_PTR UStack_10;
  char *pcStack_c;
  undefined4 uStack_8;
  int iStack_4;
  
  if (1 < DAT_0042d6e8) {
    UStack_10 = 0x41b1e9;
    pcStack_c = param_3;
    uStack_8 = param_2;
    iStack_4 = param_1;
    FUN_0041b1b8(param_1,param_2,param_3);
    RaiseException(0xeedfae1,0,1,&UStack_10);
    param_1 = iStack_4;
  }
  return param_1;
}



undefined4 __cdecl FUN_0041b4d0(int param_1,int param_2)

{
  int iVar1;
  code *extraout_ECX;
  
  if ((*(uint *)(param_1 + 4) & 6) != 0) {
    iVar1 = *(int *)(param_2 + 4);
    *(undefined4 *)(param_2 + 4) = 0x41b500;
    FUN_0041b1d8(param_1,param_2,(char *)(iVar1 + 5));
    (*extraout_ECX)();
  }
  return 1;
}



void FUN_0041b508(undefined4 param_1)

{
  ULONG_PTR UStack_18;
  undefined4 uStack_14;
  
  uStack_14 = param_1;
  RaiseException(0xeedfade,1,7,&UStack_18);
  return;
}



undefined4 __stdcall FUN_0041b524(void)

{
  uint *puVar1;
  undefined4 *puVar2;
  int *piVar3;
  int iVar4;
  undefined4 *in_FS_OFFSET;
  undefined4 *in_stack_00000014;
  int in_stack_00000030;
  
  *(undefined4 *)(in_stack_00000030 + 4) = 0x41b56f;
  iVar4 = FUN_00401202();
  puVar2 = *(undefined4 **)(iVar4 + 8);
  *(undefined4 *)(iVar4 + 8) = *puVar2;
  piVar3 = (int *)puVar2[3];
  puVar1 = (uint *)(piVar3 + 1);
  *puVar1 = *puVar1 & 0xfffffffd;
  if (*piVar3 != 0xeedfade) {
    FUN_0041ae54((int *)puVar2[2]);
    FUN_0041b140();
  }
  *in_stack_00000014 = *(undefined4 *)*in_FS_OFFSET;
  return 1;
}



void __stdcall FUN_0041b5a8(undefined4 param_1,int param_2)

{
  undefined4 *in_FS_OFFSET;
  
  *in_FS_OFFSET = param_1;
  (*(code *)(param_2 + 5))();
  return;
}



void __stdcall FUN_0041b5c0(void)

{
  int *piVar1;
  int *in_FS_OFFSET;
  
  piVar1 = (int *)*in_FS_OFFSET;
  if (DAT_00430e8c == piVar1) {
    *in_FS_OFFSET = *DAT_00430e8c;
    return;
  }
  while( true ) {
    if (piVar1 == (int *)0xffffffff) {
      return;
    }
    if ((int *)*piVar1 == DAT_00430e8c) break;
    piVar1 = (int *)*piVar1;
  }
  *piVar1 = *DAT_00430e8c;
  return;
}



void __stdcall FUN_0041b5e8(void)

{
  int iVar1;
  code *pcVar2;
  int iVar3;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_1c;
  undefined1 *puStack_18;
  undefined1 *puStack_14;
  
  iVar3 = DAT_00430e94;
  puStack_14 = &stack0xfffffffc;
  if (DAT_00430e90 != 0) {
    iVar1 = *(int *)(DAT_00430e90 + 4);
    puStack_18 = &LAB_0041b62e;
    uStack_1c = *in_FS_OFFSET;
    *in_FS_OFFSET = &uStack_1c;
    while (0 < iVar3) {
      iVar3 = iVar3 + -1;
      pcVar2 = *(code **)(iVar1 + 4 + iVar3 * 8);
      DAT_00430e94 = iVar3;
      if (pcVar2 != (code *)0x0) {
        (*pcVar2)();
      }
    }
    *in_FS_OFFSET = uStack_1c;
  }
  return;
}



void FUN_0041b648(int *param_1)

{
  int iVar1;
  int *piVar2;
  
  iVar1 = *param_1;
  piVar2 = param_1 + 1;
  do {
    FUN_0041d2c4(*(undefined4 **)piVar2[1],(int *)*piVar2);
    piVar2 = piVar2 + 2;
    iVar1 = iVar1 + -1;
  } while (iVar1 != 0);
  return;
}



void FUN_0041b664(int *param_1)

{
  int iVar1;
  int *piVar2;
  
  iVar1 = *param_1;
  piVar2 = param_1 + 1;
  do {
    *(int *)*piVar2 = *(int *)piVar2[1] + piVar2[2];
    piVar2 = piVar2 + 3;
    iVar1 = iVar1 + -1;
  } while (iVar1 != 0);
  return;
}



void __stdcall FUN_0041b684(void)

{
  ulonglong uVar1;
  uint uVar2;
  PVOID pvVar3;
  char *pcVar4;
  
  pcVar4 = s_Runtime_error_at_00000000_0042d710 + 0x10;
  uVar2 = DAT_00430a1c;
  do {
    uVar1 = (ulonglong)uVar2;
    uVar2 = uVar2 / 10;
    *pcVar4 = (char)(uVar1 % 10) + '0';
    pcVar4 = pcVar4 + -1;
  } while (uVar2 != 0);
  pvVar3 = FUN_0041d11c(DAT_00430a24);
  uVar2 = (int)DAT_00430a24 - (int)pvVar3;
  pcVar4 = s_Runtime_error_at_00000000_0042d710 + 0x1c;
  do {
    *pcVar4 = s_0123456789ABCDEF_0042d738[uVar2 & 0xf];
    pcVar4 = pcVar4 + -1;
    uVar2 = uVar2 >> 4;
  } while (uVar2 != 0);
  return;
}



bool __stdcall FUN_0041b6d4(void)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  
  puVar3 = DAT_00430e88;
  puVar4 = &DAT_00430e88;
  for (iVar2 = 0xb; iVar1 = DAT_00430a1c, iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar4 = *puVar3;
    puVar3 = puVar3 + 1;
    puVar4 = puVar4 + 1;
  }
  LOCK();
  DAT_00430a1c = 0;
  UNLOCK();
  return (bool)('\x01' - (iVar1 != 0));
}



void __stdcall FUN_0041b708(void)

{
  code *pcVar1;
  undefined *puVar2;
  undefined1 *extraout_ECX;
  int iVar3;
  undefined4 extraout_EDX;
  undefined4 *puVar4;
  undefined4 *puVar5;
  byte bVar6;
  
  bVar6 = 0;
  pcVar1 = DAT_00430a20;
  if (DAT_00430eac == 0) {
    while (DAT_00430a20 = pcVar1, pcVar1 != (code *)0x0) {
      DAT_00430a20 = (code *)0x0;
      (*pcVar1)();
      pcVar1 = DAT_00430a20;
    }
  }
  if (DAT_00430a24 != 0) {
    FUN_0041b684();
    if (DAT_00430a2c == '\0') {
      if (DAT_0042d6f0 == '\0') {
        MessageBoxA((HWND)0x0,s_Runtime_error_at_00000000_0042d710,s_Error_0042d730,0);
      }
    }
    else {
      puVar2 = (undefined *)FUN_00418c90(&DAT_00430c00,s_Runtime_error_at_00000000_0042d710);
      FUN_00418c13(puVar2);
    }
    DAT_00430a24 = 0;
  }
  while( true ) {
    if ((DAT_00430eac == 2) && (DAT_00430a1c == 0)) {
      DAT_00430e94 = 0;
    }
    FUN_0041b5e8();
    if (((DAT_00430eac < 2) || (DAT_00430a1c != 0)) && (DAT_00430e98 != (undefined4 *)0x0)) {
      FUN_0041d254(DAT_00430e98,extraout_EDX,extraout_ECX);
      if ((HMODULE)DAT_00430e98[4] != (HMODULE)DAT_00430e98[1]) {
        FreeLibrary((HMODULE)DAT_00430e98[4]);
      }
    }
    FUN_0041b5c0();
    if (DAT_00430eac == 1) {
      (*DAT_00430eb0)();
    }
    if (DAT_00430eac != 0) {
      FUN_0041b6d4();
    }
    if (DAT_00430e88 == (undefined4 *)0x0) break;
    puVar4 = DAT_00430e88;
    puVar5 = &DAT_00430e88;
    for (iVar3 = 0xb; iVar3 != 0; iVar3 = iVar3 + -1) {
      *puVar5 = *puVar4;
      puVar4 = puVar4 + (uint)bVar6 * -2 + 1;
      puVar5 = puVar5 + (uint)bVar6 * -2 + 1;
    }
  }
                    // WARNING: Subroutine does not return
  ExitProcess(DAT_00430a1c);
}



void FUN_0041b828(undefined4 param_1)

{
  DAT_00430a1c = param_1;
  FUN_0041b708();
  return;
}



void FUN_0041b834(undefined4 param_1)

{
  undefined4 in_stack_00000000;
  
  DAT_00430a24 = in_stack_00000000;
  FUN_0041b828(param_1);
  return;
}



int * FUN_0041b840(int *param_1)

{
  int *piVar1;
  int iVar2;
  
  iVar2 = *param_1;
  if ((iVar2 != 0) && (*param_1 = 0, 0 < *(int *)(iVar2 + -8))) {
    LOCK();
    piVar1 = (int *)(iVar2 + -8);
    *piVar1 = *piVar1 + -1;
    UNLOCK();
    if (*piVar1 == 0) {
      FUN_0041a9d8(iVar2 + -8);
    }
  }
  return param_1;
}



void FUN_0041b864(int *param_1,int param_2)

{
  int *piVar1;
  int iVar2;
  
  do {
    iVar2 = *param_1;
    if ((iVar2 != 0) && (*param_1 = 0, 0 < *(int *)(iVar2 + -8))) {
      LOCK();
      piVar1 = (int *)(iVar2 + -8);
      *piVar1 = *piVar1 + -1;
      UNLOCK();
      if (*piVar1 == 0) {
        FUN_0041a9d8(iVar2 + -8);
      }
    }
    param_1 = param_1 + 1;
    param_2 = param_2 + -1;
  } while (param_2 != 0);
  return;
}



void FUN_0041b894(int *param_1,undefined4 *param_2)

{
  int *piVar1;
  int iVar2;
  undefined4 *puVar3;
  
  if (param_2 != (undefined4 *)0x0) {
    if ((uint)param_2[-2] < 0x80000000) {
      LOCK();
      param_2[-2] = param_2[-2] + 1;
      UNLOCK();
    }
    else {
      puVar3 = FUN_0041b904(param_2[-1]);
      FUN_0041aae4(param_2,puVar3,param_2[-1]);
      param_2 = puVar3;
    }
  }
  LOCK();
  iVar2 = *param_1;
  *param_1 = (int)param_2;
  UNLOCK();
  if ((iVar2 != 0) && (0 < *(int *)(iVar2 + -8))) {
    LOCK();
    piVar1 = (int *)(iVar2 + -8);
    *piVar1 = *piVar1 + -1;
    UNLOCK();
    if (*piVar1 == 0) {
      FUN_0041a9d8(iVar2 + -8);
    }
  }
  return;
}



void FUN_0041b8d8(int *param_1,int param_2)

{
  int *piVar1;
  int iVar2;
  
  if ((param_2 != 0) && (-1 < *(int *)(param_2 + -8))) {
    LOCK();
    *(int *)(param_2 + -8) = *(int *)(param_2 + -8) + 1;
    UNLOCK();
  }
  LOCK();
  iVar2 = *param_1;
  *param_1 = param_2;
  UNLOCK();
  if ((iVar2 != 0) && (0 < *(int *)(iVar2 + -8))) {
    LOCK();
    piVar1 = (int *)(iVar2 + -8);
    *piVar1 = *piVar1 + -1;
    UNLOCK();
    if (*piVar1 == 0) {
      FUN_0041a9d8(iVar2 + -8);
    }
  }
  return;
}



undefined4 * FUN_0041b904(int param_1)

{
  undefined4 *puVar1;
  
  if (0 < param_1) {
    puVar1 = (undefined4 *)FUN_0041a9c0(param_1 + 9);
    puVar1[1] = param_1;
    *puVar1 = 1;
    *(undefined1 *)((int)(puVar1 + 2) + param_1) = 0;
    return puVar1 + 2;
  }
  return (undefined4 *)0x0;
}



void FUN_0041b928(int *param_1,undefined4 *param_2,uint param_3)

{
  undefined4 *puVar1;
  
  puVar1 = FUN_0041b904(param_3);
  if (param_2 != (undefined4 *)0x0) {
    FUN_0041aae4(param_2,puVar1,param_3);
  }
  FUN_0041b840(param_1);
  *param_1 = (int)puVar1;
  return;
}



void FUN_0041b958(int *param_1,LPCWSTR param_2,int param_3)

{
  uint uVar1;
  CHAR local_810 [2048];
  
  if (param_3 < 1) {
    FUN_0041b840(param_1);
  }
  else {
    if (param_3 < 0x400) {
      uVar1 = WideCharToMultiByte(0,0,param_2,param_3,local_810,0x800,(LPCSTR)0x0,(LPBOOL)0x0);
      if (0 < (int)uVar1) {
        FUN_0041b928(param_1,(undefined4 *)local_810,uVar1);
        return;
      }
    }
    uVar1 = WideCharToMultiByte(0,0,param_2,param_3,(LPSTR)0x0,0,(LPCSTR)0x0,(LPBOOL)0x0);
    FUN_0041b928(param_1,(undefined4 *)0x0,uVar1);
    WideCharToMultiByte(0,0,param_2,param_3,(LPSTR)*param_1,uVar1,(LPCSTR)0x0,(LPBOOL)0x0);
  }
  return;
}



void FUN_0041b9e8(int *param_1,undefined4 param_2)

{
  undefined4 uStack_4;
  
  uStack_4 = param_2;
  FUN_0041b928(param_1,&uStack_4,1);
  return;
}



void FUN_0041b9f8(int *param_1,char *param_2)

{
  uint uVar1;
  char *pcVar2;
  
  uVar1 = 0;
  pcVar2 = param_2;
  if (param_2 != (char *)0x0) {
    for (; *pcVar2 != '\0'; pcVar2 = pcVar2 + 4) {
      if (pcVar2[1] == '\0') {
LAB_0041ba19:
        pcVar2 = pcVar2 + 1;
        break;
      }
      if (pcVar2[2] == '\0') {
LAB_0041ba18:
        pcVar2 = pcVar2 + 1;
        goto LAB_0041ba19;
      }
      if (pcVar2[3] == '\0') {
        pcVar2 = pcVar2 + 1;
        goto LAB_0041ba18;
      }
    }
    uVar1 = (int)pcVar2 - (int)param_2;
  }
  FUN_0041b928(param_1,(undefined4 *)param_2,uVar1);
  return;
}



void FUN_0041ba64(int *param_1,byte *param_2)

{
  FUN_0041b928(param_1,(undefined4 *)(param_2 + 1),(uint)*param_2);
  return;
}



void FUN_0041ba70(int *param_1,char *param_2,uint param_3)

{
  uint uVar1;
  char *pcVar2;
  bool bVar3;
  
  bVar3 = true;
  uVar1 = param_3;
  pcVar2 = param_2;
  do {
    if (uVar1 == 0) break;
    uVar1 = uVar1 - 1;
    bVar3 = *pcVar2 == '\0';
    pcVar2 = pcVar2 + 1;
  } while (!bVar3);
  if (bVar3) {
    uVar1 = ~uVar1;
  }
  FUN_0041b928(param_1,(undefined4 *)param_2,uVar1 + param_3);
  return;
}



void FUN_0041ba88(int *param_1,LPCWSTR param_2)

{
  uint uVar1;
  
  uVar1 = 0;
  if (param_2 != (LPCWSTR)0x0) {
    uVar1 = (uint)param_2[-1] >> 1;
  }
  FUN_0041b958(param_1,param_2,uVar1);
  return;
}



void FUN_0041ba9c(undefined1 *param_1,undefined4 *param_2,uint param_3)

{
  uint uVar1;
  
  if ((param_2 != (undefined4 *)0x0) && (uVar1 = param_2[-1], uVar1 != 0)) {
    if ((int)uVar1 <= (int)param_3) {
      param_3 = uVar1;
    }
    *param_1 = (char)param_3;
    FUN_0041aae4(param_2,(undefined4 *)(param_1 + 1),param_3);
    return;
  }
  *param_1 = 0;
  return;
}



undefined4 FUN_0041bac0(int param_1)

{
  undefined4 uVar1;
  
  uVar1 = 0;
  if (param_1 != 0) {
    uVar1 = *(undefined4 *)(param_1 + -4);
  }
  return uVar1;
}



void FUN_0041bac8(int *param_1,undefined4 *param_2)

{
  int *piVar1;
  int iVar2;
  uint uVar3;
  undefined4 *puVar4;
  uint uVar5;
  
  if (param_2 == (undefined4 *)0x0) {
    return;
  }
  puVar4 = (undefined4 *)*param_1;
  if (puVar4 != (undefined4 *)0x0) {
    uVar3 = puVar4[-1];
    if (param_2 == puVar4) {
      FUN_0041bd88(param_1,param_2[-1] + uVar3);
      param_2 = (undefined4 *)*param_1;
      uVar5 = uVar3;
    }
    else {
      FUN_0041bd88(param_1,param_2[-1] + uVar3);
      uVar5 = param_2[-1];
    }
    FUN_0041aae4(param_2,(undefined4 *)(*param_1 + uVar3),uVar5);
    return;
  }
  if (param_2 != (undefined4 *)0x0) {
    if ((uint)param_2[-2] < 0x80000000) {
      LOCK();
      param_2[-2] = param_2[-2] + 1;
      UNLOCK();
    }
    else {
      puVar4 = FUN_0041b904(param_2[-1]);
      FUN_0041aae4(param_2,puVar4,param_2[-1]);
      param_2 = puVar4;
    }
  }
  LOCK();
  iVar2 = *param_1;
  *param_1 = (int)param_2;
  UNLOCK();
  if ((iVar2 != 0) && (0 < *(int *)(iVar2 + -8))) {
    LOCK();
    piVar1 = (int *)(iVar2 + -8);
    *piVar1 = *piVar1 + -1;
    UNLOCK();
    if (*piVar1 == 0) {
      FUN_0041a9d8(iVar2 + -8);
    }
  }
  return;
}



void FUN_0041bb0c(int *param_1,undefined4 *param_2,undefined4 *param_3)

{
  int *piVar1;
  int iVar2;
  undefined4 *puVar3;
  
  if (param_2 == (undefined4 *)0x0) {
    FUN_0041b894(param_1,param_3);
    return;
  }
  if (param_3 == (undefined4 *)0x0) {
    if (param_2 != (undefined4 *)0x0) {
      if ((uint)param_2[-2] < 0x80000000) {
        LOCK();
        param_2[-2] = param_2[-2] + 1;
        UNLOCK();
      }
      else {
        puVar3 = FUN_0041b904(param_2[-1]);
        FUN_0041aae4(param_2,puVar3,param_2[-1]);
        param_2 = puVar3;
      }
    }
    LOCK();
    iVar2 = *param_1;
    *param_1 = (int)param_2;
    UNLOCK();
    if ((iVar2 != 0) && (0 < *(int *)(iVar2 + -8))) {
      LOCK();
      piVar1 = (int *)(iVar2 + -8);
      *piVar1 = *piVar1 + -1;
      UNLOCK();
      if (*piVar1 == 0) {
        FUN_0041a9d8(iVar2 + -8);
      }
    }
    return;
  }
  if (param_2 == (undefined4 *)*param_1) {
    FUN_0041bac8(param_1,param_3);
    return;
  }
  if (param_3 != (undefined4 *)*param_1) {
    FUN_0041b894(param_1,param_2);
    FUN_0041bac8(param_1,param_3);
    return;
  }
  puVar3 = FUN_0041b904(param_2[-1] + param_3[-1]);
  FUN_0041aae4(param_2,puVar3,param_2[-1]);
  FUN_0041aae4(param_3,(undefined4 *)((int)puVar3 + param_2[-1]),param_3[-1]);
  if (puVar3 != (undefined4 *)0x0) {
    puVar3[-2] = puVar3[-2] + -1;
  }
  FUN_0041b894(param_1,puVar3);
  return;
}



void FUN_0041bb80(int *param_1,int param_2)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int iVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  code *UNRECOVERED_JUMPTABLE;
  
  iVar2 = 0;
  iVar4 = param_2;
  do {
    if (*(int *)(&stack0x00000000 + iVar4 * 4) != 0) {
      iVar2 = iVar2 + *(int *)(*(int *)(&stack0x00000000 + iVar4 * 4) + -4);
    }
    iVar4 = iVar4 + -1;
  } while (iVar4 != 0);
  puVar3 = FUN_0041b904(iVar2);
  puVar5 = puVar3;
  do {
    puVar1 = *(undefined4 **)(&stack0x00000000 + param_2 * 4);
    puVar6 = puVar5;
    if (puVar1 != (undefined4 *)0x0) {
      puVar6 = (undefined4 *)((int)puVar5 + puVar1[-1]);
      FUN_0041aae4(puVar1,puVar5,puVar1[-1]);
    }
    param_2 = param_2 + -1;
    puVar5 = puVar6;
  } while (param_2 != 0);
  if (puVar3 != (undefined4 *)0x0) {
    puVar3[-2] = puVar3[-2] + -1;
  }
  FUN_0041b894(param_1,puVar3);
                    // WARNING: Could not recover jumptable at 0x0041bbca. Too many branches
                    // WARNING: Treating indirect jump as call
  (*UNRECOVERED_JUMPTABLE)(UNRECOVERED_JUMPTABLE);
  return;
}



uint * FUN_0041bbd0(uint *param_1,uint *param_2)

{
  uint uVar1;
  uint *puVar2;
  uint uVar3;
  uint uVar4;
  
  if (param_1 != param_2) {
    if (param_1 == (uint *)0x0) {
      param_1 = (uint *)-param_2[-1];
    }
    else if (param_2 == (uint *)0x0) {
      param_1 = (uint *)param_1[-1];
    }
    else {
      uVar3 = param_2[-1];
      puVar2 = (uint *)(param_1[-1] - uVar3);
      if (param_1[-1] < uVar3 || puVar2 == (uint *)0x0) {
        uVar3 = uVar3 + (int)puVar2;
      }
      for (uVar4 = uVar3 >> 2; uVar4 != 0; uVar4 = uVar4 - 2) {
        if (*param_1 != *param_2) {
          return puVar2;
        }
        if (uVar4 == 1) {
          param_1 = param_1 + 1;
          param_2 = param_2 + 1;
          break;
        }
        if (param_1[1] != param_2[1]) {
          return puVar2;
        }
        param_1 = param_1 + 2;
        param_2 = param_2 + 2;
      }
      uVar3 = uVar3 & 3;
      if (uVar3 != 0) {
        uVar4 = *param_1;
        uVar1 = *param_2;
        if ((char)uVar4 != (char)uVar1) {
          return puVar2;
        }
        if (uVar3 != 1) {
          if ((char)(uVar4 >> 8) != (char)(uVar1 >> 8)) {
            return puVar2;
          }
          if ((uVar3 != 2) && ((uVar4 & 0xff0000) != (uVar1 & 0xff0000))) {
            return puVar2;
          }
        }
      }
      param_1 = (uint *)((int)puVar2 * 2);
    }
  }
  return param_1;
}



undefined * FUN_0041bc74(undefined *param_1)

{
  if (param_1 != (undefined *)0x0) {
    return param_1;
  }
  return &DAT_0041bc79;
}



int FUN_0041bc80(int *param_1)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int iVar3;
  
  iVar3 = *param_1;
  if ((iVar3 != 0) && (*(int *)(iVar3 + -8) != 1)) {
    puVar2 = FUN_0041b904(*(int *)(iVar3 + -4));
    puVar1 = (undefined4 *)*param_1;
    *param_1 = (int)puVar2;
    if (0 < (int)puVar1[-2]) {
      LOCK();
      puVar1[-2] = puVar1[-2] + -1;
      UNLOCK();
    }
    FUN_0041aae4(puVar1,puVar2,puVar1[-1]);
    iVar3 = *param_1;
  }
  return iVar3;
}



void FUN_0041bcb8(int param_1,int param_2,uint param_3,int *param_4)

{
  int iVar1;
  int iVar2;
  
  if ((param_1 != 0) && (iVar1 = *(int *)(param_1 + -4), iVar1 != 0)) {
    iVar2 = param_2 + -1;
    if (param_2 < 1) {
      iVar2 = 0;
    }
    else if (iVar1 <= iVar2) goto LAB_0041bcea;
    if (-1 < (int)param_3) {
      if (iVar1 - iVar2 < (int)param_3) {
        param_3 = iVar1 - iVar2;
      }
      FUN_0041b928(param_4,(undefined4 *)(iVar2 + param_1),param_3);
      return;
    }
  }
LAB_0041bcea:
  FUN_0041b840(param_4);
  return;
}



void FUN_0041bcf8(int *param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  FUN_0041bc80(param_1);
  iVar1 = *param_1;
  if (iVar1 != 0) {
    iVar3 = param_2 + -1;
    if (((0 < param_2) && (iVar3 < *(int *)(iVar1 + -4))) && (0 < param_3)) {
      iVar2 = *(int *)(iVar1 + -4) - iVar3;
      if (iVar2 < param_3) {
        param_3 = iVar2;
      }
      FUN_0041aae4((undefined4 *)(iVar1 + iVar3 + param_3),(undefined4 *)(iVar1 + iVar3),
                   iVar2 - param_3);
      FUN_0041bd88(param_1,*(int *)(*param_1 + -4) - param_3);
    }
  }
  return;
}



int FUN_0041bd40(char *param_1,char *param_2)

{
  char *pcVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  char *pcVar5;
  char *pcVar6;
  bool bVar7;
  
  if (param_1 != (char *)0x0) {
    if (param_2 == (char *)0x0) {
      return 0;
    }
    iVar4 = *(int *)(param_1 + -4) + -1;
    if (-1 < iVar4) {
      iVar2 = *(int *)(param_2 + -4) - iVar4;
      bVar7 = iVar2 == 0;
      pcVar5 = param_2;
      if (!bVar7 && iVar4 <= *(int *)(param_2 + -4)) {
LAB_0041bd60:
        do {
          if (iVar2 != 0) {
            iVar2 = iVar2 + -1;
            pcVar1 = pcVar5 + 1;
            bVar7 = *param_1 == *pcVar5;
            pcVar5 = pcVar1;
            if (!bVar7) goto LAB_0041bd60;
          }
          iVar3 = iVar4;
          pcVar6 = pcVar5;
          pcVar1 = param_1;
          if (!bVar7) {
            return 0;
          }
          do {
            if (iVar3 == 0) break;
            bVar7 = pcVar1[1] == *pcVar6;
            iVar3 = iVar3 + -1;
            pcVar6 = pcVar6 + 1;
            pcVar1 = pcVar1 + 1;
          } while (bVar7);
          if (bVar7) {
            return (int)pcVar5 - (int)param_2;
          }
        } while( true );
      }
    }
  }
  return 0;
}



void FUN_0041bd88(int *param_1,uint param_2)

{
  undefined4 *puVar1;
  uint uVar2;
  undefined4 *puVar3;
  int iStack_10;
  
  puVar3 = (undefined4 *)0x0;
  if (param_2 != 0) {
    iStack_10 = *param_1;
    if ((iStack_10 != 0) && (*(int *)(iStack_10 + -8) == 1)) {
      iStack_10 = iStack_10 + -8;
      FUN_0041a9f0(&iStack_10,param_2 + 9);
      *param_1 = iStack_10 + 8;
      *(uint *)(iStack_10 + 4) = param_2;
      *(undefined1 *)(iStack_10 + 8 + param_2) = 0;
      return;
    }
    iStack_10 = 0x41bdc5;
    puVar3 = FUN_0041b904(param_2);
    puVar1 = (undefined4 *)*param_1;
    if (puVar1 != (undefined4 *)0x0) {
      uVar2 = puVar1[-1];
      if ((int)param_2 <= (int)puVar1[-1]) {
        uVar2 = param_2;
      }
      iStack_10 = 0x41bddd;
      FUN_0041aae4(puVar1,puVar3,uVar2);
    }
  }
  iStack_10 = 0x41bde4;
  FUN_0041b840(param_1);
  *param_1 = (int)puVar3;
  return;
}



void FUN_0041bdf4(undefined4 *param_1,undefined4 param_2)

{
  BSTR bstrString;
  
  bstrString = (BSTR)*param_1;
  *param_1 = param_2;
  if (bstrString != (BSTR)0x0) {
    SysFreeString(bstrString);
  }
  return;
}



undefined4 * FUN_0041be04(undefined4 *param_1)

{
  BSTR bstrString;
  
  bstrString = (BSTR)*param_1;
  if (bstrString != (BSTR)0x0) {
    *param_1 = 0;
    SysFreeString(bstrString);
  }
  return param_1;
}



void FUN_0041be1c(undefined4 *param_1,int param_2)

{
  BSTR bstrString;
  
  do {
    bstrString = (BSTR)*param_1;
    if (bstrString != (BSTR)0x0) {
      *param_1 = 0;
      SysFreeString(bstrString);
    }
    param_1 = param_1 + 1;
    param_2 = param_2 + -1;
  } while (param_2 != 0);
  return;
}



BSTR * FUN_0041be40(BSTR *param_1,OLECHAR *param_2)

{
  BSTR bstrString;
  BSTR *ppOVar1;
  
  if ((param_2 != (OLECHAR *)0x0) && ((uint)param_2[-1] >> 1 != 0)) {
    ppOVar1 = (BSTR *)SysReAllocStringLen(param_1,param_2,(uint)param_2[-1] >> 1);
    if (ppOVar1 != (BSTR *)0x0) {
      return ppOVar1;
    }
    ppOVar1 = (BSTR *)FUN_0041aa4c(1);
    return ppOVar1;
  }
  bstrString = *param_1;
  if (bstrString != (BSTR)0x0) {
    *param_1 = (BSTR)0x0;
    SysFreeString(bstrString);
  }
  return param_1;
}



void FUN_0041be64(BSTR param_1,LPCSTR param_2,int param_3)

{
  UINT UVar1;
  WCHAR local_810 [512];
  
  if (param_3 < 1) {
    FUN_0041be04(param_1);
  }
  else {
    if (param_3 < 0x400) {
      UVar1 = MultiByteToWideChar(0,0,param_2,param_3,local_810,0x400);
      if (0 < (int)UVar1) {
        FUN_0041bee8(param_1,local_810,UVar1);
        return;
      }
    }
    UVar1 = MultiByteToWideChar(0,0,param_2,param_3,(LPWSTR)0x0,0);
    FUN_0041bee8(param_1,(OLECHAR *)0x0,UVar1);
    MultiByteToWideChar(0,0,param_2,param_3,(LPWSTR)*param_1,UVar1);
  }
  return;
}



BSTR FUN_0041bee8(BSTR param_1,OLECHAR *param_2,UINT param_3)

{
  BSTR bstrString;
  BSTR pOVar1;
  
  if (param_3 == 0) {
    pOVar1 = (BSTR)*param_1;
    if (pOVar1 != (BSTR)0x0) {
      *param_1 = L'\0';
      SysFreeString(pOVar1);
    }
    return param_1;
  }
  pOVar1 = SysAllocStringLen(param_2,param_3);
  if (pOVar1 != (BSTR)0x0) {
    bstrString = (BSTR)*param_1;
    *param_1 = (OLECHAR)pOVar1;
    SysFreeString(bstrString);
    return pOVar1;
  }
  pOVar1 = (BSTR)FUN_0041aa4c(1);
  return pOVar1;
}



void FUN_0041bf0c(BSTR param_1,LPCSTR param_2)

{
  int iVar1;
  
  iVar1 = 0;
  if (param_2 != (LPCSTR)0x0) {
    iVar1 = *(int *)(param_2 + -4);
  }
  FUN_0041be64(param_1,param_2,iVar1);
  return;
}



uint FUN_0041bf1c(int param_1)

{
  uint uVar1;
  
  uVar1 = 0;
  if (param_1 != 0) {
    uVar1 = *(uint *)(param_1 + -4) >> 1;
  }
  return uVar1;
}



void FUN_0041bf28(int *param_1,undefined4 *param_2)

{
  uint uVar1;
  uint uVar2;
  undefined4 *puVar3;
  
  uVar1 = FUN_0041bf1c((int)param_2);
  if (uVar1 != 0) {
    uVar2 = FUN_0041bf1c(*param_1);
    puVar3 = (undefined4 *)FUN_0041bf88(uVar1 + uVar2);
    if (0 < (int)uVar2) {
      FUN_0041aae4((undefined4 *)*param_1,puVar3,uVar2 * 2);
    }
    FUN_0041aae4(param_2,(undefined4 *)((int)puVar3 + uVar2 * 2),uVar1 * 2);
    FUN_0041bdf4(param_1,puVar3);
  }
  return;
}



void FUN_0041bf88(UINT param_1)

{
  BSTR pOVar1;
  
  if (param_1 != 0) {
    pOVar1 = SysAllocStringLen((OLECHAR *)0x0,param_1);
    if (pOVar1 == (BSTR)0x0) {
      FUN_0041aa4c(1);
      return;
    }
  }
  return;
}



void FUN_0041bfa0(int *param_1,UINT param_2)

{
  uint uVar1;
  undefined4 *puVar2;
  
  puVar2 = (undefined4 *)0x0;
  if (0 < (int)param_2) {
    puVar2 = (undefined4 *)FUN_0041bf88(param_2);
    uVar1 = FUN_0041bf1c(*param_1);
    if (0 < (int)uVar1) {
      if ((int)param_2 < (int)uVar1) {
        uVar1 = param_2;
      }
      FUN_0041aae4((undefined4 *)*param_1,puVar2,uVar1 * 2);
    }
  }
  FUN_0041bdf4(param_1,puVar2);
  return;
}



void FUN_0041bfe4(int param_1,int param_2)

{
  bool bVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  
  puVar2 = (undefined4 *)(param_2 + 10 + (uint)*(byte *)(param_2 + 1));
  iVar3 = *(int *)(param_2 + 6 + (uint)*(byte *)(param_2 + 1));
  do {
    FUN_0041c0a4((undefined4 *)(puVar2[1] + param_1),*(char **)*puVar2);
    puVar2 = puVar2 + 2;
    iVar4 = iVar3 + -1;
    bVar1 = 0 < iVar3;
    iVar3 = iVar4;
  } while (iVar4 != 0 && bVar1);
  return;
}



void FUN_0041c010(undefined4 *param_1,char *param_2,int param_3)

{
  char cVar1;
  bool bVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  
  cVar1 = *param_2;
  uVar3 = (uint)(byte)param_2[1];
  if ((cVar1 != '\n') && (cVar1 != '\v')) {
    if (cVar1 == '\f') {
      do {
        *param_1 = 0;
        param_1[1] = 0;
        param_1[2] = 0;
        param_1[3] = 0;
        param_1 = param_1 + 4;
        iVar4 = param_3 + -1;
        bVar2 = 0 < param_3;
        param_3 = iVar4;
      } while (iVar4 != 0 && bVar2);
      return;
    }
    if (cVar1 == '\r') {
      do {
        iVar4 = *(int *)(param_2 + uVar3 + 2);
        FUN_0041c010(param_1,(char *)**(undefined4 **)(param_2 + uVar3 + 10),
                     *(int *)(param_2 + uVar3 + 6));
        iVar5 = param_3 + -1;
        bVar2 = 0 < param_3;
        param_1 = (undefined4 *)((int)param_1 + iVar4);
        param_3 = iVar5;
      } while (iVar5 != 0 && bVar2);
      return;
    }
    if (cVar1 == '\x0e') {
      do {
        iVar4 = *(int *)(param_2 + uVar3 + 2);
        FUN_0041bfe4((int)param_1,(int)param_2);
        iVar5 = param_3 + -1;
        bVar2 = 0 < param_3;
        param_1 = (undefined4 *)((int)param_1 + iVar4);
        param_3 = iVar5;
      } while (iVar5 != 0 && bVar2);
      return;
    }
    if ((cVar1 != '\x0f') && (cVar1 != '\x11')) {
      FUN_0041aa4c(CONCAT31((int3)((uint)param_1 >> 8),2));
      return;
    }
  }
  do {
    *param_1 = 0;
    param_1 = param_1 + 1;
    iVar4 = param_3 + -1;
    bVar2 = 0 < param_3;
    param_3 = iVar4;
  } while (iVar4 != 0 && bVar2);
  return;
}



void FUN_0041c0a4(undefined4 *param_1,char *param_2)

{
  FUN_0041c010(param_1,param_2,1);
  return;
}



int FUN_0041c0b0(int param_1,int param_2)

{
  bool bVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  
  puVar2 = (undefined4 *)(param_2 + 10 + (uint)*(byte *)(param_2 + 1));
  iVar3 = *(int *)(param_2 + 6 + (uint)*(byte *)(param_2 + 1));
  do {
    FUN_0041c1c4((VARIANTARG *)(puVar2[1] + param_1),*(char **)*puVar2);
    puVar2 = puVar2 + 2;
    iVar4 = iVar3 + -1;
    bVar1 = 0 < iVar3;
    iVar3 = iVar4;
  } while (iVar4 != 0 && bVar1);
  return param_1;
}



// WARNING: Type propagation algorithm not settling

VARIANTARG * FUN_0041c0dc(VARIANTARG *param_1,char *param_2,int param_3)

{
  char cVar1;
  bool bVar2;
  VARIANTARG *pVVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  
  if (param_3 != 0) {
    cVar1 = *param_2;
    uVar4 = (uint)(byte)param_2[1];
    if (cVar1 == '\n') {
      if (param_3 < 2) {
        FUN_0041b840((int *)param_1);
      }
      else {
        FUN_0041b864((int *)param_1,param_3);
      }
    }
    else if (cVar1 == '\v') {
      if (param_3 < 2) {
        FUN_0041be04((undefined4 *)param_1);
      }
      else {
        FUN_0041be1c((undefined4 *)param_1,param_3);
      }
    }
    else {
      pVVar3 = param_1;
      if (cVar1 == '\f') {
        do {
          FUN_0041cec4(pVVar3);
          iVar6 = param_3 + -1;
          bVar2 = 0 < param_3;
          pVVar3 = pVVar3 + 1;
          param_3 = iVar6;
        } while (iVar6 != 0 && bVar2);
      }
      else if (cVar1 == '\r') {
        do {
          iVar6 = *(int *)(param_2 + uVar4 + 2);
          FUN_0041c0dc(pVVar3,(char *)**(undefined4 **)(param_2 + uVar4 + 10),
                       *(int *)(param_2 + uVar4 + 6));
          iVar5 = param_3 + -1;
          bVar2 = 0 < param_3;
          pVVar3 = (VARIANTARG *)((int)&pVVar3->n1 + iVar6);
          param_3 = iVar5;
        } while (iVar5 != 0 && bVar2);
      }
      else if (cVar1 == '\x0e') {
        do {
          iVar6 = *(int *)(param_2 + uVar4 + 2);
          FUN_0041c0b0((int)pVVar3,(int)param_2);
          iVar5 = param_3 + -1;
          bVar2 = 0 < param_3;
          pVVar3 = (VARIANTARG *)((int)&pVVar3->n1 + iVar6);
          param_3 = iVar5;
        } while (iVar5 != 0 && bVar2);
      }
      else if (cVar1 == '\x0f') {
        do {
          FUN_0041d31c((int *)pVVar3);
          iVar6 = param_3 + -1;
          bVar2 = 0 < param_3;
          pVVar3 = (VARIANTARG *)((int)&pVVar3->n1 + 4);
          param_3 = iVar6;
        } while (iVar6 != 0 && bVar2);
      }
      else {
        if (cVar1 != '\x11') {
          pVVar3 = (VARIANTARG *)FUN_0041aa4c(CONCAT31((int3)((uint)param_1 >> 8),2));
          return pVVar3;
        }
        do {
          FUN_0041d0b8((int *)pVVar3,(int)param_2);
          iVar6 = param_3 + -1;
          bVar2 = 0 < param_3;
          pVVar3 = (VARIANTARG *)((int)&pVVar3->n1 + 4);
          param_3 = iVar6;
        } while (iVar6 != 0 && bVar2);
      }
    }
  }
  return param_1;
}



void FUN_0041c1c4(VARIANTARG *param_1,char *param_2)

{
  FUN_0041c0dc(param_1,param_2,1);
  return;
}



void FUN_0041c1d0(int param_1,int param_2,int param_3)

{
  char cVar1;
  int iVar2;
  int iVar3;
  char *pcVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  undefined4 *puVar8;
  
  puVar8 = (undefined4 *)(param_3 + 10 + (uint)*(byte *)(param_3 + 1));
  iVar7 = puVar8[-1];
  iVar5 = 0;
  iVar2 = puVar8[-2];
  do {
    uVar6 = puVar8[1] - iVar5;
    if (uVar6 != 0 && iVar5 <= (int)puVar8[1]) {
      FUN_0041aae4((undefined4 *)(iVar5 + param_2),(undefined4 *)(iVar5 + param_1),uVar6);
    }
    iVar3 = puVar8[1];
    pcVar4 = *(char **)*puVar8;
    cVar1 = *pcVar4;
    if (cVar1 == '\n') {
      FUN_0041b894((int *)(iVar3 + param_1),*(undefined4 **)(param_2 + iVar3));
      iVar5 = 4;
    }
    else if (cVar1 == '\v') {
      FUN_0041be40((BSTR *)(iVar3 + param_1),*(OLECHAR **)(param_2 + iVar3));
      iVar5 = 4;
    }
    else if (cVar1 == '\f') {
      FUN_0041c570((VARIANT *)(iVar3 + param_1),(VARIANTARG *)(param_2 + iVar3));
      iVar5 = 0x10;
    }
    else if (cVar1 == '\r') {
      uVar6 = (uint)(byte)pcVar4[1];
      iVar5 = *(int *)(pcVar4 + uVar6 + 2);
      FUN_0041c2ec((VARIANT *)(iVar3 + param_1),(VARIANTARG *)(param_2 + iVar3),
                   (char *)**(undefined4 **)(pcVar4 + uVar6 + 10),*(int *)(pcVar4 + uVar6 + 6));
    }
    else if (cVar1 == '\x0e') {
      iVar5 = *(int *)(pcVar4 + (byte)pcVar4[1] + 2);
      FUN_0041c1d0(iVar3 + param_1,param_2 + iVar3,(int)pcVar4);
    }
    else if (cVar1 == '\x0f') {
      FUN_0041d334((int *)(iVar3 + param_1),*(int **)(param_2 + iVar3));
      iVar5 = 4;
    }
    else {
      if (cVar1 != '\x11') {
        FUN_0041aa4c(CONCAT31((int3)((uint)iVar3 >> 8),2));
        return;
      }
      FUN_0041d0f4((int *)(iVar3 + param_1),*(int *)(param_2 + iVar3),(int)pcVar4);
      iVar5 = 4;
    }
    iVar5 = iVar5 + puVar8[1];
    puVar8 = puVar8 + 2;
    iVar7 = iVar7 + -1;
  } while (iVar7 != 0);
  uVar6 = iVar2 - iVar5;
  if (uVar6 != 0 && iVar5 <= iVar2) {
    FUN_0041aae4((undefined4 *)(iVar5 + param_2),(undefined4 *)(param_1 + iVar5),uVar6);
  }
  return;
}



// WARNING: Type propagation algorithm not settling

void FUN_0041c2ec(VARIANT *param_1,VARIANTARG *param_2,char *param_3,int param_4)

{
  int *piVar1;
  char cVar2;
  
  cVar2 = *param_3;
  if (cVar2 == '\n') {
    do {
      FUN_0041b894((int *)param_1,*(undefined4 **)&param_2->n1);
      param_1 = (VARIANT *)((int)&param_1->n1 + 4);
      param_2 = (VARIANTARG *)((int)&param_2->n1 + 4);
      param_4 = param_4 + -1;
    } while (param_4 != 0);
  }
  else if (cVar2 == '\v') {
    do {
      FUN_0041be40((BSTR *)param_1,*(OLECHAR **)&param_2->n1);
      param_1 = (VARIANT *)((int)&param_1->n1 + 4);
      param_2 = (VARIANTARG *)((int)&param_2->n1 + 4);
      param_4 = param_4 + -1;
    } while (param_4 != 0);
  }
  else if (cVar2 == '\f') {
    do {
      FUN_0041c570(param_1,param_2);
      param_1 = param_1 + 1;
      param_2 = param_2 + 1;
      param_4 = param_4 + -1;
    } while (param_4 != 0);
  }
  else if (cVar2 == '\r') {
    piVar1 = (int *)(param_3 + (byte)param_3[1] + 2);
    do {
      FUN_0041c2ec(param_1,param_2,(char *)piVar1[2],piVar1[1]);
      param_1 = (VARIANT *)((int)&param_1->n1 + *piVar1);
      param_2 = (VARIANTARG *)((int)&param_2->n1 + *piVar1);
      param_4 = param_4 + -1;
    } while (param_4 != 0);
  }
  else if (cVar2 == '\x0e') {
    do {
      FUN_0041c1d0((int)param_1,(int)param_2,(int)param_3);
      param_1 = (VARIANT *)((int)&param_1->n1 + *(int *)(param_3 + (byte)param_3[1] + 2));
      param_2 = (VARIANTARG *)((int)&param_2->n1 + *(int *)(param_3 + (byte)param_3[1] + 2));
      param_4 = param_4 + -1;
    } while (param_4 != 0);
  }
  else if (cVar2 == '\x0f') {
    do {
      FUN_0041d334((int *)param_1,*(int **)&param_2->n1);
      param_1 = (VARIANT *)((int)&param_1->n1 + 4);
      param_2 = (VARIANTARG *)((int)&param_2->n1 + 4);
      param_4 = param_4 + -1;
    } while (param_4 != 0);
  }
  else {
    if (cVar2 != '\x11') {
      FUN_0041aa4c(CONCAT31((int3)((uint)param_1 >> 8),2));
      return;
    }
    do {
      FUN_0041d0f4((int *)param_1,*(int *)&param_2->n1,(int)param_3);
      param_1 = (VARIANT *)((int)&param_1->n1 + 4);
      param_2 = (VARIANTARG *)((int)&param_2->n1 + 4);
      param_4 = param_4 + -1;
    } while (param_4 != 0);
  }
  return;
}



void FUN_0041c3e0(LPCWSTR param_1,int param_2,int *param_3)

{
  uint uVar1;
  CHAR local_810 [2048];
  
  if (param_2 == 0) {
    FUN_0041b840(param_3);
  }
  else if (param_2 < 0x400) {
    uVar1 = WideCharToMultiByte(0,0,param_1,param_2,local_810,0x800,(LPCSTR)0x0,(LPBOOL)0x0);
    FUN_0041b928(param_3,(undefined4 *)local_810,uVar1);
  }
  else {
    uVar1 = WideCharToMultiByte(0,0,param_1,param_2,(LPSTR)0x0,0,(LPCSTR)0x0,(LPBOOL)0x0);
    FUN_0041b928(param_3,(undefined4 *)0x0,uVar1);
    WideCharToMultiByte(0,0,param_1,param_2,(LPSTR)*param_3,uVar1,(LPCSTR)0x0,(LPBOOL)0x0);
  }
  return;
}



void FUN_0041c46c(LPCWSTR param_1,int *param_2)

{
  UINT UVar1;
  
  UVar1 = SysStringLen(param_1);
  FUN_0041c3e0(param_1,UVar1,param_2);
  return;
}



BSTR FUN_0041c490(LPCSTR param_1)

{
  int cbMultiByte;
  int iVar1;
  LPCSTR lpMultiByteStr;
  UINT UVar2;
  BSTR lpWideCharStr;
  WCHAR *lpWideCharStr_00;
  WCHAR local_810 [512];
  
  lpWideCharStr_00 = local_810;
  cbMultiByte = FUN_0041bac0((int)param_1);
  iVar1 = FUN_0041bac0((int)param_1);
  if (iVar1 < 0x400) {
    iVar1 = 0x400;
    lpMultiByteStr = FUN_0041bc74(param_1);
    UVar2 = MultiByteToWideChar(0,0,lpMultiByteStr,cbMultiByte,lpWideCharStr_00,iVar1);
    lpWideCharStr = SysAllocStringLen(local_810,UVar2);
  }
  else {
    UVar2 = MultiByteToWideChar(0,0,param_1,cbMultiByte,(LPWSTR)0x0,0);
    lpWideCharStr = SysAllocStringLen((OLECHAR *)0x0,UVar2);
    MultiByteToWideChar(0,0,param_1,cbMultiByte,lpWideCharStr,UVar2);
  }
  return lpWideCharStr;
}



void thunk_FUN_0041aa4c(uint param_1)

{
  FUN_0041aa4c(CONCAT31((int3)(param_1 >> 8),0xf));
  return;
}



void thunk_FUN_0041aa4c(uint param_1)

{
  FUN_0041aa4c(CONCAT31((int3)(param_1 >> 8),0x10));
  return;
}



void FUN_0041c52c(VARIANTARG *param_1)

{
  ushort uVar1;
  
  uVar1 = (param_1->n1).n2.vt;
  if (((uVar1 & 0x4000) != 0) || (uVar1 < 8)) {
    (param_1->n1).n2.vt = 0;
    return;
  }
  if (uVar1 == 0x100) {
    (param_1->n1).n2.vt = 0;
    FUN_0041b840((int *)((int)&param_1->n1 + 8));
    return;
  }
  if (uVar1 == 0x101) {
                    // WARNING: Could not recover jumptable at 0x0041c54e. Too many branches
                    // WARNING: Treating indirect jump as call
    (*DAT_00430dcc)();
    return;
  }
  VariantClear(param_1);
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0041c570(VARIANT *param_1,VARIANTARG *param_2)

{
  int iVar1;
  HRESULT HVar2;
  
  if (param_1 != param_2) {
    if (7 < (param_1->n1).n2.vt) {
      if ((param_1->n1).n2.vt == 0x100) {
        FUN_0041b840((int *)((int)&param_1->n1 + 8));
      }
      else if ((param_1->n1).n2.vt == 0x101) {
        (*DAT_00430dcc)();
      }
      else {
        VariantClear(param_1);
      }
    }
    if ((param_2->n1).n2.vt < 8) {
      *(undefined4 *)&param_1->n1 = *(undefined4 *)&param_2->n1;
      *(undefined4 *)((int)&param_1->n1 + 8) = *(undefined4 *)((int)&param_2->n1 + 8);
      *(undefined4 *)((int)&param_1->n1 + 0xc) = *(undefined4 *)((int)&param_2->n1 + 0xc);
      return;
    }
    if ((param_2->n1).n2.vt == 0x100) {
      iVar1 = *(int *)((int)&param_2->n1 + 8);
      if ((iVar1 != 0) && (-1 < *(int *)(iVar1 + -8))) {
        LOCK();
        *(int *)(iVar1 + -8) = *(int *)(iVar1 + -8) + 1;
        UNLOCK();
      }
      (param_1->n1).n2.vt = 0x100;
      *(int *)((int)&param_1->n1 + 8) = iVar1;
      return;
    }
    if ((param_2->n1).n2.vt == 0x101) {
      FUN_0041c5ae((undefined4 *)param_1,(undefined4 *)param_2);
                    // WARNING: Could not recover jumptable at 0x0041c5d4. Too many branches
                    // WARNING: Treating indirect jump as call
      (*_DAT_00430dd4)(param_1);
      return;
    }
    (param_1->n1).n2.vt = 0;
    HVar2 = VariantCopyInd(param_1,param_2);
    if (HVar2 != 0) {
      FUN_0041aa4c(CONCAT31((int3)((uint)HVar2 >> 8),0x10));
      return;
    }
  }
  return;
}



void FUN_0041c5ae(undefined4 *param_1,undefined4 *param_2)

{
  *param_1 = *param_2;
  param_1[2] = param_2[2];
  param_1[3] = param_2[3];
  return;
}



void FUN_0041c60c(VARIANTARG *param_1,undefined4 *param_2,undefined4 param_3)

{
  int iVar1;
  VARIANT *pVVar2;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_2c;
  undefined1 *puStack_28;
  undefined1 *puStack_24;
  VARIANT local_14;
  
  pVVar2 = &local_14;
  for (iVar1 = 4; iVar1 != 0; iVar1 = iVar1 + -1) {
    *(undefined4 *)&pVVar2->n1 = *param_2;
    param_2 = param_2 + 1;
    pVVar2 = (VARIANT *)((int)&pVVar2->n1 + 4);
  }
  puStack_24 = (undefined1 *)0x41c62f;
  FUN_0041cecc(&local_14);
  puStack_28 = &LAB_0041c668;
  uStack_2c = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_2c;
  puStack_24 = &stack0xfffffffc;
  (*DAT_00430dd0)(&local_14);
  FUN_0041c690(param_1,&local_14,param_3);
  *in_FS_OFFSET = uStack_2c;
  puStack_24 = &LAB_0041c66f;
  puStack_28 = (undefined1 *)0x41c667;
  FUN_0041cec4(&local_14);
  return;
}



bool FUN_0041c678(undefined4 param_1,short *param_2,undefined4 param_3)

{
  short sVar1;
  
  sVar1 = *param_2;
  if (sVar1 == 0x101) {
    FUN_0041c60c(param_1,(undefined4 *)param_2,param_3);
  }
  return sVar1 == 0x101;
}



void FUN_0041c690(VARIANTARG *param_1,VARIANTARG *param_2,undefined4 param_3)

{
  VARTYPE VVar1;
  bool bVar2;
  uint uVar3;
  _union_2683 local_14;
  
  VVar1 = (param_1->n1).n2.vt;
  if (VVar1 == 0x100) {
    bVar2 = FUN_0041c678(param_1,(short *)param_2,param_3);
    if (!bVar2) {
      local_14._0_4_ = (uint)local_14.n2.wReserved1 << 0x10;
      uVar3 = VariantChangeTypeEx((VARIANTARG *)&local_14.n2,param_2,0x400,0,(VARTYPE)param_3);
      if (uVar3 != 0) {
        thunk_FUN_0041aa4c(uVar3);
      }
      FUN_0041c52c(param_1);
      *(undefined4 *)&param_1->n1 = local_14._0_4_;
      *(undefined4 *)((int)&param_1->n1 + 8) = local_14._8_4_;
      *(undefined4 *)((int)&param_1->n1 + 0xc) = local_14._12_4_;
    }
  }
  else if (VVar1 == 0x101) {
    FUN_0041c60c(param_1,(undefined4 *)param_2,param_3);
  }
  else {
    bVar2 = FUN_0041c678(param_1,(short *)param_2,param_3);
    if (!bVar2) {
      uVar3 = VariantChangeTypeEx(param_1,param_2,0x400,0,(VARTYPE)param_3);
      if (uVar3 != 0) {
        thunk_FUN_0041aa4c(uVar3);
      }
    }
  }
  return;
}



void FUN_0041c734(VARIANTARG *param_1,int param_2)

{
  int local_8;
  
  local_8 = 0;
  FUN_0041c46c(*(LPCWSTR *)(param_2 + 8),&local_8);
  FUN_0041c52c(param_1);
  (param_1->n1).n2.vt = 0x100;
  *(int *)((int)&param_1->n1 + 8) = local_8;
  return;
}



void FUN_0041c764(VARIANTARG *param_1,int param_2)

{
  BSTR pOVar1;
  
  pOVar1 = FUN_0041c490(*(LPCSTR *)(param_2 + 8));
  FUN_0041c52c(param_1);
  (param_1->n1).n2.vt = 8;
  *(BSTR *)((int)&param_1->n1 + 8) = pOVar1;
  return;
}



void FUN_0041c784(VARIANT *param_1,VARIANTARG *param_2,undefined4 param_3)

{
  VARTYPE VVar1;
  VARTYPE VVar2;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_28;
  undefined1 *puStack_24;
  undefined1 *puStack_20;
  _union_2683 local_14;
  
  VVar1 = (param_2->n1).n2.vt;
  VVar2 = (VARTYPE)param_3;
  if (VVar2 == VVar1) {
    puStack_20 = (undefined1 *)0x41c7a0;
    FUN_0041c570(param_1,param_2);
  }
  else if (VVar1 == 0x100) {
    if (VVar2 != 8) {
      local_14.n2.vt = 0;
      puStack_20 = (undefined1 *)0x41c7cc;
      FUN_0041c764((VARIANTARG *)&local_14.n2,(int)param_2);
      puStack_24 = &LAB_0041c7fd;
      uStack_28 = *in_FS_OFFSET;
      *in_FS_OFFSET = &uStack_28;
      puStack_20 = &stack0xfffffffc;
      FUN_0041c690(param_1,(VARIANTARG *)&local_14.n2,param_3);
      *in_FS_OFFSET = uStack_28;
      puStack_20 = (undefined1 *)0x41c873;
      puStack_24 = (undefined1 *)0x41c7fc;
      FUN_0041c52c((VARIANTARG *)&local_14.n2);
      return;
    }
    puStack_20 = (undefined1 *)0x41c7b9;
    FUN_0041c764(param_1,(int)param_2);
  }
  else if ((VVar2 == 0x100) && (VVar1 != 0x101)) {
    if (VVar1 != 8) {
      local_14.n2.vt = 0;
      puStack_20 = (undefined1 *)0x41c833;
      FUN_0041c690((VARIANTARG *)&local_14.n2,param_2,CONCAT22((short)((uint)param_3 >> 0x10),8));
      puStack_24 = &LAB_0041c862;
      uStack_28 = *in_FS_OFFSET;
      *in_FS_OFFSET = &uStack_28;
      puStack_20 = &stack0xfffffffc;
      FUN_0041c734(param_1,(int)&local_14);
      *in_FS_OFFSET = uStack_28;
      puStack_20 = (undefined1 *)0x41c873;
      puStack_24 = (undefined1 *)0x41c861;
      FUN_0041c52c((VARIANTARG *)&local_14.n2);
      return;
    }
    puStack_20 = (undefined1 *)0x41c81f;
    FUN_0041c734(param_1,(int)param_2);
  }
  else {
    puStack_20 = (undefined1 *)0x41c873;
    FUN_0041c690(param_1,param_2,param_3);
  }
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

uint FUN_0041c878(VARIANTARG *param_1)

{
  float fVar1;
  VARTYPE VVar2;
  _union_2683 local_10;
  
  VVar2 = (param_1->n1).n2.vt;
  if (VVar2 == 3) {
    return *(uint *)((int)&param_1->n1 + 8);
  }
  if (VVar2 != 2) {
    if (VVar2 == 0x11) {
      return (uint)*(byte *)((int)&param_1->n1 + 8);
    }
    if (VVar2 == 5) {
      fVar1 = (float)*(double *)((int)&param_1->n1 + 8);
    }
    else if (VVar2 == 4) {
      fVar1 = *(float *)((int)&param_1->n1 + 8);
    }
    else {
      if (VVar2 != 6) {
        local_10.n2.vt = 0;
        FUN_0041c784((VARIANT *)&local_10.n2,param_1,3);
        return local_10._8_4_;
      }
      fVar1 = (float)*(longlong *)((int)&param_1->n1 + 8) / _DAT_0042d79c;
    }
    return (int)ROUND(fVar1);
  }
  return (int)*(short *)((int)&param_1->n1 + 8);
}



void FUN_0041c918(VARIANTARG *param_1)

{
  VARTYPE VVar1;
  _union_2683 local_10;
  
  VVar1 = (param_1->n1).n2.vt;
  if (VVar1 != 5) {
    if (VVar1 == 4) {
      return;
    }
    if (VVar1 == 6) {
      return;
    }
    if (VVar1 == 3) {
      return;
    }
    if (VVar1 == 2) {
      return;
    }
    if (VVar1 != 7) {
      local_10.n2.vt = 0;
      FUN_0041c784((VARIANT *)&local_10.n2,param_1,5);
      return;
    }
  }
  return;
}



void FUN_0041c9d0(int *param_1,VARIANTARG *param_2)

{
  _union_2683 local_14;
  
  if ((param_2->n1).n2.vt == 0x100) {
    FUN_0041b894(param_1,*(undefined4 **)((int)&param_2->n1 + 8));
    return;
  }
  local_14.n2.vt = 0;
  FUN_0041c784((VARIANT *)&local_14.n2,param_2,0x100);
  FUN_0041b840(param_1);
  *param_1 = local_14._8_4_;
  return;
}



void FUN_0041ca0c(BSTR *param_1,VARIANTARG *param_2)

{
  _union_2683 local_14;
  
  if ((param_2->n1).n2.vt == 8) {
    FUN_0041be40(param_1,*(OLECHAR **)((int)&param_2->n1 + 8));
    return;
  }
  local_14.n2.vt = 0;
  FUN_0041c784((VARIANT *)&local_14.n2,param_2,8);
  FUN_0041bdf4(param_1,local_14._8_4_);
  return;
}



void FUN_0041ca44(VARIANTARG *param_1,undefined4 param_2)

{
  if (7 < (param_1->n1).n2.vt) {
    FUN_0041c52c(param_1);
  }
  (param_1->n1).n2.vt = 3;
  *(undefined4 *)((int)&param_1->n1 + 8) = param_2;
  return;
}



void FUN_0041ca7c(VARIANTARG *param_1)

{
  longdouble in_ST0;
  
  if (7 < (param_1->n1).n2.vt) {
    FUN_0041c52c(param_1);
  }
  (param_1->n1).n2.vt = 5;
  *(double *)((int)&param_1->n1 + 8) = (double)in_ST0;
  return;
}



void FUN_0041caac(VARIANTARG *param_1,undefined4 *param_2)

{
  undefined4 *puVar1;
  
  if (7 < (param_1->n1).n2.vt) {
    FUN_0041c52c(param_1);
  }
  puVar1 = param_2;
  if (param_2 != (undefined4 *)0x0) {
    if ((int)param_2[-2] < 0) {
      puVar1 = FUN_0041b904(param_2[-1]);
      FUN_0041aae4(param_2,puVar1,puVar1[-1]);
    }
    else {
      LOCK();
      param_2[-2] = param_2[-2] + 1;
      UNLOCK();
    }
  }
  (param_1->n1).n2.vt = 0x100;
  *(undefined4 **)((int)&param_1->n1 + 8) = puVar1;
  return;
}



void FUN_0041caec(VARIANTARG *param_1,OLECHAR *param_2)

{
  BSTR pOVar1;
  
  if (7 < (param_1->n1).n2.vt) {
    FUN_0041c52c(param_1);
  }
  pOVar1 = (BSTR)0x0;
  if ((param_2 != (OLECHAR *)0x0) && (pOVar1 = (BSTR)0x0, (uint)param_2[-1] >> 1 != 0)) {
    pOVar1 = SysAllocStringLen(param_2,(uint)param_2[-1] >> 1);
    if (pOVar1 == (BSTR)0x0) {
      FUN_0041aa4c(1);
      return;
    }
  }
  (param_1->n1).n2.vt = 8;
  *(BSTR *)((int)&param_1->n1 + 8) = pOVar1;
  return;
}



void FUN_0041cb20(VARIANTARG *param_1,VARIANTARG *param_2)

{
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_28;
  undefined1 *puStack_24;
  undefined1 *puStack_20;
  BSTR local_14;
  BSTR local_10;
  undefined4 *local_c;
  undefined4 *local_8;
  
  puStack_20 = &stack0xfffffffc;
  local_8 = (undefined4 *)0x0;
  local_c = (undefined4 *)0x0;
  local_10 = (OLECHAR *)0x0;
  local_14 = (BSTR)0x0;
  puStack_24 = &LAB_0041cbc4;
  uStack_28 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_28;
  if ((param_1->n1).n2.vt == 0x100) {
    puStack_20 = &stack0xfffffffc;
    FUN_0041c9d0((int *)&local_8,param_1);
    FUN_0041c9d0((int *)&local_c,param_2);
    FUN_0041bac8((int *)&local_8,local_c);
    FUN_0041caac(param_1,local_8);
  }
  else {
    FUN_0041ca0c(&local_10,param_1);
    FUN_0041ca0c(&local_14,param_2);
    FUN_0041bf28((int *)&local_10,local_14);
    FUN_0041caec(param_1,local_10);
  }
  *in_FS_OFFSET = uStack_28;
  puStack_20 = &LAB_0041cbcb;
  puStack_24 = (undefined1 *)0x41cbb6;
  FUN_0041be1c(&local_14,2);
  puStack_24 = (undefined1 *)0x41cbc3;
  FUN_0041b864((int *)&local_c,2);
  return;
}



void FUN_0041cbd4(short *param_1,undefined4 *param_2,undefined4 param_3)

{
  int iVar1;
  _union_2683 *p_Var2;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_2c;
  undefined1 *puStack_28;
  undefined1 *puStack_24;
  _union_2683 local_14;
  
  p_Var2 = &local_14;
  for (iVar1 = 4; iVar1 != 0; iVar1 = iVar1 + -1) {
    *(undefined4 *)p_Var2 = *param_2;
    param_2 = param_2 + 1;
    p_Var2 = (_union_2683 *)((int)p_Var2 + 4);
  }
  puStack_24 = (undefined1 *)0x41cbf7;
  FUN_0041cecc((VARIANT *)&local_14.n2);
  puStack_28 = &LAB_0041cc47;
  uStack_2c = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_2c;
  puStack_24 = &stack0xfffffffc;
  if (*param_1 == 0x101) {
    puStack_24 = &stack0xfffffffc;
    (*DAT_00430dd0)(param_1);
  }
  if (local_14.n2.vt == 0x101) {
    (*DAT_00430dd0)(&local_14);
  }
  FUN_0041cc58((uint *)param_1,(uint *)&local_14,param_3);
  *in_FS_OFFSET = uStack_2c;
  puStack_24 = &LAB_0041cc4e;
  puStack_28 = (undefined1 *)0x41cc46;
  FUN_0041cec4((VARIANTARG *)&local_14.n2);
  return;
}



void FUN_0041cc58(uint *param_1,uint *param_2,undefined4 param_3)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  
  puVar1 = (undefined4 *)(*param_1 & 0xfff);
  puVar3 = (undefined4 *)(*param_2 & 0xfff);
  if ((undefined4 *)0x11 < puVar1) {
    puVar2 = puVar1;
    if (puVar1 != (undefined4 *)0x100) goto LAB_0041ccb4;
    puVar1 = (undefined4 *)0x8;
  }
  if ((undefined4 *)0x11 < puVar3) {
    puVar2 = puVar3;
    if (puVar3 != (undefined4 *)0x100) {
LAB_0041ccb4:
      if (puVar2 != (undefined4 *)0x101) {
        thunk_FUN_0041aa4c((uint)puVar2);
        return;
      }
      FUN_0041cbd4((short *)0x101,puVar3,param_3);
      return;
    }
    puVar3 = (undefined4 *)0x8;
  }
  (**(code **)(&UNK_0041ccc3 +
              (uint)*(byte *)(CONCAT31((int3)((uint)puVar3 >> 8),*(undefined1 *)(puVar3 + 0x10b5d2))
                              + 0x42d75c +
                             CONCAT31((int3)((uint)puVar1 >> 8),*(undefined1 *)(puVar1 + 0x10b5d2))
                             * 8) * 4))();
  return;
}



void __stdcall FUN_0041ce57(void)

{
  uint uVar1;
  uint uVar2;
  undefined4 uVar3;
  int unaff_EBX;
  VARIANTARG *unaff_ESI;
  VARIANTARG *unaff_EDI;
  
  if ((byte)unaff_EBX == 0) {
    FUN_0041cb20(unaff_EDI,unaff_ESI);
    return;
  }
  if ((byte)unaff_EBX < 4) {
    FUN_0041c918(unaff_ESI);
    FUN_0041c918(unaff_EDI);
    (**(code **)(&DAT_0041ce3b + unaff_EBX * 4))();
    FUN_0041ca7c(unaff_EDI);
    return;
  }
  uVar1 = FUN_0041c878(unaff_ESI);
  uVar2 = FUN_0041c878(unaff_EDI);
  uVar3 = (**(code **)(&DAT_0041cd22 + unaff_EBX * 4))(uVar2,uVar1);
  FUN_0041ca44(unaff_EDI,uVar3);
  return;
}



void FUN_0041ceb8(uint *param_1,uint *param_2)

{
  FUN_0041cc58(param_1,param_2,4);
  return;
}



VARIANTARG * FUN_0041cec4(VARIANTARG *param_1)

{
  FUN_0041c52c(param_1);
  return param_1;
}



void FUN_0041cecc(VARIANT *param_1)

{
  _union_2683 _Stack_10;
  
  if (7 < (param_1->n1).n2.vt) {
    _Stack_10._12_4_ = *(undefined4 *)((int)&param_1->n1 + 0xc);
    _Stack_10._8_4_ = *(undefined4 *)((int)&param_1->n1 + 8);
    _Stack_10.decVal.Hi32 = (param_1->n1).decVal.Hi32;
    _Stack_10._0_4_ = *(undefined4 *)&param_1->n1;
    (param_1->n1).n2.vt = 0;
    FUN_0041c570(param_1,(VARIANTARG *)&_Stack_10.n2);
  }
  return;
}



undefined4 FUN_0041cef0(int param_1)

{
  undefined4 uVar1;
  
  uVar1 = 0;
  if (param_1 != 0) {
    uVar1 = *(undefined4 *)(param_1 + -4);
  }
  return uVar1;
}



int FUN_0041cef8(int param_1)

{
  int iVar1;
  
  iVar1 = FUN_0041cef0(param_1);
  return iVar1 + -1;
}



void FUN_0041cf00(VARIANT *param_1,VARIANTARG *param_2,char *param_3,int param_4)

{
  FUN_0041c2ec(param_1,param_2,param_3,param_4);
  return;
}



// WARNING: Type propagation algorithm not settling

VARIANTARG * thunk_FUN_0041c0dc(VARIANTARG *param_1,char *param_2,int param_3)

{
  char cVar1;
  bool bVar2;
  VARIANTARG *pVVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  
  if (param_3 != 0) {
    cVar1 = *param_2;
    uVar4 = (uint)(byte)param_2[1];
    if (cVar1 == '\n') {
      if (param_3 < 2) {
        FUN_0041b840((int *)param_1);
      }
      else {
        FUN_0041b864((int *)param_1,param_3);
      }
    }
    else if (cVar1 == '\v') {
      if (param_3 < 2) {
        FUN_0041be04((undefined4 *)param_1);
      }
      else {
        FUN_0041be1c((undefined4 *)param_1,param_3);
      }
    }
    else {
      pVVar3 = param_1;
      if (cVar1 == '\f') {
        do {
          FUN_0041cec4(pVVar3);
          iVar6 = param_3 + -1;
          bVar2 = 0 < param_3;
          pVVar3 = pVVar3 + 1;
          param_3 = iVar6;
        } while (iVar6 != 0 && bVar2);
      }
      else if (cVar1 == '\r') {
        do {
          iVar6 = *(int *)(param_2 + uVar4 + 2);
          FUN_0041c0dc(pVVar3,(char *)**(undefined4 **)(param_2 + uVar4 + 10),
                       *(int *)(param_2 + uVar4 + 6));
          iVar5 = param_3 + -1;
          bVar2 = 0 < param_3;
          pVVar3 = (VARIANTARG *)((int)&pVVar3->n1 + iVar6);
          param_3 = iVar5;
        } while (iVar5 != 0 && bVar2);
      }
      else if (cVar1 == '\x0e') {
        do {
          iVar6 = *(int *)(param_2 + uVar4 + 2);
          FUN_0041c0b0((int)pVVar3,(int)param_2);
          iVar5 = param_3 + -1;
          bVar2 = 0 < param_3;
          pVVar3 = (VARIANTARG *)((int)&pVVar3->n1 + iVar6);
          param_3 = iVar5;
        } while (iVar5 != 0 && bVar2);
      }
      else if (cVar1 == '\x0f') {
        do {
          FUN_0041d31c((int *)pVVar3);
          iVar6 = param_3 + -1;
          bVar2 = 0 < param_3;
          pVVar3 = (VARIANTARG *)((int)&pVVar3->n1 + 4);
          param_3 = iVar6;
        } while (iVar6 != 0 && bVar2);
      }
      else {
        if (cVar1 != '\x11') {
          pVVar3 = (VARIANTARG *)FUN_0041aa4c(CONCAT31((int3)((uint)param_1 >> 8),2));
          return pVVar3;
        }
        do {
          FUN_0041d0b8((int *)pVVar3,(int)param_2);
          iVar6 = param_3 + -1;
          bVar2 = 0 < param_3;
          pVVar3 = (VARIANTARG *)((int)&pVVar3->n1 + 4);
          param_3 = iVar6;
        } while (iVar6 != 0 && bVar2);
      }
    }
  }
  return param_1;
}



void FUN_0041cf18(int *param_1,int param_2)

{
  FUN_0041d0b8(param_1,param_2);
  return;
}



void FUN_0041cf20(int *param_1,int param_2,int param_3,int *param_4)

{
  undefined4 *puVar1;
  int *piVar2;
  int iVar3;
  char *pcVar4;
  int iVar5;
  int *local_24;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  int *local_8;
  
  piVar2 = (int *)*param_1;
  iVar5 = *param_4;
  local_c = param_3;
  local_8 = param_1;
  if (iVar5 < 1) {
    if (iVar5 < 0) {
      FUN_0041aa4c(CONCAT31((int3)((uint)param_4 >> 8),4));
    }
    FUN_0041cf18(local_8,param_2);
  }
  else {
    local_14 = 0;
    if (piVar2 != (int *)0x0) {
      local_14 = piVar2[-1];
      piVar2 = piVar2 + -2;
    }
    iVar3 = param_2 + (uint)*(byte *)(param_2 + 1);
    local_1c = *(int *)(iVar3 + 2);
    puVar1 = *(undefined4 **)(iVar3 + 6);
    if (puVar1 == (undefined4 *)0x0) {
      pcVar4 = (char *)0x0;
    }
    else {
      pcVar4 = (char *)*puVar1;
    }
    local_20 = iVar5 * local_1c;
    if (local_20 / iVar5 != local_1c) {
      FUN_0041aa4c(CONCAT31((int3)((uint)(local_20 / iVar5) >> 8),4));
    }
    local_20 = local_20 + 8;
    if ((piVar2 == (int *)0x0) || (*piVar2 == 1)) {
      local_24 = piVar2;
      if ((iVar5 < local_14) && (pcVar4 != (char *)0x0)) {
        thunk_FUN_0041c0dc((VARIANTARG *)((int)piVar2 + iVar5 * local_1c + 8),pcVar4,
                           local_14 - iVar5);
      }
      FUN_0041a9f0((int *)&local_24,local_20);
      piVar2 = local_24;
    }
    else {
      *piVar2 = *piVar2 + -1;
      piVar2 = (int *)FUN_0041a9c0(local_20);
      local_18 = local_14;
      if (iVar5 < local_14) {
        local_18 = iVar5;
      }
      if (pcVar4 == (char *)0x0) {
        FUN_0041aae4((undefined4 *)*local_8,piVar2 + 2,local_18 * local_1c);
      }
      else {
        FUN_0041abb0(piVar2 + 2,local_18 * local_1c,0);
        FUN_0041cf00((VARIANT *)(piVar2 + 2),(VARIANTARG *)*local_8,pcVar4,local_18);
      }
    }
    *piVar2 = 1;
    piVar2[1] = iVar5;
    piVar2 = piVar2 + 2;
    FUN_0041abb0((int *)(local_1c * local_14 + (int)piVar2),(iVar5 - local_14) * local_1c,0);
    if (1 < local_c) {
      local_c = local_c + -1;
      if (-1 < iVar5 + -1) {
        local_10 = 0;
        do {
          FUN_0041cf20(piVar2 + local_10,(int)pcVar4,local_c,param_4 + 1);
          local_10 = local_10 + 1;
          iVar5 = iVar5 + -1;
        } while (iVar5 != 0);
      }
    }
    *local_8 = (int)piVar2;
  }
  return;
}



void FUN_0041d0ac(int *param_1,int param_2,int param_3)

{
  FUN_0041cf20(param_1,param_2,param_3,(int *)&stack0x00000004);
  return;
}



int * FUN_0041d0b8(int *param_1,int param_2)

{
  int *piVar1;
  undefined4 *puVar2;
  int iVar3;
  VARIANTARG *pVVar4;
  
  pVVar4 = (VARIANTARG *)*param_1;
  if (pVVar4 != (VARIANTARG *)0x0) {
    *param_1 = 0;
    LOCK();
    piVar1 = (int *)((int)&pVVar4[-1].n1 + 8);
    *piVar1 = *piVar1 + -1;
    UNLOCK();
    if (*piVar1 == 0) {
      puVar2 = *(undefined4 **)(param_2 + 6 + (uint)*(byte *)(param_2 + 1));
      if ((puVar2 != (undefined4 *)0x0) && (iVar3 = *(int *)((int)&pVVar4[-1].n1 + 0xc), iVar3 != 0)
         ) {
        pVVar4 = FUN_0041c0dc(pVVar4,(char *)*puVar2,iVar3);
      }
      FUN_0041a9d8((int)((int)&pVVar4[-1].n1 + 8));
    }
  }
  return param_1;
}



void FUN_0041d0f4(int *param_1,int param_2,int param_3)

{
  int *piVar1;
  int iVar2;
  
  iVar2 = *param_1;
  if (param_2 != 0) {
    LOCK();
    *(int *)(param_2 + -8) = *(int *)(param_2 + -8) + 1;
    UNLOCK();
  }
  if (iVar2 != 0) {
    LOCK();
    piVar1 = (int *)(iVar2 + -8);
    *piVar1 = *piVar1 + -1;
    UNLOCK();
    if (*piVar1 == 0) {
      *(int *)(iVar2 + -8) = *(int *)(iVar2 + -8) + 1;
      FUN_0041d0b8(param_1,param_3);
    }
  }
  *param_1 = param_2;
  return;
}



PVOID FUN_0041d11c(LPCVOID param_1)

{
  _MEMORY_BASIC_INFORMATION local_1c;
  
  VirtualQuery(param_1,&local_1c,0x1c);
  if (local_1c.State != 0x1000) {
    local_1c.AllocationBase = (PVOID)0x0;
  }
  return local_1c.AllocationBase;
}



int FUN_0041d144(int param_1)

{
  undefined4 *puVar1;
  
  puVar1 = DAT_0042d6fc;
  while( true ) {
    if (puVar1 == (undefined4 *)0x0) {
      return param_1;
    }
    if (((param_1 == puVar1[1]) || (param_1 == puVar1[2])) || (param_1 == puVar1[3])) break;
    puVar1 = (undefined4 *)*puVar1;
  }
  return puVar1[4];
}



void FUN_0041d170(int param_1)

{
  FUN_0041d198(param_1);
  return;
}



void FUN_0041d198(int param_1)

{
  int *piVar1;
  int *piVar2;
  
  piVar2 = DAT_0042d700;
  if ((DAT_0042d700 != (int *)0x0) && (DAT_0042d700[1] == param_1)) {
    DAT_0042d700 = (int *)*DAT_0042d700;
    FUN_0041a9d8((int)piVar2);
    return;
  }
  while( true ) {
    if (piVar2 == (int *)0x0) {
      return;
    }
    piVar1 = (int *)*piVar2;
    if ((piVar1 != (int *)0x0) && (piVar1[1] == param_1)) break;
    piVar2 = (int *)*piVar2;
  }
  *piVar2 = *piVar1;
  FUN_0041a9d8((int)piVar1);
  return;
}



void FUN_0041d1f8(undefined4 param_1,undefined4 param_2,undefined1 *param_3)

{
  undefined4 *puVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_24;
  undefined1 *puStack_20;
  undefined1 *puStack_1c;
  
  for (puVar1 = DAT_0042d700; puVar1 != (undefined4 *)0x0; puVar1 = (undefined4 *)*puVar1) {
    puStack_20 = &LAB_0041d233;
    uStack_24 = *in_FS_OFFSET;
    *in_FS_OFFSET = &uStack_24;
    puStack_1c = &stack0xfffffffc;
    (*(code *)puVar1[1])(param_1,param_2,param_3);
    *in_FS_OFFSET = uStack_24;
    param_2 = uStack_24;
    param_3 = puStack_1c;
  }
  return;
}



void FUN_0041d254(undefined4 *param_1,undefined4 param_2,undefined1 *param_3)

{
  undefined4 *puVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_14;
  undefined1 *puStack_10;
  undefined1 *puStack_c;
  undefined4 *local_8;
  
  puStack_c = &stack0xfffffffc;
  puStack_10 = &LAB_0041d2b8;
  uStack_14 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_14;
  local_8 = param_1;
  FUN_0041d1f8(param_1[1],0,param_3);
  *in_FS_OFFSET = uStack_14;
  puVar1 = DAT_0042d6fc;
  if (local_8 == DAT_0042d6fc) {
    DAT_0042d6fc = (undefined4 *)*local_8;
  }
  else {
    for (; puVar1 != (undefined4 *)0x0; puVar1 = (undefined4 *)*puVar1) {
      if ((undefined4 *)*puVar1 == local_8) {
        *puVar1 = *local_8;
        return;
      }
    }
  }
  return;
}



void FUN_0041d2c4(undefined4 *param_1,int *param_2)

{
  HINSTANCE hInstance;
  uint uVar1;
  UINT uID;
  CHAR *lpBuffer;
  int cchBufferMax;
  CHAR local_408 [1024];
  
  lpBuffer = local_408;
  if (param_1 != (undefined4 *)0x0) {
    if ((int)param_1[1] < 0x10000) {
      cchBufferMax = 0x400;
      uID = param_1[1];
      hInstance = (HINSTANCE)FUN_0041d144(*(int *)*param_1);
      uVar1 = LoadStringA(hInstance,uID,lpBuffer,cchBufferMax);
      FUN_0041b928(param_2,(undefined4 *)local_408,uVar1);
    }
    else {
      FUN_0041b9f8(param_2,(char *)param_1[1]);
    }
  }
  return;
}



int * FUN_0041d31c(int *param_1)

{
  int *piVar1;
  
  piVar1 = (int *)*param_1;
  if (piVar1 != (int *)0x0) {
    *param_1 = 0;
    (**(code **)(*piVar1 + 8))();
    param_1 = piVar1;
  }
  return param_1;
}



void FUN_0041d334(int *param_1,int *param_2)

{
  int *piVar1;
  
  piVar1 = (int *)*param_1;
  *param_1 = (int)param_2;
  if (param_2 != (int *)0x0) {
    (**(code **)(*param_2 + 4))();
  }
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))();
  }
  return;
}



undefined4 __stdcall FUN_0041d37c(int *param_1,int *param_2,int *param_3)

{
  char cVar1;
  undefined4 uVar2;
  
  cVar1 = FUN_0041aee8(param_1,param_2,param_3);
  if (cVar1 == '\0') {
    uVar2 = 0x80004002;
  }
  else {
    uVar2 = 0;
  }
  return uVar2;
}



void __stdcall FUN_0041d3a4(int param_1)

{
  InterlockedIncrement((LONG *)(param_1 + 4));
  return;
}



LONG __stdcall FUN_0041d3b8(int *param_1)

{
  LONG LVar1;
  
  LVar1 = InterlockedDecrement(param_1 + 1);
  if (LVar1 == 0) {
    (**(code **)(*param_1 + -4))(param_1,1);
  }
  return LVar1;
}



void FUN_0041d3e0(undefined *param_1)

{
  int iVar1;
  
  if (*(int *)(param_1 + 4) != 0xd7b1) {
    if (2 < *(int *)(param_1 + 4) - 0xd7b1U) {
      if (param_1 == &DAT_00430a34) {
        return;
      }
      iVar1 = 0x67;
      goto LAB_0041d405;
    }
    iVar1 = (**(code **)(param_1 + 0x1c))();
    if (iVar1 != 0) goto LAB_0041d405;
  }
  iVar1 = (**(code **)(param_1 + 0x24))(param_1);
  if (iVar1 == 0) {
    return;
  }
LAB_0041d405:
  FUN_0041aab0(iVar1);
  return;
}



char FUN_0041d41c(undefined *param_1)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  
  if (*(int *)(param_1 + 4) != 0xd7b1) {
    if (param_1 == &DAT_00430a34) {
      FUN_004189fb(&DAT_00430a34);
      param_1 = &DAT_00430a34;
      if (DAT_00430a38 == 0xd7b1) goto LAB_0041d425;
    }
    FUN_0041aab0(0x68);
    return '\x1a';
  }
LAB_0041d425:
  uVar3 = *(uint *)(param_1 + 0xc);
  if (*(uint *)(param_1 + 0x10) <= uVar3) {
    iVar2 = (**(code **)(param_1 + 0x1c))();
    if (iVar2 != 0) {
      FUN_0041aab0(iVar2);
      return '\x1a';
    }
    uVar3 = *(uint *)(param_1 + 0xc);
    if (*(uint *)(param_1 + 0x10) <= uVar3) {
      *(undefined1 *)(*(int *)(param_1 + 0x14) + uVar3) = 0x1a;
      *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + 1;
      return '\x1a';
    }
  }
  cVar1 = *(char *)(*(int *)(param_1 + 0x14) + uVar3);
  if (cVar1 != '\x1a') {
    *(uint *)(param_1 + 0xc) = uVar3 + 1;
  }
  return cVar1;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __stdcall FUN_0041d490(void)

{
  undefined4 *in_FS_OFFSET;
  undefined4 uStack_10;
  undefined1 *puStack_c;
  undefined1 *puStack_8;
  
  puStack_8 = &stack0xfffffffc;
  puStack_c = &LAB_0041d4ee;
  uStack_10 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_10;
  _DAT_00430e10 = _DAT_00430e10 + 1;
  if (_DAT_00430e10 == 0) {
    FUN_0041d3e0(&DAT_00430a34);
    FUN_0041d3e0(&DAT_00430c00);
    FUN_00419df0();
    FUN_0041cec4((VARIANTARG *)&DAT_00430df8);
    FUN_0041cec4((VARIANTARG *)&DAT_00430de8);
    FUN_0041cec4((VARIANTARG *)&DAT_00430dd8);
  }
  *in_FS_OFFSET = uStack_10;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __stdcall FUN_0041d4f8(void)

{
  WORD WVar1;
  undefined4 uVar2;
  undefined2 extraout_var;
  undefined4 *in_FS_OFFSET;
  bool bVar3;
  undefined4 uStack_10;
  undefined1 *puStack_c;
  undefined1 *puStack_8;
  
  puStack_8 = &stack0xfffffffc;
  puStack_c = &LAB_0041d5e0;
  uStack_10 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack_10;
  bVar3 = _DAT_00430e10 == 0;
  _DAT_00430e10 = _DAT_00430e10 + -1;
  if (bVar3) {
    DAT_00430a1c = 0;
    DAT_00430a24 = 0;
    _DAT_00430a28 = 0;
    DAT_00430a2e = 2;
    DAT_00430a2f = 2;
    DAT_00430a30 = 3;
    _DAT_00430dd8 = 0;
    _DAT_00430de8 = 1;
    _DAT_00430df8 = 10;
    _DAT_00430e00 = 0x80020004;
    DAT_00430dcc = thunk_FUN_0041aa4c;
    DAT_00430dd0 = thunk_FUN_0041aa4c;
    _DAT_00430dd4 = thunk_FUN_0041aa4c;
    uVar2 = FUN_0041ac9c();
    if ((char)uVar2 != '\0') {
      FUN_0041accc();
    }
    FUN_0041ad90();
    FUN_00418726((undefined4 *)&DAT_00430a34,"");
    FUN_00418726((undefined4 *)&DAT_00430c00,"");
    _DAT_00430a14 = GetCommandLineA();
    WVar1 = FUN_0041967c();
    _DAT_00430a10 = CONCAT22(extraout_var,WVar1);
    _DAT_00430a08 = GetCurrentThreadId();
  }
  *in_FS_OFFSET = uStack_10;
  return;
}



void __stdcall BCB_FreeObject(void)

{
  FUN_0041da5c();
  return;
}



void __stdcall FUN_0041d600(void)

{
  BCB_FreeObject();
  return;
}



undefined4 __cdecl FUN_0041d610(undefined4 param_1)

{
  undefined4 uVar1;
  
  uVar1 = DAT_00430eb4;
  DAT_00430eb4 = param_1;
  return uVar1;
}



int __stdcall FUN_0041d624(void)

{
  int iVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  undefined **local_8;
  
  BCB_InitExceptionHandler(0x42d7d0);
  while (iVar1 = FUN_0041da4c(), iVar1 == 0) {
    if (DAT_00430eb4 == (code *)0x0) {
      local_8 = &PTR_FUN_0042d828;
      FUN_004245b3((uint *)&DAT_0041d6b4,&local_8,0,&LAB_0041d70c,1,0,0,0);
    }
    else {
      (*DAT_00430eb4)();
    }
  }
  *in_FS_OFFSET = local_2c;
  return iVar1;
}



undefined * __stdcall FUN_0041d7b8(void)

{
  return PTR_s_bad_alloc_exception_thrown_0042d9a8;
}



void __stdcall FUN_0041d7c4(void)

{
  undefined **local_8;
  
  BCB_InitExceptionHandler(0x42d840);
  local_8 = &PTR_FUN_0042d828;
  FUN_004245b3((uint *)&DAT_0041d6b4,&local_8,0,&LAB_0041d70c,1,0,0,0);
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __stdcall FUN_0041d824(void)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_28;
  
  BCB_InitExceptionHandler(0x42d888);
  _DAT_00430eb8 = &PTR_FUN_0042d828;
  *in_FS_OFFSET = local_28;
  return;
}



void __stdcall FUN_0041d864(void)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_28;
  
  BCB_InitExceptionHandler(0x42d89c);
  FUN_0041e7b4((undefined4 *)&DAT_00430eb8,2);
  *in_FS_OFFSET = local_28;
  return;
}



bool FUN_0041d898(int param_1,undefined4 *param_2,uint *param_3)

{
  LPVOID pvVar1;
  uint dwSize;
  
  if (DAT_0042d8a4 == 0) {
    if ((DAT_004316ac == 2) || (DAT_004316ac == 1)) {
      DAT_0042d8a4 = 0x100000;
    }
    else {
      DAT_0042d8a4 = 0x400000;
    }
  }
  dwSize = (DAT_0042d8a4 + param_1) - 1U & ~(DAT_0042d8a4 - 1U);
  pvVar1 = VirtualAlloc((LPVOID)0x0,dwSize,0x2000,1);
  *param_2 = pvVar1;
  if (pvVar1 != (LPVOID)0x0) {
    *param_3 = dwSize;
  }
  return pvVar1 != (LPVOID)0x0;
}



bool FUN_0041d904(undefined4 *param_1,SIZE_T param_2)

{
  LPVOID pvVar1;
  
  pvVar1 = VirtualAlloc((LPVOID)*param_1,param_2,0x1000,4);
  *param_1 = pvVar1;
  return pvVar1 != (LPVOID)0x0;
}



undefined4 FUN_0041d92c(LPVOID param_1,int param_2)

{
  LPVOID pvVar1;
  LPVOID lpAddress;
  
  lpAddress = param_1;
  while( true ) {
    if (param_2 == 0) {
      return 1;
    }
    pvVar1 = VirtualAlloc(lpAddress,0x1000,0x1000,4);
    if (pvVar1 == (LPVOID)0x0) break;
    lpAddress = (LPVOID)((int)lpAddress + 0x1000);
    param_2 = param_2 + -0x1000;
  }
  FUN_0041d978(param_1,(int)lpAddress - (int)param_1);
  return 0;
}



undefined4 FUN_0041d978(LPVOID param_1,SIZE_T param_2)

{
  BOOL BVar1;
  
  BVar1 = VirtualFree(param_1,param_2,0x4000);
  if (BVar1 != 1) {
    return 0;
  }
  return 1;
}



undefined4 FUN_0041d990(LPVOID param_1)

{
  BOOL BVar1;
  
  BVar1 = VirtualFree(param_1,0,0x8000);
  if (BVar1 != 1) {
    return 0;
  }
  return 1;
}



undefined4 __cdecl FUN_0041d9cc(int *param_1,int *param_2,int *param_3)

{
  undefined4 uVar1;
  HMODULE hModule;
  FARPROC pFVar2;
  
  if (((param_1 == (int *)0x0) || (param_2 == (int *)0x0)) || (param_3 == (int *)0x0)) {
    uVar1 = 0;
  }
  else {
    hModule = GetModuleHandleA(s_borlndmm_0042d8e1);
    if (hModule == (HMODULE)0x0) {
      uVar1 = 0;
    }
    else {
      pFVar2 = GetProcAddress(hModule,s__Borlndmm_SysGetMem_qqri_0042d8ea);
      *param_1 = (int)pFVar2;
      pFVar2 = GetProcAddress(hModule,s__Borlndmm_SysFreeMem_qqrpv_0042d903);
      *param_2 = (int)pFVar2;
      pFVar2 = GetProcAddress(hModule,s__Borlndmm_SysReallocMem_qqrpvi_0042d91e);
      *param_3 = (int)pFVar2;
      if (((*param_1 == 0) || (*param_2 == 0)) || (*param_3 == 0)) {
        uVar1 = 0;
      }
      else {
        uVar1 = 1;
      }
    }
  }
  return uVar1;
}



undefined4 __stdcall FUN_0041da48(void)

{
  return 0;
}



void __stdcall FUN_0041da4c(void)

{
  (*(code *)PTR_FUN_0042d954)();
  return;
}



void __stdcall FUN_0041da5c(void)

{
  (*(code *)PTR_FUN_0042d950)();
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __cdecl FUN_0041da6c(undefined4 param_1)

{
  int iVar1;
  
  iVar1 = FUN_0041da48();
  if (iVar1 == 0) {
    PTR_FUN_0042d950 = FUN_0041e10c;
    PTR_FUN_0042d954 = FUN_0041e27c;
    PTR_FUN_0042d958 = FUN_0041e6d4;
    PTR_FUN_0042d95c = FUN_0041e494;
    _DAT_0042d94c = 1;
  }
  _DAT_0042d948 = param_1;
  return;
}



void __cdecl FUN_0041dab4(undefined4 param_1)

{
  undefined4 extraout_EDX;
  undefined4 uVar1;
  
  uVar1 = 1;
  FUN_0041da6c(1);
  (*(code *)PTR_FUN_0042d950)(param_1,extraout_EDX,uVar1,param_1);
  return;
}



void __cdecl FUN_0041dacc(undefined4 param_1)

{
  undefined4 extraout_EDX;
  undefined4 uVar1;
  
  uVar1 = 1;
  FUN_0041da6c(1);
  (*(code *)PTR_FUN_0042d954)(param_1,extraout_EDX,uVar1,param_1);
  return;
}



void __cdecl FUN_0041dae4(undefined4 param_1,undefined4 param_2)

{
  undefined4 uVar1;
  
  uVar1 = 1;
  FUN_0041da6c(1);
  (*(code *)PTR_FUN_0042d958)(param_2,param_1,uVar1,param_1,param_2);
  return;
}



void __stdcall FUN_0041db04(void)

{
  return;
}



void __stdcall FUN_0041db14(void)

{
  if (DAT_0042d940 != 0) {
    (*(code *)PTR_FUN_0042d95c)();
  }
  return;
}



void __stdcall FUN_0041db24(void)

{
  return;
}



void __stdcall FUN_0041db28(void)

{
  int iVar1;
  int iVar2;
  
  for (iVar2 = 0xc; iVar2 < DAT_0042d970; iVar2 = iVar2 + 4) {
    iVar1 = iVar2 * 2 + DAT_0042d984;
    *(int *)(iVar1 + -8) = iVar1 + -0xc;
    *(int *)(iVar1 + -4) = iVar1 + -0xc;
  }
  iVar2 = DAT_0042d970 * 2 + DAT_0042d984;
  *(undefined4 *)(iVar2 + -8) = 0;
  *(undefined4 *)(iVar2 + -4) = 0;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 __cdecl FUN_0041db68(uint *param_1,uint param_2,uint param_3)

{
  undefined4 uVar1;
  undefined *puVar2;
  uint *puVar3;
  int iVar4;
  uint uVar5;
  
  iVar4 = 0;
  if (param_2 < 0x1000) {
    uVar1 = 0xffffffff;
  }
  else {
    param_1[2] = 1;
    param_1[3] = (uint)param_1;
    *param_1 = param_2;
    param_1[1] = param_3;
    param_1[0x23] = (uint)DAT_0042d97c;
    param_1[0x24] = 0;
    if (DAT_0042d97c == (uint *)0x0) {
      _DAT_0042d980 = param_1;
    }
    else {
      DAT_0042d97c[0x24] = (uint)param_1;
    }
    DAT_0042d97c = param_1;
    param_1[0x25] = 0;
    puVar3 = param_1 + 0x26;
    if (DAT_0042d984 == (uint *)0x0) {
      uVar5 = DAT_0042d970 * 2 + 3 & 0xfffffffc;
      DAT_0042d984 = param_1 + 0x27;
      *puVar3 = uVar5;
      puVar3 = (uint *)((int)puVar3 + uVar5 + 4);
      FUN_0041db28();
      iVar4 = uVar5 + 4;
    }
    uVar5 = (param_2 - 0xa0) - iVar4;
    *puVar3 = uVar5 + 1;
    *(undefined4 *)((int)puVar3 + (uVar5 + 1 & 0xfffffffc) + 4) = 2;
    puVar2 = PTR_DAT_0042d994;
    if (uVar5 < DAT_0042d970) {
      puVar2 = (undefined *)((int)DAT_0042d984 + uVar5 * 2 + -0xc);
    }
    puVar3[1] = *(uint *)(puVar2 + 4);
    puVar3[2] = (uint)puVar2;
    *(uint **)(puVar3[1] + 8) = puVar3;
    *(uint **)(puVar2 + 4) = puVar3;
    *(uint *)((int)puVar3 + uVar5) = uVar5 + 4;
    uVar1 = 0;
  }
  return uVar1;
}



undefined4 __cdecl FUN_0041dc68(uint *param_1,uint param_2)

{
  uint uVar1;
  int iVar2;
  uint *puVar3;
  uint uVar4;
  
  uVar4 = param_2 & 0xfffff000;
  uVar1 = *param_1;
  iVar2 = uVar1 + (int)param_1;
  puVar3 = (uint *)(iVar2 + -4);
  if (uVar4 < uVar1) {
    if ((*(byte *)puVar3 & 2) == 0) {
      return 0xffffffff;
    }
    puVar3 = (uint *)((int)puVar3 - *(int *)(iVar2 + -8));
    if ((*puVar3 & 0xfffffffc) - 0xc < uVar1 - uVar4) {
      return 0xffffffff;
    }
    *puVar3 = *puVar3 - (uVar1 - uVar4);
    *(uint *)((int)puVar3 + (*puVar3 & 0xfffffffc)) = (*puVar3 & 0xfffffffc) + 4;
    *(undefined4 *)((int)puVar3 + (*puVar3 & 0xfffffffc) + 4) = 2;
    if ((*puVar3 & 0xfffffffc) < DAT_0042d970) {
      uVar1 = puVar3[1];
      *(uint *)(uVar1 + 8) = puVar3[2];
      *(uint *)(puVar3[2] + 4) = uVar1;
      puVar3[1] = *(uint *)(DAT_0042d984 + -8 + (*puVar3 & 0xfffffffc) * 2);
      puVar3[2] = ((*puVar3 & 0xfffffffc) * 2 + DAT_0042d984) - 0xc;
      *(uint **)(puVar3[1] + 8) = puVar3;
      *(uint **)(DAT_0042d984 + -8 + (*puVar3 & 0xfffffffc) * 2) = puVar3;
    }
  }
  else {
    uVar1 = *param_1;
    *puVar3 = *puVar3 + (uVar4 - uVar1) + -4;
    *(undefined4 *)(iVar2 + (*puVar3 & 0xfffffffc)) = 0;
    FUN_0041e138(iVar2);
    *param_1 = *param_1 + (uVar4 - uVar1);
  }
  return 0;
}



undefined4 __cdecl FUN_0041dd54(int param_1)

{
  uint *puVar1;
  bool bVar2;
  int iVar3;
  undefined3 extraout_var;
  undefined4 uVar5;
  int iVar6;
  uint uVar7;
  uint local_c;
  uint *local_8;
  uint uVar4;
  
  uVar7 = param_1 + 0xfffU & 0xfffff000;
  if (DAT_0042d998 == 0) {
    DAT_0042d998 = FUN_0041e47c();
  }
  puVar1 = DAT_0042d97c;
  if (DAT_0042d984 == 0) {
    uVar7 = uVar7 + (DAT_0042d970 * 2 + 0x109fU & 0xfffff000);
  }
  for (; puVar1 != (uint *)0x0; puVar1 = (uint *)puVar1[0x23]) {
    if (uVar7 < puVar1[1] - *puVar1) {
      iVar3 = DAT_0042d968;
      if ((uint)(DAT_0042d99c + DAT_0042d964) < DAT_0042d998) {
        iVar3 = DAT_0042d964;
      }
      iVar6 = DAT_0042d968;
      if ((uint)(DAT_0042d99c + DAT_0042d964) < DAT_0042d998) {
        iVar6 = DAT_0042d964;
      }
      if (puVar1[1] < (iVar3 + -1 + uVar7 & ~(iVar6 - 1U)) + *puVar1) {
        uVar7 = puVar1[1];
      }
      else {
        iVar3 = DAT_0042d968;
        if ((uint)(DAT_0042d99c + DAT_0042d964) < DAT_0042d998) {
          iVar3 = DAT_0042d964;
        }
        iVar6 = DAT_0042d968;
        if ((uint)(DAT_0042d99c + DAT_0042d964) < DAT_0042d998) {
          iVar6 = DAT_0042d964;
        }
        uVar7 = (iVar3 + -1 + uVar7 & ~(iVar6 - 1U)) + *puVar1;
      }
      iVar3 = FUN_0041d92c((LPVOID)(*puVar1 + (int)puVar1),uVar7 - *puVar1);
      if (iVar3 != 0) {
        DAT_0042d99c = DAT_0042d99c + (uVar7 - *puVar1);
        FUN_0041dc68(puVar1,uVar7);
        return 0;
      }
      iVar3 = FUN_0041d92c((LPVOID)(*puVar1 + (int)puVar1),0x1000);
      if (iVar3 != 0) {
        DAT_0042d99c = DAT_0042d99c + 0x1000;
        FUN_0041dc68(puVar1,*puVar1 + 0x1000);
        return 0;
      }
      return 0xffffffff;
    }
  }
  uVar4 = DAT_0042d960;
  if (DAT_0042d960 <= uVar7 + 0x1000) {
    uVar4 = uVar7 + 0x1000;
  }
  bVar2 = FUN_0041d898(uVar4,&local_8,&local_c);
  if (CONCAT31(extraout_var,bVar2) == 0) {
    uVar5 = 0xffffffff;
  }
  else {
    iVar3 = DAT_0042d968;
    if ((uint)(DAT_0042d99c + DAT_0042d964) < DAT_0042d998) {
      iVar3 = DAT_0042d964;
    }
    iVar6 = DAT_0042d968;
    if ((uint)(DAT_0042d99c + DAT_0042d964) < DAT_0042d998) {
      iVar6 = DAT_0042d964;
    }
    uVar7 = uVar7 + iVar3 + 0xfff & ~(iVar6 - 1U);
    for (puVar1 = DAT_0042d97c; puVar1 != (uint *)0x0; puVar1 = (uint *)puVar1[0x23]) {
      if (((uint *)(puVar1[1] + (int)puVar1) == local_8) && ((int)puVar1[2] < 0x20)) {
        iVar3 = puVar1[1] - *puVar1;
        if (iVar3 != 0) {
          iVar6 = FUN_0041d92c((LPVOID)(*puVar1 + (int)puVar1),iVar3);
          if (iVar6 == 0) {
            return 0xffffffff;
          }
          DAT_0042d99c = DAT_0042d99c + iVar3;
          FUN_0041dc68(puVar1,puVar1[1]);
        }
        iVar6 = FUN_0041d92c(local_8,uVar7 - iVar3);
        if (iVar6 != 0) {
          DAT_0042d99c = DAT_0042d99c + (uVar7 - iVar3);
          uVar4 = puVar1[2];
          puVar1[2] = puVar1[2] + 1;
          puVar1[uVar4 + 3] = (uint)local_8;
          puVar1[1] = puVar1[1] + local_c;
          FUN_0041dc68(puVar1,(uVar7 + *puVar1) - iVar3);
          return 0;
        }
        return 0xffffffff;
      }
    }
    iVar3 = FUN_0041d92c(local_8,uVar7);
    if (iVar3 == 0) {
      FUN_0041d990(local_8);
      uVar5 = 0xffffffff;
    }
    else {
      DAT_0042d99c = DAT_0042d99c + uVar7;
      FUN_0041db68(local_8,uVar7,local_c);
      uVar5 = 0;
    }
  }
  return uVar5;
}



undefined4 __cdecl FUN_0041e018(uint *param_1)

{
  uint uVar1;
  uint *puVar2;
  SIZE_T SVar3;
  uint uVar4;
  
  uVar4 = DAT_0042d978;
  if (DAT_0042d99c < DAT_0042d998) {
    uVar4 = DAT_0042d974;
  }
  if (uVar4 <= *param_1 - 0xc) {
    for (puVar2 = DAT_0042d97c; puVar2 != (uint *)0x0; puVar2 = (uint *)puVar2[0x23]) {
      if ((int)param_1 + (*param_1 & 0xfffffffc) + 4 == (int)puVar2 + (*puVar2 - 4)) {
        uVar4 = *puVar2 - (*param_1 - 0xc & ~(uVar4 - 1));
        FUN_0041dc68(puVar2,uVar4);
        while (uVar4 + (int)puVar2 <= puVar2[puVar2[2] + 2]) {
          puVar2[2] = puVar2[2] - 1;
          SVar3 = *puVar2 - (puVar2[puVar2[2] + 3] - (int)puVar2);
          FUN_0041d978((LPVOID)puVar2[puVar2[2] + 3],SVar3);
          DAT_0042d99c = DAT_0042d99c - SVar3;
          FUN_0041d990((LPVOID)puVar2[puVar2[2] + 3]);
          uVar1 = puVar2[puVar2[2] + 3];
          puVar2[1] = uVar1 - (int)puVar2;
          *puVar2 = uVar1 - (int)puVar2;
        }
        FUN_0041d978((LPVOID)(uVar4 + (int)puVar2),*puVar2 - uVar4);
        DAT_0042d99c = DAT_0042d99c - (*puVar2 - uVar4);
        *puVar2 = uVar4;
        return 1;
      }
    }
  }
  return 0;
}



void __cdecl FUN_0041e10c(int param_1)

{
  if (param_1 != 0) {
    if (0xfffff < (*(uint *)(param_1 + -4) & 0xfffffffc)) {
      FUN_0041d990((LPVOID)(param_1 + -4));
      return;
    }
    FUN_0041e138(param_1);
  }
  return;
}



void __cdecl FUN_0041e138(int param_1)

{
  int iVar1;
  uint *puVar2;
  uint uVar3;
  uint *puVar4;
  uint uVar5;
  
  if (param_1 != 0) {
    puVar2 = (uint *)(param_1 + -4);
    uVar3 = *puVar2;
    if ((uVar3 & 2) == 0) {
      *puVar2 = *puVar2 | 1;
    }
    else {
      puVar2 = (uint *)((int)puVar2 - *(int *)(param_1 + -8));
      *puVar2 = *puVar2 + (uVar3 & 0xfffffffc) + 4;
      if (puVar2 == (uint *)PTR_DAT_0042d994) {
        PTR_DAT_0042d994 = *(undefined **)(PTR_DAT_0042d994 + 4);
      }
      uVar3 = puVar2[1];
      *(uint *)(uVar3 + 8) = puVar2[2];
      *(uint *)(puVar2[2] + 4) = uVar3;
    }
    uVar3 = *puVar2 & 0xfffffffc;
    puVar4 = (uint *)((int)puVar2 + uVar3 + 4);
    if ((*puVar4 & 1) != 0) {
      *puVar2 = *puVar2 + (*puVar4 & 0xfffffffc) + 4;
      if (puVar4 == (uint *)PTR_DAT_0042d994) {
        PTR_DAT_0042d994 = *(undefined **)((int)puVar2 + uVar3 + 8);
      }
      iVar1 = *(int *)((int)puVar2 + uVar3 + 8);
      *(undefined4 *)(iVar1 + 8) = *(undefined4 *)((int)puVar2 + uVar3 + 0xc);
      *(int *)(*(int *)((int)puVar2 + uVar3 + 0xc) + 4) = iVar1;
    }
    puVar4 = (uint *)((int)puVar2 + (*puVar2 & 0xfffffffc) + 4);
    *puVar4 = *puVar4 | 2;
    uVar3 = *puVar2 & 0xfffffffc;
    if (uVar3 < DAT_0042d970) {
      uVar5 = (uVar3 * 2 + DAT_0042d984) - 0xc;
    }
    else {
      uVar5 = *(uint *)(PTR_DAT_0042d994 + 4);
    }
    puVar2[1] = *(uint *)(uVar5 + 4);
    puVar2[2] = uVar5;
    *(uint **)(puVar2[1] + 8) = puVar2;
    *(uint **)(uVar5 + 4) = puVar2;
    *(uint *)((int)puVar2 + uVar3) = uVar3 + 4;
    if (*(int *)((int)puVar2 + (*puVar2 & 0xfffffffc) + 4) == 2) {
      uVar3 = DAT_0042d978;
      if (DAT_0042d99c < DAT_0042d998) {
        uVar3 = DAT_0042d974;
      }
      if (uVar3 < *puVar2) {
        FUN_0041e018(puVar2);
      }
    }
  }
  return;
}



int * __cdecl FUN_0041e240(int param_1)

{
  bool bVar1;
  undefined3 extraout_var;
  uint uVar2;
  int *local_8;
  
  local_8 = (int *)0x0;
  uVar2 = param_1 + 0x1003U & 0xfffff000;
  bVar1 = FUN_0041d904(&local_8,uVar2);
  if (CONCAT31(extraout_var,bVar1) != 0) {
    *local_8 = uVar2 - 4;
    local_8 = local_8 + 1;
  }
  return local_8;
}



uint * __cdecl FUN_0041e27c(uint param_1)

{
  uint *puVar1;
  
  if (param_1 == 0) {
    return (uint *)0x0;
  }
  if (0xfffff < param_1) {
    puVar1 = (uint *)FUN_0041e240(param_1);
    return puVar1;
  }
  puVar1 = FUN_0041e2a4(param_1);
  return puVar1;
}



uint * __cdecl FUN_0041e2a4(uint param_1)

{
  undefined *puVar1;
  uint *puVar2;
  int iVar3;
  uint *puVar4;
  int *piVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  
  if (param_1 == 0) {
    puVar2 = (uint *)0x0;
  }
  else {
    if (param_1 < 0xc) {
      uVar6 = 0xc;
    }
    else {
      uVar6 = param_1 + 3 & 0xfffffffc;
    }
    if (DAT_0042d984 == 0) {
      FUN_0041dd54(1);
    }
    puVar1 = PTR_DAT_0042d994;
    if (uVar6 < DAT_0042d970) {
      iVar3 = uVar6 * 2 + DAT_0042d984;
      puVar4 = *(uint **)(iVar3 + -8);
      if ((uint *)(iVar3 + -0xc) != puVar4) {
        *puVar4 = *puVar4 & 0xfffffffe;
        puVar2 = (uint *)((int)puVar4 + (*puVar4 & 0xfffffffc) + 4);
        *puVar2 = *puVar2 & 0xfffffffd;
        uVar6 = puVar4[1];
        *(uint *)(uVar6 + 8) = puVar4[2];
        *(uint *)(puVar4[2] + 4) = uVar6;
        return puVar4 + 1;
      }
      puVar4 = (uint *)PTR_DAT_0042d98c;
      if ((undefined4 *)PTR_DAT_0042d98c == &DAT_0042d988) {
        for (iVar3 = uVar6 * 2 + DAT_0042d984 + -4; iVar3 == *(int *)(iVar3 + 4); iVar3 = iVar3 + 8)
        {
        }
        puVar4 = (uint *)PTR_DAT_0042d994;
        if (*(uint **)(iVar3 + 4) != (uint *)0x0) {
          puVar4 = *(uint **)(iVar3 + 4);
        }
      }
    }
    else {
      uVar8 = *(uint *)PTR_DAT_0042d994;
      puVar4 = (uint *)PTR_DAT_0042d994;
      if (uVar8 < uVar6) {
        *(undefined4 *)PTR_DAT_0042d994 = 0xfffffffd;
        for (puVar4 = *(uint **)(puVar1 + 4); *puVar4 < uVar6; puVar4 = (uint *)puVar4[1]) {
        }
        *(uint *)PTR_DAT_0042d994 = uVar8;
        if (puVar4 == (uint *)PTR_DAT_0042d994) {
          puVar4 = &DAT_0042d988;
        }
      }
    }
    if (puVar4 == &DAT_0042d988) {
      iVar3 = FUN_0041dd54(param_1 + 0x40);
      if (iVar3 == 0) {
        puVar2 = FUN_0041e2a4(param_1);
      }
      else {
        puVar2 = (uint *)0x0;
      }
    }
    else {
      uVar8 = *puVar4;
      uVar7 = (uVar8 & 0xfffffffc) - uVar6;
      if (uVar7 < 0x10) {
        *puVar4 = *puVar4 & 0xfffffffe;
        puVar2 = (uint *)((int)puVar4 + (*puVar4 & 0xfffffffc) + 4);
        *puVar2 = *puVar2 & 0xfffffffd;
        if (DAT_0042d970 <= (uVar8 & 0xfffffffc)) {
          PTR_DAT_0042d994 = (undefined *)puVar4[1];
        }
        uVar6 = puVar4[1];
        *(uint *)(uVar6 + 8) = puVar4[2];
        puVar2 = puVar4 + 1;
        *(uint *)(puVar4[2] + 4) = uVar6;
      }
      else {
        uVar8 = uVar7 - 4;
        *puVar4 = uVar6;
        piVar5 = (int *)((int)puVar4 + uVar6 + 4);
        *piVar5 = uVar7 - 3;
        *(uint *)((int)piVar5 + uVar8) = uVar7;
        if (uVar8 < DAT_0042d970) {
          iVar3 = uVar8 * 2 + DAT_0042d984;
          *(undefined4 *)((int)puVar4 + uVar6 + 8) = *(undefined4 *)(iVar3 + -8);
          *(int *)((int)puVar4 + uVar6 + 0xc) = iVar3 + -0xc;
          *(int **)(*(int *)((int)puVar4 + uVar6 + 8) + 8) = piVar5;
          *(int **)(iVar3 + -8) = piVar5;
          if (puVar4 == (uint *)PTR_DAT_0042d994) {
            PTR_DAT_0042d994 = (undefined *)puVar4[1];
          }
          uVar6 = puVar4[1];
          *(uint *)(uVar6 + 8) = puVar4[2];
          *(uint *)(puVar4[2] + 4) = uVar6;
        }
        else {
          uVar8 = puVar4[2];
          *(int **)(uVar8 + 4) = piVar5;
          *(uint *)((int)puVar4 + uVar6 + 0xc) = uVar8;
          uVar8 = puVar4[1];
          *(int **)(uVar8 + 8) = piVar5;
          *(uint *)((int)puVar4 + uVar6 + 8) = uVar8;
          PTR_DAT_0042d994 = (undefined *)piVar5;
        }
        puVar2 = puVar4 + 1;
      }
    }
  }
  return puVar2;
}



SIZE_T __stdcall FUN_0041e47c(void)

{
  _MEMORYSTATUS local_20;
  
  local_20.dwLength = 0x20;
  GlobalMemoryStatus(&local_20);
  return local_20.dwAvailPhys;
}



void __stdcall FUN_0041e494(void)

{
  int *piVar1;
  LPVOID pvVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  int *piVar6;
  int iVar7;
  
  piVar1 = DAT_0042d97c;
  while (piVar5 = piVar1, piVar5 != (int *)0x0) {
    piVar1 = (int *)piVar5[0x23];
    piVar5[2] = piVar5[2] + -1;
    iVar7 = piVar5[2];
    piVar6 = piVar5 + iVar7 + 3;
    for (; -1 < iVar7; iVar7 = iVar7 + -1) {
      pvVar2 = (LPVOID)*piVar6;
      iVar3 = *piVar5;
      iVar4 = *piVar6;
      piVar5[1] = iVar4 - (int)piVar5;
      *piVar5 = iVar4 - (int)piVar5;
      FUN_0041d978(pvVar2,iVar3 - ((int)pvVar2 - (int)piVar5));
      FUN_0041d990(pvVar2);
      piVar6 = piVar6 + -1;
    }
  }
  return;
}



int __cdecl FUN_0041e4f4(int param_1,uint param_2)

{
  uint uVar1;
  uint *puVar2;
  uint uVar3;
  uint *puVar4;
  int *piVar5;
  uint uVar6;
  int local_8;
  
  puVar4 = (uint *)(param_1 + -4);
  uVar1 = *puVar4 & 0xfffffffc;
  puVar2 = (uint *)((int)puVar4 + uVar1 + 4);
  if (param_2 < 0xc) {
    uVar3 = 0xc;
  }
  else {
    uVar3 = param_2 + 3 & 0xfffffffc;
  }
  uVar6 = *puVar4 & 0xfffffffc;
  if (uVar3 != uVar6) {
    if ((*puVar2 & 1) != 0) {
      uVar6 = uVar6 + (*puVar2 & 0xfffffffc) + 4;
    }
    if (uVar6 < uVar3) {
      param_1 = 0;
    }
    else {
      local_8 = 0;
      if ((*puVar2 & 1) != 0) {
        if (DAT_0042d970 <= (*puVar2 & 0xfffffffc)) {
          local_8 = *(int *)((int)puVar4 + uVar1 + 0xc);
        }
        if (puVar2 == (uint *)PTR_DAT_0042d994) {
          PTR_DAT_0042d994 = *(undefined **)((int)puVar4 + uVar1 + 8);
        }
        *(undefined4 *)(*(int *)((int)puVar4 + uVar1 + 8) + 8) =
             *(undefined4 *)((int)puVar4 + uVar1 + 0xc);
        *(undefined4 *)(*(int *)((int)puVar4 + uVar1 + 0xc) + 4) =
             *(undefined4 *)((int)puVar4 + uVar1 + 8);
        puVar2 = (uint *)((int)puVar4 + (*puVar2 & 0xfffffffc) + uVar1 + 8);
        *puVar2 = *puVar2 & 0xfffffffd;
        *puVar4 = (*puVar4 & 2) + uVar6;
      }
      uVar6 = uVar6 - uVar3;
      if (0xf < uVar6) {
        uVar1 = uVar6 - 4;
        uVar3 = (*puVar4 & 2) + uVar3;
        *puVar4 = uVar3;
        uVar3 = uVar3 & 0xfffffffc;
        piVar5 = (int *)((int)puVar4 + uVar3 + 4);
        *piVar5 = uVar6 - 3;
        puVar2 = (uint *)((int)puVar4 + uVar6 + 4 + uVar3);
        *puVar2 = *puVar2 | 2;
        if (uVar1 < DAT_0042d970) {
          local_8 = uVar1 * 2 + DAT_0042d984 + -0xc;
        }
        else if (local_8 == 0) {
          local_8 = *(int *)(PTR_DAT_0042d994 + 4);
        }
        *(undefined4 *)((int)puVar4 + uVar3 + 8) = *(undefined4 *)(local_8 + 4);
        *(int *)((int)puVar4 + uVar3 + 0xc) = local_8;
        *(int **)(*(int *)((int)puVar4 + uVar3 + 8) + 8) = piVar5;
        *(int **)(local_8 + 4) = piVar5;
        *(uint *)((int)piVar5 + uVar1) = uVar6;
      }
    }
  }
  return param_1;
}



uint * __cdecl FUN_0041e61c(uint *param_1,uint param_2)

{
  uint *puVar1;
  uint uVar2;
  uint uVar3;
  
  uVar2 = param_2 + 0x1003 & 0xfffff000;
  uVar3 = param_1[-1] & 0xfffffffc;
  puVar1 = param_1;
  if (uVar3 != uVar2 - 4) {
    if (uVar3 < uVar2 - 4) {
      puVar1 = FUN_0041e27c(param_2);
      if (puVar1 != (uint *)0x0) {
        BCB_memcpy(puVar1,param_1,uVar3);
        FUN_0041e10c((int)param_1);
      }
    }
    else if (uVar2 - 4 < 0x100000) {
      puVar1 = FUN_0041e27c(param_2);
      if (puVar1 != (uint *)0x0) {
        BCB_memcpy(puVar1,param_1,param_2);
        FUN_0041e10c((int)param_1);
      }
    }
    else {
      FUN_0041d978((LPVOID)((int)param_1 + (uVar2 - 4)),(uVar3 + 4) - uVar2);
      param_1[-1] = uVar2 - 4;
    }
  }
  return puVar1;
}



uint * __cdecl FUN_0041e6d4(uint *param_1,uint param_2)

{
  uint *puVar1;
  int iVar2;
  
  if (param_2 == 0) {
    FUN_0041e10c((int)param_1);
    puVar1 = (uint *)0x0;
  }
  else if (param_1 == (uint *)0x0) {
    puVar1 = FUN_0041e27c(param_2);
  }
  else if ((param_1[-1] & 0xfffffffc) < 0x100000) {
    iVar2 = FUN_0041e4f4((int)param_1,param_2);
    puVar1 = param_1;
    if (iVar2 == 0) {
      puVar1 = FUN_0041e27c(param_2);
      if (puVar1 != (uint *)0x0) {
        if ((param_1[-1] & 0xfffffffc) <= param_2) {
          param_2 = param_1[-1] & 0xfffffffc;
        }
        FUN_0041e834(puVar1,param_1,param_2);
        FUN_0041e10c((int)param_1);
      }
    }
  }
  else {
    puVar1 = FUN_0041e61c(param_1,param_2);
  }
  return puVar1;
}



uint __cdecl FUN_0041e76c(int param_1)

{
  if (param_1 == 0) {
    return 0;
  }
  return *(uint *)(param_1 + -4) & 0xfffffffc;
}



void __stdcall FUN_0041e788(void)

{
  return;
}



void __stdcall FUN_0041e78c(void)

{
  return;
}



void __stdcall FUN_0041e790(void)

{
  return;
}



void __cdecl FUN_0041e794(undefined4 *param_1,byte param_2)

{
  if ((param_1 != (undefined4 *)0x0) && (*param_1 = &PTR_FUN_0042d814, (param_2 & 1) != 0)) {
    BCB_FreeObject();
  }
  return;
}



void __cdecl FUN_0041e7b4(undefined4 *param_1,byte param_2)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_28;
  
  BCB_InitExceptionHandler(0x42da20);
  if (param_1 != (undefined4 *)0x0) {
    *param_1 = &PTR_FUN_0042d828;
    FUN_0041e794(param_1,0);
    if ((param_2 & 1) != 0) {
      BCB_FreeObject();
    }
  }
  *in_FS_OFFSET = local_28;
  return;
}



undefined4 * __cdecl BCB_memcpy(undefined4 *param_1,undefined4 *param_2,uint param_3)

{
  uint uVar1;
  undefined4 *puVar2;
  
  puVar2 = param_1;
  for (uVar1 = param_3 >> 2; uVar1 != 0; uVar1 = uVar1 - 1) {
    *puVar2 = *param_2;
    param_2 = param_2 + 1;
    puVar2 = puVar2 + 1;
  }
  for (uVar1 = param_3 & 3; uVar1 != 0; uVar1 = uVar1 - 1) {
    *(undefined1 *)puVar2 = *(undefined1 *)param_2;
    param_2 = (undefined4 *)((int)param_2 + 1);
    puVar2 = (undefined4 *)((int)puVar2 + 1);
  }
  return param_1;
}



undefined4 * __cdecl FUN_0041e834(undefined4 *param_1,undefined4 *param_2,uint param_3)

{
  uint uVar1;
  undefined1 *puVar2;
  undefined4 *puVar3;
  undefined1 *puVar4;
  undefined4 *puVar5;
  
  if (param_1 < param_2) {
    puVar3 = param_1;
    for (uVar1 = param_3 >> 2; uVar1 != 0; uVar1 = uVar1 - 1) {
      *puVar3 = *param_2;
      param_2 = param_2 + 1;
      puVar3 = puVar3 + 1;
    }
    for (uVar1 = param_3 & 3; uVar1 != 0; uVar1 = uVar1 - 1) {
      *(undefined1 *)puVar3 = *(undefined1 *)param_2;
      param_2 = (undefined4 *)((int)param_2 + 1);
      puVar3 = (undefined4 *)((int)puVar3 + 1);
    }
  }
  else if (param_1 != param_2) {
    puVar2 = (undefined1 *)((int)param_2 + (param_3 - 1));
    puVar4 = (undefined1 *)((int)param_1 + (param_3 - 1));
    for (uVar1 = param_3 & 3; uVar1 != 0; uVar1 = uVar1 - 1) {
      *puVar4 = *puVar2;
      puVar2 = puVar2 + -1;
      puVar4 = puVar4 + -1;
    }
    puVar3 = (undefined4 *)(puVar2 + -3);
    puVar5 = (undefined4 *)(puVar4 + -3);
    for (uVar1 = param_3 >> 2; uVar1 != 0; uVar1 = uVar1 - 1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + -1;
      puVar5 = puVar5 + -1;
    }
  }
  return param_1;
}



undefined4 * __cdecl FUN_0041e880(undefined4 *param_1,undefined1 param_2,uint param_3)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  undefined2 uVar3;
  byte bVar4;
  uint uVar5;
  undefined4 *puVar6;
  
  uVar3 = CONCAT11(param_2,param_2);
  if ((param_3 & 0xfffffffc) == 0) {
    if ((param_3 & 3) != 0) {
      *(undefined1 *)param_1 = param_2;
      bVar4 = (char)(param_3 & 3) - 1;
      if (bVar4 != 0) {
        *(undefined2 *)((int)param_1 + (bVar4 - 1)) = uVar3;
      }
    }
    return param_1;
  }
  *(undefined2 *)param_1 = uVar3;
  puVar1 = (undefined4 *)((int)param_1 + (param_3 - 4));
  *(undefined2 *)((int)param_1 + 2) = uVar3;
  uVar2 = *param_1;
  uVar5 = param_3 >> 3;
  puVar6 = param_1;
  if (uVar5 == 0) {
    *puVar1 = uVar2;
    return param_1;
  }
  do {
    *puVar6 = uVar2;
    puVar6[1] = uVar2;
    if (uVar5 == 1) break;
    puVar6[2] = uVar2;
    puVar6[3] = uVar2;
    if (uVar5 == 2) break;
    puVar6[4] = uVar2;
    puVar6[5] = uVar2;
    if (uVar5 == 3) break;
    puVar6[6] = uVar2;
    puVar6[7] = uVar2;
    if (uVar5 == 4) break;
    puVar6[8] = uVar2;
    puVar6[9] = uVar2;
    if (uVar5 == 5) break;
    puVar6[10] = uVar2;
    puVar6[0xb] = uVar2;
    puVar6 = puVar6 + 0xc;
    uVar5 = uVar5 - 6;
  } while (uVar5 != 0);
  *puVar1 = uVar2;
  puVar1[-1] = uVar2;
  return param_1;
}



undefined2 * __cdecl FUN_0041e90c(undefined2 *param_1,undefined2 param_2,int param_3)

{
  undefined2 *puVar1;
  
  puVar1 = param_1;
  for (; param_3 != 0; param_3 = param_3 + -1) {
    *puVar1 = param_2;
    puVar1 = puVar1 + 1;
  }
  return param_1;
}



char * __cdecl FUN_0041e924(uint *param_1)

{
  uint uVar1;
  uint *puVar2;
  uint uVar3;
  
  puVar2 = param_1;
  if (((uint)param_1 & 3) == 0) {
LAB_0041e92c:
    do {
      do {
        uVar1 = *puVar2;
        puVar2 = puVar2 + 1;
        uVar3 = uVar1 + 0xfefefeff & 0x80808080;
      } while (uVar3 == 0);
      uVar3 = uVar3 & ~uVar1;
    } while (uVar3 == 0);
    if ((char)uVar3 == '\0') {
      if ((char)(uVar3 >> 8) == '\0') {
        if ((uVar3 & 0xff0000) == 0) goto LAB_0041e976;
        goto LAB_0041e975;
      }
    }
    else {
LAB_0041e973:
      puVar2 = (uint *)((int)puVar2 + -1);
    }
LAB_0041e974:
    puVar2 = (uint *)((int)puVar2 + -1);
  }
  else {
    puVar2 = param_1 + 1;
    if ((char)*param_1 == '\0') goto LAB_0041e973;
    if (*(char *)((int)param_1 + 1) == '\0') goto LAB_0041e974;
    if (*(char *)((int)param_1 + 2) != '\0') {
      puVar2 = (uint *)((int)param_1 + 3U & 0xfffffffc);
      goto LAB_0041e92c;
    }
  }
LAB_0041e975:
  puVar2 = (uint *)((int)puVar2 + -1);
LAB_0041e976:
  return (char *)((int)puVar2 + (-1 - (int)param_1));
}



char * __cdecl FUN_0041e980(char *param_1,char *param_2,uint param_3)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  char *pcVar4;
  char *pcVar5;
  
  iVar2 = -1;
  pcVar5 = param_1;
  do {
    pcVar4 = pcVar5;
    if (iVar2 == 0) break;
    iVar2 = iVar2 + -1;
    pcVar4 = pcVar5 + 1;
    cVar1 = *pcVar5;
    pcVar5 = pcVar4;
  } while (cVar1 != '\0');
  pcVar4 = pcVar4 + -1;
  for (uVar3 = param_3 & 3; uVar3 != 0; uVar3 = uVar3 - 1) {
    cVar1 = *param_2;
    *pcVar4 = cVar1;
    if (cVar1 == '\0') {
      return param_1;
    }
    param_2 = param_2 + 1;
    pcVar4 = pcVar4 + 1;
  }
  uVar3 = param_3 >> 2;
  while( true ) {
    if (uVar3 == 0) {
      *pcVar4 = '\0';
      return param_1;
    }
    cVar1 = *param_2;
    *pcVar4 = cVar1;
    if (cVar1 == '\0') {
      return param_1;
    }
    cVar1 = param_2[1];
    pcVar4[1] = cVar1;
    if (cVar1 == '\0') {
      return param_1;
    }
    cVar1 = param_2[2];
    pcVar4[2] = cVar1;
    if (cVar1 == '\0') break;
    cVar1 = param_2[3];
    param_2 = param_2 + 4;
    pcVar4[3] = cVar1;
    pcVar4 = pcVar4 + 4;
    if (cVar1 == '\0') {
      return param_1;
    }
    uVar3 = uVar3 - 1;
  }
  return param_1;
}



int __cdecl FUN_0041e9f0(byte *param_1,byte *param_2,int param_3)

{
  byte bVar1;
  byte bVar2;
  int iVar3;
  
  while (3 < param_3) {
    if ((((*param_2 != *param_1) || (param_2[1] != param_1[1])) || (param_2[2] != param_1[2])) ||
       (param_2[3] != param_1[3])) break;
    param_3 = param_3 + -4;
    param_1 = param_1 + 4;
    param_2 = param_2 + 4;
    if (param_3 < 4) break;
  }
  if (param_3 == 0) {
    iVar3 = 0;
  }
  else {
    do {
      bVar1 = *param_1;
      bVar2 = *param_2;
      if (bVar2 != bVar1) break;
      param_1 = param_1 + 1;
      param_2 = param_2 + 1;
      param_3 = param_3 + -1;
    } while (param_3 != 0);
    iVar3 = (uint)bVar1 - (uint)bVar2;
  }
  return iVar3;
}



int __cdecl FUN_0041ea5c(short *param_1)

{
  int iVar1;
  
  iVar1 = 0;
  for (; *param_1 != 0; param_1 = param_1 + 1) {
    iVar1 = iVar1 + 1;
  }
  return iVar1;
}



short * __cdecl FUN_0041ea74(short *param_1,short *param_2)

{
  uint uVar1;
  uint uVar2;
  short *psVar3;
  
  uVar1 = FUN_0041ea5c(param_2);
  uVar2 = 0;
  psVar3 = param_1;
  if (uVar1 != 0) {
    do {
      *psVar3 = *param_2;
      uVar2 = uVar2 + 1;
      psVar3 = psVar3 + 1;
      param_2 = param_2 + 1;
    } while (uVar2 < uVar1);
  }
  param_1[uVar1] = 0;
  return param_1;
}



void BCB_InitExceptionHandler(int param_1)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int unaff_EBP;
  undefined2 in_FS;
  
  puVar2 = (undefined4 *)(unaff_EBP + *(int *)(param_1 + 4));
  puVar2[2] = param_1;
  puVar2[3] = &stack0x00000004;
  puVar2[1] = &LAB_0041eafb;
  *(undefined2 *)(puVar2 + 4) = 0;
  *(undefined2 *)((int)puVar2 + 0x12) = 0;
  puVar2[7] = 0;
  puVar1 = (undefined4 *)segment(in_FS,0);
  *puVar2 = *puVar1;
  puVar1 = (undefined4 *)segment(in_FS,0);
  *puVar1 = puVar2;
  return;
}



undefined4 __cdecl FUN_0041eb24(int param_1,int *param_2,undefined4 param_3,int param_4)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  undefined4 *puVar4;
  
  if ((*(byte *)(param_2 + 1) & 1) == 0) {
    FUN_00423084(s_IS_STRUC_base_>tpMask__0042daab,s_xxtype_cpp_0042dac2,0x13b);
  }
  if ((*(byte *)(param_1 + 4) & 1) == 0) {
    FUN_00423084(s_IS_STRUC_derv_>tpMask__0042dacd,s_xxtype_cpp_0042dae4,0x13c);
  }
  if ((*(byte *)(param_1 + 0xc) & 4) == 0) {
    FUN_00423084(s_derv_>tpClass_tpcFlags___CF_HAS__0042daef,s_xxtype_cpp_0042db15,0x13e);
  }
  puVar4 = (undefined4 *)((uint)*(ushort *)(param_1 + 0x12) + param_1);
  do {
    piVar1 = (int *)*puVar4;
    if (piVar1 == (int *)0x0) {
      puVar4 = (undefined4 *)((uint)*(ushort *)(param_1 + 0x10) + param_1);
      while( true ) {
        piVar1 = (int *)*puVar4;
        if (piVar1 == (int *)0x0) {
          return 0;
        }
        if ((param_4 == 0) || ((puVar4[2] & 3) != 3)) {
          iVar3 = 0;
        }
        else {
          iVar3 = 1;
        }
        iVar2 = FUN_0041f010(param_2,piVar1);
        if ((iVar2 != 0) && (iVar3 != 0)) break;
        if (((*(byte *)(piVar1 + 3) & 4) != 0) &&
           (iVar3 = FUN_0041eb24((int)piVar1,param_2,param_3,iVar3), iVar3 != 0)) {
          return 1;
        }
        puVar4 = puVar4 + 3;
      }
      return 1;
    }
    if ((*(byte *)(puVar4 + 2) & 8) == 0) {
      if ((param_4 == 0) || ((puVar4[2] & 3) != 3)) {
        iVar3 = 0;
      }
      else {
        iVar3 = 1;
      }
      iVar2 = FUN_0041f010(param_2,piVar1);
      if ((iVar2 != 0) && (iVar3 != 0)) {
        return 1;
      }
      if (((*(byte *)(piVar1 + 3) & 4) != 0) &&
         (iVar3 = FUN_0041eb24((int)piVar1,param_2,param_3,iVar3), iVar3 != 0)) {
        return 1;
      }
    }
    puVar4 = puVar4 + 3;
  } while( true );
}



void __stdcall FUN_0041ec7b(void)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined4 *in_FS_OFFSET;
  undefined4 local_28;
  
  BCB_InitExceptionHandler(0x42da38);
  iVar3 = 0;
  do {
    puVar1 = (undefined4 *)(&DAT_00430edc)[iVar3];
    if ((undefined4 *)(&DAT_00430edc)[iVar3] != (undefined4 *)0x0) {
      while (puVar2 = puVar1, puVar2 != (undefined4 *)0x0) {
        puVar1 = (undefined4 *)puVar2[3];
        if (puVar2 != (undefined4 *)0x0) {
          (**(code **)*puVar2)();
        }
      }
    }
    iVar3 = iVar3 + 1;
  } while (iVar3 < 0x100);
  *in_FS_OFFSET = local_28;
  return;
}



undefined4 * __cdecl
FUN_0041ecce(int param_1,int *param_2,undefined4 *param_3,int *param_4,int param_5,int *param_6,
            uint *param_7,int param_8)

{
  int *piVar1;
  bool bVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  uint local_18;
  int local_14;
  undefined4 *local_10;
  uint local_c;
  
  local_c = 0;
  local_10 = (undefined4 *)0x0;
  local_14 = 0;
  if ((param_2 == (int *)0x0) || ((*(byte *)(param_2 + 1) & 1) == 0)) {
    FUN_00423084(s_topTypPtr____0____IS_STRUC_topTy_0042db5b,s_xxtype_cpp_0042db89,0x34f);
  }
  if ((param_4 == (int *)0x0) || ((*(byte *)(param_4 + 1) & 1) == 0)) {
    FUN_00423084(s_tgtTypPtr____0____IS_STRUC_tgtTy_0042db94,s_xxtype_cpp_0042dbc2,0x350);
  }
  if ((param_6 != (int *)0x0) && ((*(byte *)(param_6 + 1) & 1) == 0)) {
    FUN_00423084(s_srcTypPtr____0____IS_STRUC_srcTy_0042dbcd,s_xxtype_cpp_0042dbfb,0x351);
  }
  if (param_6 != (int *)0x0) {
    iVar3 = FUN_0041f010(param_6,param_4);
    if (iVar3 != 0) {
      FUN_00423084(s___isSameTypeID_srcTypPtr__tgtTyp_0042dc06,s_xxtype_cpp_0042dc30,0x355);
    }
    iVar3 = FUN_0041f010(param_6,param_2);
    if (iVar3 != 0) {
      return (undefined4 *)0x0;
    }
  }
  if ((param_4 == (int *)0x0) || (iVar3 = FUN_0041f010(param_2,param_4), iVar3 != 0)) {
    FUN_00423084(s_tgtTypPtr____0______isSameTypeID_0042dc3b,s_xxtype_cpp_0042dc77,0x35b);
  }
  if ((*(byte *)(param_2 + 3) & 4) == 0) {
    return (undefined4 *)0x0;
  }
  bVar2 = false;
  puVar5 = (undefined4 *)((uint)*(ushort *)((int)param_2 + 0x12) + (int)param_2);
  do {
    while (piVar1 = (int *)*puVar5, piVar1 == (int *)0x0) {
      if (bVar2) {
        *param_7 = local_c;
        if (local_14 == 1) {
          return local_10;
        }
        return (undefined4 *)0x0;
      }
      bVar2 = true;
      puVar5 = (undefined4 *)((uint)*(ushort *)(param_2 + 4) + (int)param_2);
    }
    if ((*(byte *)(puVar5 + 2) & 8) == 0) {
      if ((param_8 == 0) || ((puVar5[2] & 3) != 3)) {
        local_18 = 0;
      }
      else {
        local_18 = 1;
      }
      puVar6 = (undefined4 *)(param_1 + puVar5[1]);
      if ((*(byte *)(puVar5 + 2) & 4) != 0) {
        puVar6 = (undefined4 *)*puVar6;
      }
      iVar3 = FUN_0041f010(param_4,piVar1);
      if (iVar3 == 0) {
        if (((*(byte *)(piVar1 + 3) & 4) != 0) &&
           (puVar6 = FUN_0041ecce((int)puVar6,piVar1,param_3,param_4,param_5,param_6,param_7,
                                  local_18), puVar6 != (undefined4 *)0x0)) {
          local_18 = *param_7;
          goto LAB_0041eed9;
        }
      }
      else if (param_3 == (undefined4 *)0x0) {
        if (param_5 != 0) {
          if (param_6 == (int *)0x0) {
            FUN_00423084(s_srcTypPtr_0042dc82,s_xxtype_cpp_0042dc8c,0x3d0);
          }
          puVar4 = FUN_0041ecce((int)puVar6,piVar1,(undefined4 *)param_5,param_6,0,(int *)0x0,
                                param_7,0);
          if (puVar4 == (undefined4 *)0x0) goto LAB_0041eef8;
        }
LAB_0041eed9:
        if ((local_14 == 0) || (puVar6 != local_10)) {
          local_14 = local_14 + 1;
          local_c = local_18;
          local_10 = puVar6;
        }
        else {
          local_c = local_c | local_18;
        }
      }
      else if (puVar6 == param_3) {
        return puVar6;
      }
    }
LAB_0041eef8:
    puVar5 = puVar5 + 3;
  } while( true );
}



undefined4 __cdecl FUN_0041ef35(undefined4 *param_1,int param_2,int *param_3,int *param_4)

{
  int *piVar1;
  int iVar2;
  undefined4 *local_8;
  
  if (*param_4 == 0) {
    FUN_00423084(&DAT_0042dcc9,s_xxtype_cpp_0042dcce,0x515);
  }
  while( true ) {
    piVar1 = (int *)*param_1;
    if (piVar1 == (int *)0x0) {
      return 0;
    }
    local_8 = (undefined4 *)(*param_4 + param_1[1]);
    if (param_2 != 0) {
      local_8 = (undefined4 *)*local_8;
    }
    iVar2 = FUN_0041f010(piVar1,param_3);
    if (iVar2 != 0) break;
    if ((((*(byte *)(piVar1 + 1) & 2) != 0) &&
        ((undefined4 *)((uint)*(ushort *)(piVar1 + 4) + (int)piVar1) != (undefined4 *)0x0)) &&
       (iVar2 = FUN_0041ef35((undefined4 *)((uint)*(ushort *)(piVar1 + 4) + (int)piVar1),0,param_3,
                             (int *)&local_8), iVar2 != 0)) {
      *param_4 = (int)local_8;
      return 1;
    }
    param_1 = param_1 + 3;
  }
  *param_4 = (int)local_8;
  return 1;
}



char * __cdecl FUN_0041efd8(int param_1)

{
  if (param_1 == 0) {
    return s_<notype>_0042da40;
  }
  if (*(short *)(param_1 + 6) == 0) {
    FUN_00423084(s_id_>tpName_0042da49,s_xxtype_cpp_0042da54,0x22);
  }
  return (char *)((uint)*(ushort *)(param_1 + 6) + param_1);
}



undefined4 __cdecl FUN_0041f010(int *param_1,int *param_2)

{
  char cVar1;
  char cVar2;
  undefined4 uVar3;
  char *pcVar4;
  char *pcVar5;
  
  if (param_1 == (int *)0x0) {
    FUN_00423084(&DAT_0042da5f,s_xxtype_cpp_0042da63,0x105);
  }
  if (param_2 == (int *)0x0) {
    FUN_00423084(&DAT_0042da6e,s_xxtype_cpp_0042da72,0x106);
  }
  if (param_2 == param_1) {
    uVar3 = 1;
  }
  else if (((short)param_1[1] == (short)param_2[1]) && (*param_1 == *param_2)) {
    if (((*(ushort *)(param_1 + 1) | *(ushort *)(param_2 + 1)) & 0x80) == 0) {
      pcVar5 = (char *)((uint)*(ushort *)((int)param_1 + 6) + (int)param_1);
      if (*(short *)((int)param_1 + 6) == 0) {
        FUN_00423084(s_tp1_>tpName_0042da7d,s_xxtype_cpp_0042da89,0x11e);
      }
      pcVar4 = (char *)((uint)*(ushort *)((int)param_2 + 6) + (int)param_2);
      if (*(short *)((int)param_2 + 6) == 0) {
        FUN_00423084(s_tp2_>tpName_0042da94,s_xxtype_cpp_0042daa0,0x11f);
      }
      do {
        cVar1 = *pcVar5;
        cVar2 = *pcVar4;
        pcVar4 = pcVar4 + 1;
        pcVar5 = pcVar5 + 1;
        if (cVar2 != cVar1) {
          return 0;
        }
      } while (cVar1 != '\0');
      uVar3 = 1;
    }
    else {
      uVar3 = 0;
    }
  }
  else {
    uVar3 = 0;
  }
  return uVar3;
}



undefined4 __cdecl FUN_0041f0ec(int *param_1,int *param_2,int param_3,undefined4 param_4)

{
  uint uVar1;
  ushort uVar2;
  ushort uVar3;
  int iVar4;
  undefined4 uVar5;
  uint uVar6;
  
  while( true ) {
    while( true ) {
      if (param_2 == param_1) {
        return 1;
      }
      uVar2 = *(ushort *)(param_1 + 1);
      uVar3 = *(ushort *)(param_2 + 1);
      if ((uVar3 & uVar2 & 0x10) == 0) break;
      uVar6 = uVar2 & 0x300;
      uVar1 = uVar3 & 0x300;
      if (uVar6 != uVar1) {
        if ((param_3 != 0) || (uVar6 == uVar1)) {
          return 0;
        }
        if ((byte)(~(byte)(uVar1 >> 8) & (byte)(uVar6 >> 8)) != 0) {
          return 0;
        }
      }
      param_3 = 1;
      param_1 = (int *)param_1[2];
      param_2 = (int *)param_2[2];
    }
    if ((uVar3 & uVar2 & 0x400) == 0) break;
    if ((param_1[3] != param_2[3]) && ((param_3 != 0 || (param_1[3] != 0 || param_2[3] != 0)))) {
      return 0;
    }
    param_3 = 1;
    param_1 = (int *)param_1[2];
    param_2 = (int *)param_2[2];
  }
  iVar4 = FUN_0041f010(param_1,param_2);
  if (iVar4 != 0) {
    return 1;
  }
  if ((((uVar2 & 2) != 0) && ((uVar3 & 1) != 0)) && ((*(byte *)(param_1 + 3) & 4) != 0)) {
    uVar5 = FUN_0041eb24((int)param_1,param_2,param_4,1);
    return uVar5;
  }
  return 0;
}



int __cdecl FUN_0041f1fc(int param_1,int param_2,int *param_3)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  
  piVar1 = param_3;
  iVar3 = param_2;
  if (((param_1 != 0) &&
      (iVar2 = FUN_0041ef35((undefined4 *)((uint)*(ushort *)(param_2 + 0x10) + param_2),0,param_3,
                            &param_1), iVar2 == 0)) &&
     (iVar3 = FUN_0041ef35((undefined4 *)((uint)*(ushort *)(iVar3 + 0x12) + iVar3),1,piVar1,&param_1
                          ), iVar3 == 0)) {
    FUN_00423084(s___Can_t_adjust_class_address__no_0042dd10,s_xxtype_cpp_0042dd4a,0x57e);
    param_1 = 0;
  }
  return param_1;
}



void __stdcall FUN_0041f278(void)

{
  if (DAT_0042dd58 == (code *)0x0) {
    DAT_0042dd58 = FUN_0041f2a0;
  }
  if (DAT_0042dd5c == (undefined1 *)0x0) {
    DAT_0042dd5c = &LAB_0041f2f0;
  }
  return;
}



void __stdcall FUN_0041f2a0(void)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_28;
  
  BCB_InitExceptionHandler(0x42dd80);
  if (DAT_0042dd58 != FUN_0041f2a0) {
    (*DAT_0042dd58)();
  }
  FUN_004233d8();
  *in_FS_OFFSET = local_28;
  return;
}



void __stdcall FUN_0041f344(void)

{
  int iVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 local_28;
  
  BCB_InitExceptionHandler(0x42de64);
  iVar1 = FUN_00401202();
  *(uint *)(iVar1 + 0x14) = *(uint *)(iVar1 + 0x14) & 0xfffffffd;
  if (*DAT_004312e8 == 2) {
    FUN_004241f9(4,1);
  }
  FUN_0041f848();
  (*DAT_0042dd58)();
  FUN_004233d8();
  *in_FS_OFFSET = local_28;
  return;
}



void __stdcall FUN_0041f3b8(void)

{
  int iVar1;
  
  iVar1 = FUN_00401202();
  *(uint *)(iVar1 + 0x14) = *(uint *)(iVar1 + 0x14) & 0xfffffffd;
  if (*DAT_004312e8 == 2) {
    FUN_004241f9(5,1);
  }
  (*DAT_0042dd5c)();
  FUN_004233d8();
  return;
}



void __cdecl FUN_0041f3f0(undefined4 *param_1)

{
  int iVar1;
  uint uVar2;
  undefined4 local_c;
  undefined4 *local_8;
  
  if (param_1 == (undefined4 *)0x0) {
    FUN_0041f43c(&local_c);
    param_1 = local_8;
  }
  uVar2 = 0x9c;
  iVar1 = FUN_00401202();
  BCB_memcpy((undefined4 *)(iVar1 + 0x10),param_1,uVar2);
  FUN_0041f278();
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __cdecl FUN_0041f43c(undefined4 *param_1)

{
  _DAT_0042dda8 = FUN_00401202();
  _DAT_0042dda8 = _DAT_0042dda8 + 0x2c;
  *param_1 = 0x82727349;
  param_1[1] = &DAT_0042dd94;
  return;
}



undefined4 __stdcall FUN_0041f464(void)

{
  return 1;
}



LONG __cdecl FUN_0041f470(PEXCEPTION_RECORD param_1,PVOID param_2,PCONTEXT param_3)

{
  LONG LVar1;
  int iVar2;
  _EXCEPTION_POINTERS local_c;
  
  if (param_1->ExceptionCode == 0xeedfae6) {
    LVar1 = 0;
  }
  else {
    if ((param_1->ExceptionCode == 0xeefface) || (param_1->ExceptionCode == 0xeedface)) {
      FUN_0041f63a(param_2,param_1);
      FUN_0041f344();
    }
    if ((param_1->ExceptionCode == 0xc00000fd) && (DAT_0042de78 != 0)) {
      FUN_00423320((uint *)s_Stack_Overflow__0042de7c);
    }
    if ((DAT_004312f0 != (code *)0x0) && (iVar2 = (*DAT_004312f0)(), iVar2 == 0)) {
      return 0;
    }
    if ((DAT_004312ec != (code *)0x0) && (iVar2 = (*DAT_004312ec)(), iVar2 == 0)) {
      return 0;
    }
    if (((DAT_00431708 != (int *)0x0) && ((*DAT_00431708 == 1 || (*DAT_00431708 == 2)))) &&
       ((param_1->ExceptionCode < 0xeedface || (0xeefface < param_1->ExceptionCode)))) {
      FUN_004241f9(2,3);
    }
    local_c.ExceptionRecord = param_1;
    local_c.ContextRecord = param_3;
    LVar1 = UnhandledExceptionFilter(&local_c);
  }
  return LVar1;
}



void __cdecl FUN_0041f560(undefined4 *param_1)

{
  *param_1 = 0;
  param_1[1] = FUN_0041f470;
  FUN_0041f5ec(param_1);
  return;
}



void __cdecl FUN_0041f57c(undefined4 *param_1)

{
  FUN_0041f601(param_1);
  return;
}



undefined4 __cdecl FUN_0041f58c(undefined4 param_1)

{
  undefined4 uVar1;
  
  uVar1 = DAT_004312f0;
  DAT_004312f0 = param_1;
  return uVar1;
}



void __cdecl FUN_0041f5ec(undefined4 *param_1)

{
  undefined4 *puVar1;
  undefined2 in_FS;
  
  puVar1 = (undefined4 *)segment(in_FS,0);
  *param_1 = *puVar1;
  puVar1 = (undefined4 *)segment(in_FS,0);
  *puVar1 = param_1;
  return;
}



void __cdecl FUN_0041f601(undefined4 *param_1)

{
  undefined4 *puVar1;
  int *piVar2;
  undefined2 in_FS;
  
  piVar2 = (int *)segment(in_FS,0);
  piVar2 = (int *)*piVar2;
  if (param_1 == piVar2) {
    puVar1 = (undefined4 *)segment(in_FS,0);
    *puVar1 = *param_1;
  }
  else {
    for (; piVar2 != (int *)0xffffffff; piVar2 = (int *)*piVar2) {
      if ((undefined4 *)*piVar2 == param_1) {
        *piVar2 = *param_1;
        return;
      }
    }
  }
  return;
}



void FUN_0041f63a(PVOID param_1,PEXCEPTION_RECORD param_2)

{
  RtlUnwind(param_1,(PVOID)0x41f64b,param_2,(PVOID)0x0);
  return;
}



void __stdcall FUN_0041f64f(void)

{
  undefined4 *unaff_EBX;
  
  (*(code *)*unaff_EBX)();
  return;
}



bool __cdecl FUN_0041f658(uint param_1)

{
  DWORD DVar1;
  
  if (DAT_0042e344 <= param_1) {
    return false;
  }
  DVar1 = GetFileType(*(HANDLE *)(&DAT_004313bc + param_1 * 4));
  return DVar1 == 2;
}



bool __cdecl FUN_0041f688(HANDLE param_1)

{
  DWORD DVar1;
  
  DVar1 = GetFileType(param_1);
  return DVar1 == 2;
}



DWORD __cdecl FUN_0041f6a0(uint param_1,LONG param_2,int param_3)

{
  DWORD DVar1;
  
  if (param_1 < DAT_0042e344) {
    if (param_3 == 0) {
      DVar1 = 0;
    }
    else if (param_3 == 1) {
      DVar1 = 1;
    }
    else {
      if (param_3 != 2) {
        DVar1 = FUN_0041fd20(1);
        return DVar1;
      }
      DVar1 = 2;
    }
    (&DAT_0042e348)[param_1] = (&DAT_0042e348)[param_1] & 0xfffffdff;
    DVar1 = SetFilePointer(*(HANDLE *)(&DAT_004313bc + param_1 * 4),param_2,(PLONG)0x0,DVar1);
    if (DVar1 == 0xffffffff) {
      FUN_0041fd84();
    }
  }
  else {
    DVar1 = FUN_0041fd20(6);
  }
  return DVar1;
}



int __cdecl FUN_0041f718(uint param_1,char *param_2,uint param_3)

{
  char cVar1;
  int iVar2;
  char *pcVar3;
  char *pcVar4;
  char local_20c [512];
  int local_c;
  int local_8;
  
  if (param_1 < DAT_0042e344) {
    if (param_3 + 1 < 2) {
      local_c = 0;
    }
    else {
      if ((*(byte *)((int)&DAT_0042e348 + param_1 * 4 + 1) & 8) != 0) {
        FUN_0041f6a0(param_1,0,2);
      }
      if ((*(byte *)((int)&DAT_0042e348 + param_1 * 4 + 1) & 0x40) == 0) {
        local_c = FUN_0041f880(param_1,param_2,param_3);
      }
      else {
        local_8 = 0;
        local_c = 0;
        pcVar4 = param_2;
        while ((uint)((int)pcVar4 - (int)param_2) < param_3) {
          pcVar3 = local_20c;
          while (((int)pcVar3 - (int)local_20c < 0x1ff &&
                 ((uint)((int)pcVar4 - (int)param_2) < param_3))) {
            cVar1 = *pcVar4;
            pcVar4 = pcVar4 + 1;
            if (cVar1 == '\n') {
              local_8 = local_8 + 1;
              *pcVar3 = '\r';
              pcVar3[1] = '\n';
              pcVar3 = pcVar3 + 2;
            }
            else {
              *pcVar3 = cVar1;
              pcVar3 = pcVar3 + 1;
            }
          }
          iVar2 = FUN_0041f880(param_1,local_20c,(int)pcVar3 - (int)local_20c);
          if (iVar2 != (int)pcVar3 - (int)local_20c) {
            if (iVar2 == -1) {
              return -1;
            }
            return (local_c - local_8) + iVar2;
          }
          local_c = local_c + iVar2;
        }
        local_c = local_c - local_8;
      }
    }
  }
  else {
    local_c = FUN_0041fd20(-6);
  }
  return local_c;
}



undefined4 __cdecl FUN_0041f834(undefined1 *param_1)

{
  if (param_1 != (undefined1 *)0x0) {
    *param_1 = 0;
  }
  return 0;
}



int __stdcall FUN_0041f848(void)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = 0;
  piVar1 = &DAT_0042de94;
  iVar2 = DAT_0042e344;
  while (iVar2 != 0) {
    if (((*(byte *)((int)piVar1 + 0x12) & 3) != 0) && (piVar1[2] != 0)) {
      FUN_0041f964(piVar1);
      iVar3 = iVar3 + 1;
    }
    piVar1 = piVar1 + 6;
    iVar2 = iVar2 + -1;
  }
  return iVar3;
}



undefined4 __cdecl FUN_0041f880(uint param_1,LPCVOID param_2,DWORD param_3)

{
  undefined4 uVar1;
  BOOL BVar2;
  DWORD local_8;
  
  if (DAT_0042e344 <= param_1) {
    uVar1 = FUN_0041fd20(6);
    return uVar1;
  }
  BVar2 = WriteFile(*(HANDLE *)(&DAT_004313bc + param_1 * 4),param_2,param_3,&local_8,
                    (LPOVERLAPPED)0x0);
  if (BVar2 != 1) {
    uVar1 = FUN_0041fd84();
    return uVar1;
  }
  return local_8;
}



void __cdecl FUN_0041f8c8(uint param_1,char *param_2,uint param_3)

{
  FUN_0041f718(param_1,param_2,param_3);
  return;
}



undefined4 __cdecl FUN_0041f8e4(int *param_1,int param_2,int param_3,int param_4)

{
  if ((*(byte *)((int)param_1 + 0x12) & 4) != 0) {
    FUN_0041da5c();
  }
  *(ushort *)((int)param_1 + 0x12) = *(ushort *)((int)param_1 + 0x12) & 0xfff3;
  param_1[3] = 0;
  param_1[1] = (int)(param_1 + 5);
  *param_1 = (int)(param_1 + 5);
  if ((param_3 != 2) && (param_4 != 0)) {
    PTR_FUN_0042f230 = &LAB_00420980;
    if (param_2 == 0) {
      param_2 = FUN_0041da4c();
      if (param_2 == 0) {
        return 0xffffffff;
      }
      *(ushort *)((int)param_1 + 0x12) = *(ushort *)((int)param_1 + 0x12) | 4;
    }
    *param_1 = param_2;
    param_1[1] = param_2;
    param_1[3] = param_4;
    if (param_3 == 1) {
      *(ushort *)((int)param_1 + 0x12) = *(ushort *)((int)param_1 + 0x12) | 8;
    }
  }
  return 0;
}



undefined4 __cdecl FUN_0041f964(int *param_1)

{
  undefined4 uVar1;
  uint uVar2;
  uint uVar3;
  
  if (param_1 == (int *)0x0) {
    FUN_0041f9e4();
    uVar1 = 0;
  }
  else if ((char)param_1 == *(char *)((int)param_1 + 0x17)) {
    if (param_1[2] < 0) {
      uVar3 = param_1[3] + param_1[2] + 1;
      param_1[2] = param_1[2] - uVar3;
      *param_1 = param_1[1];
      uVar2 = FUN_0041f718((int)*(char *)((int)param_1 + 0x16),(char *)param_1[1],uVar3);
      if ((uVar3 == uVar2) || ((*(byte *)((int)param_1 + 0x13) & 2) != 0)) {
        uVar1 = 0;
      }
      else {
        *(ushort *)((int)param_1 + 0x12) = *(ushort *)((int)param_1 + 0x12) | 0x10;
        uVar1 = 0xffffffff;
      }
    }
    else {
      if ((((*(byte *)((int)param_1 + 0x12) & 8) != 0) || (param_1 + 5 == (int *)*param_1)) &&
         (param_1[2] = 0, param_1 + 5 == (int *)*param_1)) {
        *param_1 = param_1[1];
      }
      uVar1 = 0;
    }
  }
  else {
    uVar1 = 0xffffffff;
  }
  return uVar1;
}



int __stdcall FUN_0041f9e4(void)

{
  uint uVar1;
  undefined4 *puVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  
  iVar3 = 0;
  puVar2 = &DAT_0042de94;
  iVar5 = DAT_0042e344;
  while (iVar5 != 0) {
    if ((int)puVar2[2] < 0) {
      uVar4 = puVar2[3] + puVar2[2] + 1;
      puVar2[2] = puVar2[2] - uVar4;
      *puVar2 = (char *)puVar2[1];
      uVar1 = FUN_0041f718((int)*(char *)((int)puVar2 + 0x16),(char *)puVar2[1],uVar4);
      if ((uVar4 != uVar1) && ((*(byte *)((int)puVar2 + 0x13) & 2) == 0)) {
        *(ushort *)((int)puVar2 + 0x12) = *(ushort *)((int)puVar2 + 0x12) | 0x10;
      }
      iVar3 = iVar3 + 1;
    }
    puVar2 = puVar2 + 6;
    iVar5 = iVar5 + -1;
  }
  return iVar3;
}



void __cdecl FUN_0041fa40(undefined *param_1)

{
  PTR_DAT_0042e414 = param_1;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __cdecl FUN_0041fa50(undefined4 *param_1)

{
  if (param_1 != (undefined4 *)0x0) {
    _DAT_0042de90 = *param_1;
  }
  return;
}



int __cdecl FUN_0041fa64(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  int *piVar2;
  
  iVar1 = 0;
  for (piVar2 = &DAT_0042e348; (iVar1 < DAT_0042e344 && (*piVar2 != 0)); piVar2 = piVar2 + 1) {
    iVar1 = iVar1 + 1;
  }
  if (iVar1 == DAT_0042e344) {
    return -1;
  }
  (&DAT_0042e348)[iVar1] = param_2;
  *(undefined4 *)(&DAT_004313bc + iVar1 * 4) = param_1;
  return iVar1;
}



uint __cdecl FUN_0041faa4(uint param_1,uint param_2,undefined4 param_3)

{
  int *piVar1;
  
  if (param_2 == 0xffffffff) {
    param_2 = 0;
    for (piVar1 = &DAT_0042e348; ((int)param_2 < (int)DAT_0042e344 && (*piVar1 != 0));
        piVar1 = piVar1 + 1) {
      param_2 = param_2 + 1;
    }
  }
  if ((param_1 < DAT_0042e344) && (param_2 < DAT_0042e344)) {
    if (((&DAT_0042e348)[param_1] != 0) && ((&DAT_0042e348)[param_2] == 0)) {
      (&DAT_0042e348)[param_2] = (&DAT_0042e348)[param_1];
      *(undefined4 *)(&DAT_004313bc + param_2 * 4) = param_3;
      return param_2;
    }
    return 0xffffffff;
  }
  return 0xffffffff;
}



void __cdecl FUN_0041fb0c(uint param_1)

{
  if (param_1 < DAT_0042e344) {
    (&DAT_0042e348)[param_1] = 0;
  }
  return;
}



int __cdecl FUN_0041fb28(int *param_1)

{
  uint uVar1;
  int iVar2;
  byte bVar3;
  int *piVar4;
  byte *pbVar5;
  uint *local_c;
  int local_8;
  
  iVar2 = DAT_0042e344;
  for (piVar4 = &DAT_0042e344 + DAT_0042e344; (iVar2 != 0 && (*piVar4 == 0)); piVar4 = piVar4 + -1)
  {
    iVar2 = iVar2 + -1;
  }
  if (param_1 == (int *)0x0) {
    if (iVar2 == 0) {
      iVar2 = 0;
    }
    else {
      iVar2 = iVar2 * 5 + 4;
    }
  }
  else {
    *param_1 = iVar2;
    local_8 = 0;
    pbVar5 = (byte *)(param_1 + 1);
    local_c = &DAT_0042e348;
    if (0 < iVar2) {
      do {
        bVar3 = 1;
        uVar1 = *local_c;
        if ((uVar1 & 0x800) != 0) {
          bVar3 = 0x21;
        }
        if ((uVar1 & 0x8000) == 0) {
          bVar3 = bVar3 | 0x80;
        }
        if ((uVar1 & 0x2000) != 0) {
          bVar3 = bVar3 | 0x40;
        }
        *pbVar5 = bVar3;
        pbVar5 = pbVar5 + 1;
        local_8 = local_8 + 1;
        local_c = local_c + 1;
      } while (local_8 < iVar2);
    }
    BCB_memcpy((undefined4 *)pbVar5,(undefined4 *)&DAT_004313bc,iVar2 << 2);
    iVar2 = 0;
  }
  return iVar2;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __stdcall FUN_0041fbc4(void)

{
  bool bVar1;
  UINT UVar2;
  uint uVar3;
  undefined4 *puVar4;
  undefined3 extraout_var;
  int iVar5;
  uint uVar6;
  uint uVar7;
  uint *puVar8;
  uint *puVar9;
  _STARTUPINFOA local_50;
  
  UVar2 = SetHandleCount(DAT_0042e344);
  if (UVar2 < DAT_0042e344) {
    DAT_0042e344 = UVar2;
  }
  PTR_FUN_0042de8c = FUN_0041fb28;
  bVar1 = false;
  GetStartupInfoA(&local_50);
  if (local_50.cbReserved2 != 0) {
    uVar7 = *(uint *)local_50.lpReserved2;
    puVar8 = (uint *)((int)local_50.lpReserved2 + 4);
    if (uVar7 * 5 + 4 == (uint)local_50.cbReserved2) {
      bVar1 = true;
      iVar5 = 0;
      puVar9 = &DAT_0042e348;
      if (0 < (int)uVar7) {
        do {
          uVar6 = *puVar8;
          puVar8 = (uint *)((int)puVar8 + 1);
          uVar3 = 0;
          if ((uVar6 & 0x20) != 0) {
            uVar3 = 0x800;
          }
          if ((uVar6 & 0x40) != 0) {
            uVar3 = uVar3 | 0x2000;
          }
          if ((uVar6 & 0x80) == 0) {
            uVar3 = uVar3 | 0x8000;
          }
          else {
            uVar3 = uVar3 | 0x4000;
          }
          iVar5 = iVar5 + 1;
          *puVar9 = *puVar9 & 3 | uVar3;
          puVar9 = puVar9 + 1;
        } while (iVar5 < (int)uVar7);
      }
      puVar4 = &DAT_0042e348 + uVar7;
      for (uVar6 = uVar7; uVar6 < DAT_0042e344; uVar6 = uVar6 + 1) {
        *puVar4 = 0;
        puVar4 = puVar4 + 1;
      }
      FUN_0041e834((undefined4 *)&DAT_004313bc,puVar8,uVar7 << 2);
    }
  }
  if (!bVar1) {
    _DAT_004313bc = GetStdHandle(0xfffffff6);
    DAT_004313c0 = GetStdHandle(0xfffffff5);
    _DAT_004313c4 = GetStdHandle(0xfffffff4);
    uVar7 = 0;
    puVar8 = &DAT_0042e348;
    do {
      bVar1 = FUN_0041f658(uVar7);
      if ((CONCAT31(extraout_var,bVar1) == 0) && (DAT_0042f2f4 == 0)) {
        *puVar8 = *puVar8 & 0xffffdfff;
      }
      else {
        *puVar8 = *puVar8 | 0x2000;
        if (DAT_0042f2f4 != 0) {
          *puVar8 = *puVar8 | 0x10000;
        }
      }
      uVar7 = uVar7 + 1;
      puVar8 = puVar8 + 1;
    } while ((int)uVar7 < 3);
  }
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 __cdecl FUN_0041fd20(int param_1)

{
  if (param_1 < 0) {
    if (-param_1 < DAT_0042e608) {
      _DAT_0042f1d0 = -param_1;
      _DAT_00431484 = 0xffffffff;
      return 0xffffffff;
    }
  }
  else if (param_1 < 299) goto LAB_0041fd36;
  param_1 = 1;
LAB_0041fd36:
  _DAT_00431484 = param_1;
  _DAT_0042f1d0 = (int)(char)(&DAT_0042e418)[param_1];
  return 0xffffffff;
}



void __stdcall FUN_0041fd84(void)

{
  DWORD DVar1;
  
  DVar1 = GetLastError();
  FUN_0041fd20(DVar1 & 0xffff);
  return;
}



uint __cdecl FUN_0041fd98(undefined4 *param_1,uint param_2,int *param_3,int param_4)

{
  BCB_memcpy((undefined4 *)*param_3,param_1,param_2);
  if (param_4 != 0) {
    *param_3 = *param_3 + param_2;
    *(undefined1 *)*param_3 = 0;
  }
  return param_2;
}



void __cdecl FUN_0041fdc8(undefined1 *param_1,int param_2,byte *param_3)

{
  if (param_2 != 0) {
    *param_1 = 0;
  }
  FUN_00420048(FUN_0041fd98,&param_1,param_3,1,param_2,(uint *)&stack0x00000010);
  return;
}



void __cdecl FUN_0041fdf8(undefined1 *param_1,int param_2,byte *param_3,uint *param_4)

{
  if (param_2 != 0) {
    *param_1 = 0;
  }
  FUN_00420048(FUN_0041fd98,&param_1,param_3,1,param_2,param_4);
  return;
}



uint __cdecl FUN_0041fe28(undefined1 *param_1,uint param_2,byte *param_3)

{
  uint uVar1;
  uint uVar2;
  
  uVar1 = param_2;
  if (param_2 != 0) {
    *param_1 = 0;
  }
  uVar2 = FUN_00420048(FUN_0041fd98,&param_1,param_3,1,param_2,(uint *)&stack0x00000010);
  if (uVar1 < uVar2) {
    uVar2 = 0xffffffff;
  }
  return uVar2;
}



uint __cdecl FUN_0041fe60(undefined1 *param_1,uint param_2,byte *param_3,uint *param_4)

{
  uint uVar1;
  uint uVar2;
  
  uVar1 = param_2;
  if (param_2 != 0) {
    *param_1 = 0;
  }
  uVar2 = FUN_00420048(FUN_0041fd98,&param_1,param_3,1,param_2,param_4);
  if (uVar1 < uVar2) {
    uVar2 = 0xffffffff;
  }
  return uVar2;
}



void __cdecl FUN_0041ff98(int param_1)

{
  int iVar1;
  
  if (*(int *)(param_1 + 0x50) != 0) {
    iVar1 = (**(code **)(param_1 + 0x54))();
    if (iVar1 == 0) {
      *(undefined4 *)(param_1 + 0x60) = 1;
    }
    *(undefined4 *)(param_1 + 0x50) = 0;
  }
  return;
}



void __cdecl FUN_0041ffe0(undefined1 param_1,int param_2)

{
  if (0x4f < *(int *)(param_2 + 0x50)) {
    FUN_0041ff98(param_2);
  }
  if ((*(uint **)(param_2 + 100) == (uint *)0x0) ||
     (*(uint *)(param_2 + 0x5c) < **(uint **)(param_2 + 100))) {
    *(undefined1 *)(param_2 + *(int *)(param_2 + 0x50)) = param_1;
    *(int *)(param_2 + 0x50) = *(int *)(param_2 + 0x50) + 1;
  }
  *(int *)(param_2 + 0x5c) = *(int *)(param_2 + 0x5c) + 1;
  return;
}



void __cdecl FUN_00420014(uint param_1,int param_2)

{
  char *pcVar1;
  int iVar2;
  char cVar3;
  
  iVar2 = 7;
  pcVar1 = (char *)(param_2 + 7);
  do {
    cVar3 = (char)(param_1 & 0xf);
    if ((param_1 & 0xf) < 10) {
      *pcVar1 = cVar3 + '0';
    }
    else {
      *pcVar1 = cVar3 + '7';
    }
    param_1 = param_1 >> 4;
    iVar2 = iVar2 + -1;
    pcVar1 = pcVar1 + -1;
  } while (-1 < iVar2);
  return;
}



uint __cdecl
FUN_00420048(undefined4 param_1,undefined4 param_2,byte *param_3,int param_4,undefined4 param_5,
            uint *param_6)

{
  bool bVar1;
  bool bVar2;
  bool bVar3;
  bool bVar4;
  uint uVar5;
  char *pcVar6;
  int iVar7;
  uint *puVar8;
  char *pcVar9;
  byte bVar10;
  char cVar11;
  byte *pbVar12;
  byte *pbVar13;
  uint uVar14;
  bool bVar15;
  undefined1 local_524 [80];
  undefined4 local_4d4;
  undefined4 local_4d0;
  undefined4 local_4cc;
  uint local_4c8;
  int local_4c4;
  undefined4 *local_4c0;
  uint local_4bc [256];
  undefined1 local_bc [2];
  undefined2 local_ba;
  undefined1 local_b4;
  CHAR local_5a [2];
  char *local_58;
  int local_54;
  char *local_50;
  uint *local_48;
  char *local_44;
  char *local_40;
  uint local_3c;
  ushort local_36;
  uint local_34;
  uint local_30;
  uint local_2c;
  uint local_28;
  char local_21;
  int local_20;
  uint *local_1c;
  byte *local_18;
  int local_14;
  byte local_d;
  uint local_c;
  uint local_8;
  
  local_4c4 = 0;
  local_4c8 = 0;
  local_4d4 = 0;
  local_4d0 = param_1;
  local_4cc = param_2;
  if (param_4 == 0) {
    local_4c0 = (undefined4 *)0x0;
  }
  else {
    local_4c0 = &param_5;
  }
LAB_00420090:
  pbVar12 = param_3;
  bVar10 = *pbVar12;
  param_3 = pbVar12 + 1;
  if (bVar10 != 0) {
    if (bVar10 == 0x25) {
      if (*param_3 != 0x25) {
        uVar5 = 0;
        local_14 = 0;
        local_d = 0;
        local_c = 0xffffffff;
        local_8 = 0xffffffff;
        uVar14 = 0;
        bVar4 = false;
        bVar2 = false;
        bVar15 = false;
        bVar3 = false;
        local_20 = 0;
switchD_00420131_default:
        pbVar13 = param_3;
        bVar1 = false;
        bVar10 = *pbVar13;
        param_3 = pbVar13 + 1;
        local_18 = pbVar12;
        if (((char)bVar10 < ' ') || ('\x7f' < (char)bVar10)) goto switchD_00420131_caseD_15;
        switch((&DAT_0042e9d6)[(byte)(bVar10 - 0x20)]) {
        case 0:
          local_18 = pbVar12;
          if (uVar5 != 0) goto switchD_00420131_caseD_15;
          if (local_d != 0x2b) {
            local_d = bVar10;
          }
          goto switchD_00420131_default;
        case 1:
          local_18 = pbVar12;
          if (uVar5 != 0) goto switchD_00420131_caseD_15;
          bVar2 = true;
          goto switchD_00420131_default;
        case 2:
          puVar8 = param_6 + 1;
          local_34 = *param_6;
          param_6 = puVar8;
          if (uVar5 < 2) {
            local_8 = local_34;
            if ((int)local_34 < 0) {
              local_8 = -local_34;
              bVar3 = true;
            }
            uVar5 = 3;
          }
          else {
            local_18 = pbVar12;
            if (uVar5 != 4) goto switchD_00420131_caseD_15;
            uVar5 = 5;
            local_c = local_34;
          }
          goto switchD_00420131_default;
        case 3:
          local_18 = pbVar12;
          if (uVar5 != 0) goto switchD_00420131_caseD_15;
          bVar3 = true;
          goto switchD_00420131_default;
        case 4:
          local_18 = pbVar12;
          if (3 < uVar5) goto switchD_00420131_caseD_15;
          uVar5 = 4;
          local_c = local_c + 1;
          goto switchD_00420131_default;
        case 5:
          goto switchD_00420131_caseD_5;
        case 6:
          uVar14 = uVar14 | 0x10;
          uVar5 = 5;
          goto switchD_00420131_default;
        case 7:
          uVar5 = 5;
          uVar14 = uVar14 & 0xffffffef;
          bVar4 = true;
          goto switchD_00420131_default;
        case 8:
          uVar5 = 5;
          uVar14 = 0x200;
          goto switchD_00420131_default;
        case 9:
          if (uVar5 == 0) {
            if (!bVar3) {
              bVar15 = true;
              uVar5 = 1;
            }
          }
          else {
switchD_00420131_caseD_5:
            cVar11 = bVar10 - 0x30;
            if (uVar5 < 3) {
              uVar5 = 2;
              if (local_8 == 0xffffffff) {
                local_8 = (uint)cVar11;
              }
              else {
                local_8 = local_8 * 10 + (int)cVar11;
              }
            }
            else {
              local_18 = pbVar12;
              if (uVar5 != 4) goto switchD_00420131_caseD_15;
              local_c = local_c * 10 + (int)cVar11;
            }
          }
          goto switchD_00420131_default;
        case 10:
          goto switchD_00420131_caseD_a;
        case 0xb:
          local_3c = 8;
          break;
        case 0xc:
          local_3c = 10;
          break;
        case 0xd:
          local_3c = 0x10;
          local_21 = bVar10 - 0x17;
          break;
        case 0xe:
          local_1c = (uint *)*param_6;
          param_6 = param_6 + 1;
          FUN_00420014((uint)local_1c,(int)local_bc);
          local_b4 = 0;
          local_1c = (uint *)local_bc;
          goto LAB_004206bc;
        case 0xf:
          local_1c = (uint *)(local_bc + 1);
          thunk_FUN_00421544();
          param_6 = (uint *)thunk_FUN_00421544();
          goto LAB_004206bc;
        case 0x10:
          goto switchD_00420131_caseD_10;
        case 0x11:
          goto switchD_00420131_caseD_11;
        case 0x12:
          if (uVar14 == 0) {
            uVar14 = 0x10;
          }
switchD_00420131_caseD_10:
          if ((uVar14 & 0x10) == 0) {
            local_1c = (uint *)local_bc;
            local_bc[1] = 0;
            local_bc[0] = (byte)*param_6;
            local_20 = 0;
            local_44 = (char *)0x1;
            param_6 = param_6 + 1;
          }
          else {
            local_1c = (uint *)local_bc;
            local_bc = SUB42(*param_6,0);
            local_ba = 0;
            local_20 = 1;
            local_44 = (char *)0x1;
            param_6 = param_6 + 1;
          }
          goto LAB_0042072a;
        case 0x13:
          if (uVar14 == 0) {
            uVar14 = 0x10;
          }
switchD_00420131_caseD_11:
          if ((uVar14 & 0x10) == 0) {
            local_1c = (uint *)*param_6;
            local_20 = 0;
            if (local_1c == (uint *)0x0) {
              local_1c = (uint *)s__null__0042e9c0;
            }
          }
          else {
            local_1c = (uint *)*param_6;
            local_20 = 1;
            if (local_1c == (uint *)0x0) {
              local_1c = (uint *)&DAT_0042e9c8;
            }
          }
          param_6 = param_6 + 1;
          if (local_20 == 0) {
            uVar14 = local_c;
            if ((int)local_c < 0) {
              uVar14 = 0x7fffffff;
            }
            local_44 = (char *)0x0;
            for (puVar8 = local_1c; (uVar14 != 0 && ((byte)*puVar8 != 0));
                puVar8 = (uint *)((int)puVar8 + 1)) {
              uVar14 = uVar14 - 1;
              local_44 = local_44 + 1;
            }
          }
          else {
            uVar14 = local_c;
            if ((int)local_c < 0) {
              uVar14 = 0x7fffffff;
            }
            local_44 = (char *)0x0;
            for (puVar8 = local_1c; (uVar14 != 0 && ((ushort)*puVar8 != 0));
                puVar8 = (uint *)((int)puVar8 + 2)) {
              uVar14 = uVar14 - 1;
              local_44 = local_44 + 1;
            }
          }
          goto LAB_0042072a;
        case 0x14:
          puVar8 = param_6 + 1;
          local_1c = (uint *)*param_6;
          param_6 = puVar8;
          if ((uVar14 & 0x10) == 0) {
            if ((uVar14 & 0x200) == 0) {
              *local_1c = local_4c8;
            }
            else {
              *(ushort *)local_1c = (ushort)local_4c8;
            }
          }
          else {
            *local_1c = local_4c8;
          }
          goto LAB_00420090;
        case 0x15:
        case 0x16:
        case 0x17:
          goto switchD_00420131_caseD_15;
        case 0x18:
          uVar5 = 5;
          goto switchD_00420131_default;
        case 0x19:
          uVar5 = 5;
          goto switchD_00420131_default;
        case 0x1a:
          if ((*param_3 == 0x36) && (pbVar13[2] == 0x34)) {
            uVar14 = 0;
            bVar4 = true;
            uVar5 = 5;
            param_3 = pbVar13 + 3;
          }
          else if ((*param_3 == 0x33) && (pbVar13[2] == 0x32)) {
            uVar14 = 0x10;
            bVar4 = false;
            uVar5 = 5;
            param_3 = pbVar13 + 3;
          }
          else if ((*param_3 == 0x31) && (pbVar13[2] == 0x36)) {
            uVar14 = 0x200;
            bVar4 = false;
            uVar5 = 5;
            param_3 = pbVar13 + 3;
          }
          else if (*param_3 == 0x38) {
            uVar14 = 0;
            bVar4 = false;
            uVar5 = 5;
            param_3 = pbVar13 + 2;
          }
        default:
          goto switchD_00420131_default;
        }
        local_d = 0;
        cVar11 = '\0';
        goto LAB_004203ac;
      }
      param_3 = pbVar12 + 2;
      bVar10 = 0x25;
    }
    if ((((&DAT_00431489)[bVar10] & 4) != 0) && (*param_3 != 0)) {
      FUN_0041ffe0(bVar10,(int)local_524);
      bVar10 = *param_3;
      param_3 = param_3 + 1;
    }
    FUN_0041ffe0(bVar10,(int)local_524);
    goto LAB_00420090;
  }
  goto LAB_00420958;
switchD_00420131_caseD_a:
  local_3c = 10;
  cVar11 = '\x01';
LAB_004203ac:
  if (bVar4) {
    local_2c = *param_6;
    local_28 = param_6[1];
    param_6 = param_6 + 2;
  }
  else if ((uVar14 & 0x10) == 0) {
    if ((uVar14 & 0x200) == 0) {
      local_34 = *param_6;
      local_2c = local_34;
      if (cVar11 == '\0') {
        local_28 = 0;
        param_6 = param_6 + 1;
      }
      else {
        local_28 = (int)local_34 >> 0x1f;
        param_6 = param_6 + 1;
      }
    }
    else {
      local_36 = (ushort)*param_6;
      if (cVar11 == '\0') {
        local_2c = (uint)local_36;
        local_28 = 0;
        param_6 = param_6 + 1;
      }
      else {
        local_2c = (uint)(short)local_36;
        local_28 = (int)local_2c >> 0x1f;
        param_6 = param_6 + 1;
      }
    }
  }
  else {
    local_30 = *param_6;
    local_2c = local_30;
    if (cVar11 == '\0') {
      local_28 = 0;
      param_6 = param_6 + 1;
    }
    else {
      local_28 = (int)local_30 >> 0x1f;
      param_6 = param_6 + 1;
    }
  }
  local_1c = (uint *)(local_bc + 1);
  if (((local_28 == 0) && (local_2c == 0)) && (local_c == 0)) {
                    // WARNING: Ignoring partial resolution of indirect
    local_bc[1] = 0;
  }
  else {
    FUN_004215f4(local_2c,local_28,(char *)local_1c,local_3c,cVar11,local_21);
  }
  if ((int)local_c < 0) {
LAB_004206bc:
    if ((bVar15) && (0 < (int)local_8)) {
      local_44 = FUN_0041e924(local_1c);
      if ((byte)*local_1c == 0x2d) {
        local_44 = local_44 + -1;
      }
      if ((int)local_44 < (int)local_8) {
        local_14 = local_8 - (int)local_44;
      }
    }
    if (((byte)*local_1c == 0x2d) || (local_d != 0)) {
      if ((byte)*local_1c != 0x2d) {
        local_1c = (uint *)((int)local_1c + -1);
        *(byte *)local_1c = local_d;
      }
      if (0 < local_14) {
        local_14 = local_14 + -1;
      }
    }
    local_44 = FUN_0041e924(local_1c);
  }
  else {
    local_40 = FUN_0041e924(local_1c);
    local_44 = local_40;
    if ((byte)*local_1c == 0x2d) {
      local_40 = local_40 + -1;
    }
    else if (local_d != 0) {
      local_44 = local_40 + 1;
      local_1c = (uint *)((int)local_1c + -1);
      *(byte *)local_1c = local_d;
    }
    if ((int)local_40 < (int)local_c) {
      local_14 = local_c - (int)local_40;
    }
  }
LAB_0042072a:
  if (bVar2) {
    if (bVar10 == 0x6f) {
      if (local_14 < 1) {
        local_14 = 1;
      }
    }
    else if ((bVar10 == 0x78) || (bVar10 == 0x58)) {
      bVar1 = true;
      local_8 = local_8 - 2;
      local_14 = local_14 + -2;
      if (local_14 < 0) {
        local_14 = 0;
      }
    }
  }
  local_44 = local_44 + local_14;
  if ((!bVar3) && ((int)local_44 < (int)local_8)) {
    do {
      FUN_0041ffe0(0x20,(int)local_524);
      local_8 = local_8 - 1;
    } while ((int)local_44 < (int)local_8);
  }
  if (bVar1) {
    FUN_0041ffe0(0x30,(int)local_524);
    FUN_0041ffe0(bVar10,(int)local_524);
  }
  if (0 < local_14) {
    local_44 = local_44 + -local_14;
    local_8 = local_8 - local_14;
    if ((((byte)*local_1c == 0x2d) || ((byte)*local_1c == 0x20)) || ((byte)*local_1c == 0x2b)) {
      uVar14 = *local_1c;
      local_1c = (uint *)((int)local_1c + 1);
      FUN_0041ffe0((byte)uVar14,(int)local_524);
      local_44 = local_44 + -1;
      local_8 = local_8 - 1;
    }
    while (iVar7 = local_14 + -1, bVar15 = local_14 != 0, local_14 = iVar7, bVar15) {
      FUN_0041ffe0(0x30,(int)local_524);
    }
  }
  if (local_20 != 0) {
    local_48 = local_1c;
    local_50 = (char *)0x0;
    local_58 = local_44;
    while (pcVar6 = local_58 + -1, bVar15 = 0 < (int)local_58, local_58 = pcVar6, bVar15) {
      uVar14 = *local_48;
      local_48 = (uint *)((int)local_48 + 2);
      local_54 = FUN_00420de8(local_5a,(ushort)uVar14);
      if (local_54 < 1) break;
      iVar7 = 0;
      pcVar9 = (char *)((int)local_4bc + (int)local_50);
      pcVar6 = local_5a;
      if (0 < local_54) {
        do {
          cVar11 = *pcVar6;
          pcVar6 = pcVar6 + 1;
          *pcVar9 = cVar11;
          pcVar9 = pcVar9 + 1;
          local_50 = local_50 + 1;
          iVar7 = iVar7 + 1;
        } while (iVar7 < local_54);
      }
    }
    local_1c = local_4bc;
    local_44 = local_50;
  }
  if (local_44 != (char *)0x0) {
    local_8 = local_8 - (int)local_44;
    while (local_44 != (char *)0x0) {
      uVar14 = *local_1c;
      local_44 = local_44 + -1;
      local_1c = (uint *)((int)local_1c + 1);
      FUN_0041ffe0((byte)uVar14,(int)local_524);
    }
    local_44 = (char *)0xffffffff;
  }
  while (uVar14 = local_8 - 1, bVar15 = 0 < (int)local_8, local_8 = uVar14, bVar15) {
    FUN_0041ffe0(0x20,(int)local_524);
  }
  goto LAB_00420090;
switchD_00420131_caseD_15:
  while( true ) {
    pbVar12 = local_18 + 1;
    bVar10 = *local_18;
    local_18 = pbVar12;
    if (bVar10 == 0) break;
    FUN_0041ffe0(bVar10,(int)local_524);
  }
LAB_00420958:
  FUN_0041ff98((int)local_524);
  if (local_4c4 != 0) {
    local_4c8 = 0xffffffff;
  }
  return local_4c8;
}



undefined4 __cdecl FUN_004209a8(int param_1)

{
  if (param_1 < 0x11) {
    if (param_1 == 0x10) {
      return *(undefined4 *)(*(int *)(PTR_DAT_0042ed78 + 0x18) + 8);
    }
    if (param_1 == 0xe) {
      return *(undefined4 *)(*(int *)(PTR_DAT_0042ed78 + 0x18) + 4);
    }
    if (param_1 == 0xf) {
      return **(undefined4 **)(PTR_DAT_0042ed78 + 0x18);
    }
  }
  else {
    if (param_1 == 0x50) {
      return *(undefined4 *)(*(int *)(PTR_DAT_0042ed78 + 0x18) + 0xc);
    }
    if (param_1 == 0x51) {
      return *(undefined4 *)(*(int *)(PTR_DAT_0042ed78 + 0x18) + 0x10);
    }
  }
  return 0;
}



void __cdecl FUN_004209fc(uint param_1)

{
  FUN_00420c04(param_1,0x104);
  return;
}



bool __cdecl FUN_00420a14(uint param_1)

{
  return param_1 < 0x80;
}



void __cdecl FUN_00420a28(uint param_1)

{
  FUN_00420c04(param_1,0x100);
  return;
}



void __cdecl FUN_00420a40(uint param_1)

{
  FUN_00420c04(param_1,0x20);
  return;
}



void __cdecl FUN_00420a54(uint param_1)

{
  FUN_00420c04(param_1,4);
  return;
}



void __cdecl FUN_00420a68(uint param_1)

{
  FUN_00420c04(param_1,0x194);
  return;
}



void __cdecl FUN_00420a80(uint param_1)

{
  FUN_00420c04(param_1,2);
  return;
}



void __cdecl FUN_00420a94(uint param_1)

{
  FUN_00420c04(param_1,0x1d4);
  return;
}



void __cdecl FUN_00420aac(uint param_1)

{
  FUN_00420c04(param_1,0x10);
  return;
}



void __cdecl FUN_00420ac0(uint param_1)

{
  FUN_00420c04(param_1,8);
  return;
}



void __cdecl FUN_00420ad4(uint param_1)

{
  FUN_00420c04(param_1,1);
  return;
}



void __cdecl FUN_00420ae8(uint param_1)

{
  FUN_00420c04(param_1,0x80);
  return;
}



void __cdecl FUN_00420b00(ushort param_1)

{
  FUN_00420c48((uint)param_1,0x104);
  return;
}



bool __cdecl FUN_00420b18(ushort param_1)

{
  return param_1 < 0x80;
}



void __cdecl FUN_00420b2c(ushort param_1)

{
  FUN_00420c48((uint)param_1,0x100);
  return;
}



void __cdecl FUN_00420b44(ushort param_1)

{
  FUN_00420c48((uint)param_1,0x20);
  return;
}



void __cdecl FUN_00420b58(ushort param_1)

{
  FUN_00420c48((uint)param_1,4);
  return;
}



void __cdecl FUN_00420b6c(ushort param_1)

{
  FUN_00420c48((uint)param_1,0x194);
  return;
}



void __cdecl FUN_00420b84(ushort param_1)

{
  FUN_00420c48((uint)param_1,2);
  return;
}



void __cdecl FUN_00420b98(ushort param_1)

{
  FUN_00420c48((uint)param_1,0x1d4);
  return;
}



void __cdecl FUN_00420bb0(ushort param_1)

{
  FUN_00420c48((uint)param_1,0x10);
  return;
}



void __cdecl FUN_00420bc4(ushort param_1)

{
  FUN_00420c48((uint)param_1,8);
  return;
}



void __cdecl FUN_00420bd8(ushort param_1)

{
  FUN_00420c48((uint)param_1,1);
  return;
}



void __cdecl FUN_00420bec(ushort param_1)

{
  FUN_00420c48((uint)param_1,0x80);
  return;
}



ushort __cdecl FUN_00420c04(uint param_1,ushort param_2)

{
  if (0xff < param_1) {
    return 0;
  }
  if ((*(int *)(PTR_DAT_0042ed78 + 8) == 0) && (*(int *)(PTR_DAT_0042ed78 + 0x10) != 0)) {
    return param_2 & *(ushort *)(*(int *)(PTR_DAT_0042ed78 + 0x10) + 2 + param_1 * 2);
  }
  return *(ushort *)(&DAT_0042ea3a + (short)param_1 * 2) & param_2;
}



uint __cdecl FUN_00420c48(WCHAR param_1,uint param_2)

{
  uint uVar1;
  ushort local_6;
  
  uVar1 = param_2;
  if (param_1 == L'\xffff') {
    uVar1 = 0;
  }
  else if ((*(int *)(PTR_DAT_0042ed78 + 8) == 0) || (L'ÿ' < param_1)) {
    GetStringTypeW(1,&param_1,1,&local_6);
    uVar1 = local_6 & uVar1;
  }
  else {
    uVar1 = *(ushort *)(&DAT_0042ea3a + (short)param_1 * 2) & param_2;
  }
  return uVar1;
}



undefined4 __cdecl FUN_00420ca0(byte *param_1,int param_2)

{
  int iVar1;
  
  if (((param_1 != (byte *)0x0) && (*param_1 != 0)) && (param_2 != 0)) {
    if (((&DAT_00431489)[*param_1] & 4) == 0) {
      iVar1 = MultiByteToWideChar(*(UINT *)PTR_DAT_0042ed78,9,(LPCSTR)param_1,1,(LPWSTR)0x0,0);
      if (iVar1 == 0) {
        return 0xffffffff;
      }
      return 1;
    }
    if (1 < param_2) {
      iVar1 = MultiByteToWideChar(*(UINT *)PTR_DAT_0042ed78,9,(LPCSTR)param_1,2,(LPWSTR)0x0,0);
      if (iVar1 != 0) {
        return 2;
      }
    }
    return 0xffffffff;
  }
  return 0;
}



undefined4 __cdecl FUN_00420d1c(LPWSTR param_1,byte *param_2,int param_3)

{
  undefined4 uVar1;
  int iVar2;
  
  if ((param_2 == (byte *)0x0) || (param_3 == 0)) {
    uVar1 = 0;
  }
  else if (*param_2 == 0) {
    if (param_1 != (LPWSTR)0x0) {
      *(undefined2 *)param_1 = 0;
    }
    uVar1 = 0;
  }
  else if (*(int *)(PTR_DAT_0042ed78 + 8) == 0) {
    if (((&DAT_00431489)[*param_2] & 4) == 0) {
      iVar2 = MultiByteToWideChar(*(UINT *)PTR_DAT_0042ed78,9,(LPCSTR)param_2,1,param_1,
                                  (uint)(param_1 != (LPWSTR)0x0));
      if (iVar2 == 0) {
        uVar1 = 0xffffffff;
      }
      else {
        uVar1 = 1;
      }
    }
    else if (((param_3 < 2) ||
             (iVar2 = MultiByteToWideChar(*(UINT *)PTR_DAT_0042ed78,9,(LPCSTR)param_2,2,param_1,
                                          (uint)(param_1 != (LPWSTR)0x0)), iVar2 == 0)) &&
            ((param_3 < 2 || (param_2[1] == 0)))) {
      uVar1 = 0xffffffff;
    }
    else {
      uVar1 = 2;
    }
  }
  else {
    if (param_1 != (LPWSTR)0x0) {
      *(ushort *)param_1 = (ushort)*param_2;
    }
    uVar1 = 1;
  }
  return uVar1;
}



int __cdecl FUN_00420de8(LPSTR param_1,ushort param_2)

{
  int iVar1;
  BOOL local_8;
  
  if (param_1 == (LPSTR)0x0) {
    return 0;
  }
  if (*(int *)(PTR_DAT_0042ed78 + 8) != 0) {
    if (0xff < param_2) {
      return -1;
    }
    *param_1 = (CHAR)param_2;
    return 1;
  }
  local_8 = 0;
  iVar1 = WideCharToMultiByte(*(UINT *)PTR_DAT_0042ed78,0x220,(LPCWSTR)&param_2,1,param_1,2,
                              (LPCSTR)0x0,&local_8);
  if ((iVar1 != 0) && (local_8 == 0)) {
    return iVar1;
  }
  return -1;
}



char * __cdecl FUN_00420e5c(LPWSTR param_1,uint *param_2,char *param_3)

{
  int iVar1;
  DWORD DVar2;
  uint *puVar3;
  char *pcVar4;
  
  pcVar4 = (char *)0x0;
  if ((param_1 == (LPWSTR)0x0) || (param_3 != (char *)0x0)) {
    if (param_1 == (LPWSTR)0x0) {
      if (*(int *)(PTR_DAT_0042ed78 + 8) == 0) {
        iVar1 = MultiByteToWideChar(*(UINT *)PTR_DAT_0042ed78,9,(LPCSTR)param_2,-1,(LPWSTR)0x0,0);
        if (iVar1 == 0) {
          pcVar4 = (char *)0xffffffff;
        }
        else {
          pcVar4 = (char *)(iVar1 + -1);
        }
      }
      else {
        pcVar4 = FUN_0041e924(param_2);
      }
    }
    else if (*(int *)(PTR_DAT_0042ed78 + 8) == 0) {
      iVar1 = MultiByteToWideChar(*(UINT *)PTR_DAT_0042ed78,9,(LPCSTR)param_2,-1,param_1,
                                  (int)param_3);
      if (iVar1 == 0) {
        DVar2 = GetLastError();
        puVar3 = param_2;
        pcVar4 = param_3;
        if (DVar2 == 0x7a) {
          for (; (pcVar4 != (char *)0x0 && ((byte)*puVar3 != 0)); puVar3 = (uint *)((int)puVar3 + 1)
              ) {
            if (((&DAT_00431489)[(byte)*puVar3] & 4) != 0) {
              puVar3 = (uint *)((int)puVar3 + 1);
            }
            pcVar4 = pcVar4 + -1;
          }
          pcVar4 = (char *)MultiByteToWideChar(*(UINT *)PTR_DAT_0042ed78,1,(LPCSTR)param_2,
                                               (int)puVar3 - (int)param_2,param_1,(int)param_3);
          if (pcVar4 == (char *)0x0) {
            pcVar4 = (char *)0xffffffff;
          }
        }
        else {
          pcVar4 = (char *)0xffffffff;
        }
      }
      else {
        pcVar4 = (char *)(iVar1 + -1);
      }
    }
    else if (param_3 != (char *)0x0) {
      do {
        *(ushort *)param_1 = (ushort)*(byte *)((int)param_2 + (int)pcVar4);
        if (*(char *)((int)param_2 + (int)pcVar4) == '\0') {
          return pcVar4;
        }
        pcVar4 = pcVar4 + 1;
        param_1 = (LPWSTR)((int)param_1 + 2);
      } while (pcVar4 < param_3);
    }
  }
  else {
    pcVar4 = (char *)0x0;
  }
  return pcVar4;
}



int __cdecl FUN_00420f88(short *param_1,int param_2)

{
  short *psVar1;
  int iVar2;
  
  iVar2 = param_2 + 1;
  for (psVar1 = param_1; (iVar2 = iVar2 + -1, iVar2 != 0 && (*psVar1 != 0)); psVar1 = psVar1 + 1) {
  }
  if ((iVar2 != 0) && (*psVar1 == 0)) {
    iVar2 = (int)psVar1 - (int)param_1 >> 1;
    if (iVar2 < 0) {
      iVar2 = iVar2 + (uint)(((int)psVar1 - (int)param_1 & 1U) != 0);
    }
    return iVar2 + 1;
  }
  return param_2;
}



uint __cdecl FUN_00420fc4(LPSTR param_1,LPCWSTR param_2,uint param_3)

{
  char cVar1;
  DWORD DVar2;
  char *pcVar3;
  char *pcVar4;
  uint uVar5;
  int iVar6;
  BOOL local_10;
  CHAR local_a [2];
  int local_8;
  
  local_10 = 0;
  uVar5 = 0;
  if ((param_1 == (LPSTR)0x0) || (param_3 != 0)) {
    if (param_1 == (LPSTR)0x0) {
      if (*(int *)(PTR_DAT_0042ed78 + 8) == 0) {
        iVar6 = WideCharToMultiByte(*(UINT *)PTR_DAT_0042ed78,0x220,param_2,-1,(LPSTR)0x0,0,
                                    (LPCSTR)0x0,&local_10);
        if ((iVar6 == 0) || (local_10 != 0)) {
          uVar5 = 0xffffffff;
        }
        else {
          uVar5 = iVar6 - 1;
        }
      }
      else {
        uVar5 = FUN_0041ea5c((short *)param_2);
      }
    }
    else if (*(int *)(PTR_DAT_0042ed78 + 8) == 0) {
      uVar5 = WideCharToMultiByte(*(UINT *)PTR_DAT_0042ed78,0x220,param_2,-1,param_1,param_3,
                                  (LPCSTR)0x0,&local_10);
      if ((uVar5 == 0) || (local_10 != 0)) {
        if ((local_10 == 0) && (DVar2 = GetLastError(), DVar2 == 0x7a)) {
          while (uVar5 < param_3) {
            local_8 = WideCharToMultiByte(*(UINT *)PTR_DAT_0042ed78,0,param_2,1,local_a,2,
                                          (LPCSTR)0x0,&local_10);
            if ((local_8 == 0) || (local_10 != 0)) {
              return 0xffffffff;
            }
            if (param_3 < local_8 + uVar5) {
              return uVar5;
            }
            iVar6 = 0;
            pcVar4 = param_1 + uVar5;
            pcVar3 = local_a;
            if (0 < local_8) {
              do {
                cVar1 = *pcVar3;
                *pcVar4 = cVar1;
                if (cVar1 == '\0') {
                  return uVar5;
                }
                pcVar3 = pcVar3 + 1;
                iVar6 = iVar6 + 1;
                pcVar4 = pcVar4 + 1;
                uVar5 = uVar5 + 1;
              } while (iVar6 < local_8);
            }
            param_2 = (LPCWSTR)((int)param_2 + 2);
            local_10 = 0;
          }
        }
        else {
          uVar5 = 0xffffffff;
        }
      }
      else {
        uVar5 = uVar5 - 1;
      }
    }
    else if (param_3 != 0) {
      while ((ushort)*param_2 < 0x100) {
        param_1[uVar5] = (CHAR)*param_2;
        if ((short)*param_2 == 0) {
          return uVar5;
        }
        uVar5 = uVar5 + 1;
        param_2 = (LPCWSTR)((int)param_2 + 2);
        if (param_3 <= uVar5) {
          return uVar5;
        }
      }
      uVar5 = 0xffffffff;
    }
  }
  else {
    uVar5 = 0;
  }
  return uVar5;
}



int FUN_00421203(int param_1,uint param_2,undefined4 param_3,uint param_4,uint param_5)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  bool bVar4;
  bool bVar5;
  
  if ((param_5 == 0) && ((param_2 == 0 || (param_4 == 0)))) {
    param_1 = (int)(CONCAT44(param_2,param_1) / (ulonglong)param_4);
  }
  else {
    iVar1 = 0x40;
    uVar3 = 0;
    uVar2 = 0;
    do {
      bVar4 = param_1 < 0;
      param_1 = param_1 * 2;
      bVar5 = (int)param_2 < 0;
      param_2 = param_2 << 1 | (uint)bVar4;
      bVar4 = (int)uVar2 < 0;
      uVar2 = uVar2 << 1 | (uint)bVar5;
      uVar3 = uVar3 << 1 | (uint)bVar4;
      if ((param_5 <= uVar3) && ((param_5 < uVar3 || (param_4 <= uVar2)))) {
        bVar4 = uVar2 < param_4;
        uVar2 = uVar2 - param_4;
        uVar3 = (uVar3 - param_5) - (uint)bVar4;
        param_1 = param_1 + 1;
      }
      iVar1 = iVar1 + -1;
    } while (iVar1 != 0);
  }
  return param_1;
}



uint FUN_004212c6(int param_1,uint param_2,undefined4 param_3,uint param_4,uint param_5)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  bool bVar4;
  bool bVar5;
  
  if ((param_5 == 0) && ((param_2 == 0 || (param_4 == 0)))) {
    uVar2 = (uint)(CONCAT44(param_2,param_1) % (ulonglong)param_4);
  }
  else {
    iVar1 = 0x40;
    uVar3 = 0;
    uVar2 = 0;
    do {
      bVar4 = param_1 < 0;
      param_1 = param_1 * 2;
      bVar5 = (int)param_2 < 0;
      param_2 = param_2 << 1 | (uint)bVar4;
      bVar4 = (int)uVar2 < 0;
      uVar2 = uVar2 << 1 | (uint)bVar5;
      uVar3 = uVar3 << 1 | (uint)bVar4;
      if ((param_5 <= uVar3) && ((param_5 < uVar3 || (param_4 <= uVar2)))) {
        bVar4 = uVar2 < param_4;
        uVar2 = uVar2 - param_4;
        uVar3 = (uVar3 - param_5) - (uint)bVar4;
        param_1 = param_1 + 1;
      }
      iVar1 = iVar1 + -1;
    } while (iVar1 != 0);
  }
  return uVar2;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __stdcall FUN_00421368(void)

{
  return;
}



undefined2 __stdcall FUN_004214e8(void)

{
  undefined2 in_FPUStatusWord;
  
  return in_FPUStatusWord;
}



void __cdecl FUN_004214fc(undefined4 param_1)

{
  DAT_0042f02c = param_1;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __stdcall thunk_FUN_00421544(void)

{
                    // WARNING: Could not recover jumptable at 0x0042152c. Too many branches
                    // WARNING: Treating indirect jump as call
  (*_DAT_0042ef5c)();
  return;
}



void __stdcall thunk_FUN_00421544(void)

{
                    // WARNING: Could not recover jumptable at 0x00421532. Too many branches
                    // WARNING: Treating indirect jump as call
  (*(code *)PTR_FUN_0042ef60)();
  return;
}



void __stdcall FUN_00421544(void)

{
  FUN_00423320((uint *)s_printf___floating_point_formats_n_0042ef6c);
  return;
}



void __stdcall FUN_00421574(void)

{
  FUN_004214fc(DAT_0042f02c);
  return;
}



void __cdecl FUN_0042158c(int param_1)

{
  if ((*(byte *)(param_1 + 7) & 0x80) == 0) {
    return;
  }
  return;
}



void __cdecl FUN_004215bc(longlong *param_1,longdouble *param_2)

{
  if (*(short *)((int)param_2 + 8) != 0x403e) {
    *param_1 = (longlong)ROUND(*param_2);
    return;
  }
  *(undefined4 *)param_1 = *(undefined4 *)param_2;
  *(undefined4 *)((int)param_1 + 4) = *(undefined4 *)((int)param_2 + 4);
  return;
}



ushort __cdecl FUN_004215e0(longdouble *param_1)

{
  longdouble lVar1;
  longdouble lVar2;
  
  lVar1 = *param_1;
  lVar2 = (longdouble)0;
  return (ushort)NAN(lVar1) << 8 | (ushort)(lVar1 < lVar2) << 9 | (ushort)(lVar1 != lVar2) << 10 |
         (ushort)(lVar1 == lVar2) << 0xe;
}



// WARNING: Removing unreachable block (ram,0x00421621)

char * __cdecl
FUN_004215f4(int param_1,uint param_2,char *param_3,uint param_4,char param_5,char param_6)

{
  char cVar1;
  uint uVar2;
  undefined4 in_ECX;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  uint extraout_EDX;
  char *pcVar3;
  char *pcVar4;
  bool bVar5;
  char local_48 [68];
  
  pcVar4 = param_3;
  if ((1 < (int)param_4) && ((int)param_4 < 0x25)) {
    if ((param_2 != 0) && (((int)param_2 < 0 && (param_5 != '\0')))) {
      *param_3 = '-';
      bVar5 = param_1 != 0;
      param_1 = -param_1;
      param_2 = -(param_2 + bVar5);
      pcVar4 = param_3 + 1;
    }
    pcVar3 = local_48;
    do {
      do {
        uVar2 = FUN_004212c6(param_1,param_2,in_ECX,param_4,(int)param_4 >> 0x1f);
        *pcVar3 = (char)uVar2;
        pcVar3 = pcVar3 + 1;
        param_1 = FUN_00421203(param_1,param_2,extraout_ECX,param_4,(int)param_4 >> 0x1f);
        in_ECX = extraout_ECX_00;
        param_2 = extraout_EDX;
      } while (extraout_EDX != 0);
    } while (param_1 != 0);
    while (pcVar3 != local_48) {
      pcVar3 = pcVar3 + -1;
      cVar1 = *pcVar3;
      if (cVar1 < '\n') {
        *pcVar4 = cVar1 + '0';
        pcVar4 = pcVar4 + 1;
      }
      else {
        *pcVar4 = cVar1 + param_6 + -10;
        pcVar4 = pcVar4 + 1;
      }
    }
  }
  *pcVar4 = '\0';
  return param_3;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __cdecl FUN_004216a8(int param_1,undefined4 param_2,undefined4 param_3,ushort param_4)

{
  undefined4 uVar1;
  ushort uVar2;
  ushort uVar3;
  ushort uVar4;
  
  if (param_1 == 0) {
    uVar2 = 0x407e;
    uVar4 = 0x3f6a;
  }
  else {
    uVar2 = 0x43fe;
    uVar4 = 0x3bcd;
  }
  uVar3 = param_4 & 0x7fff;
  if (uVar3 == 0x7fff) {
    return;
  }
  if (uVar3 <= uVar2) {
    if (uVar2 == uVar3) {
      uVar1 = FUN_004214fc(0);
      FUN_004214fc(0xc00);
      FUN_004214fc(uVar1);
      return;
    }
    if (((((short)param_2 == 0 && uVar3 == 0) && param_2._2_2_ == 0) && (short)param_3 == 0) &&
        param_3._2_2_ == 0) {
      return;
    }
    if (uVar4 <= uVar3) {
      return;
    }
  }
  _DAT_0042f1d0 = 0x22;
  return;
}



char * __cdecl FUN_004217c0(uint param_1,char *param_2,uint param_3,char param_4,char param_5)

{
  char cVar1;
  char *pcVar2;
  char *pcVar3;
  char local_28 [36];
  
  pcVar3 = param_2;
  if ((1 < (int)param_3) && ((int)param_3 < 0x25)) {
    if (((int)param_1 < 0) && (param_4 != '\0')) {
      *param_2 = '-';
      pcVar3 = param_2 + 1;
      param_1 = -param_1;
    }
    pcVar2 = local_28;
    do {
      *pcVar2 = (char)(param_1 % param_3);
      pcVar2 = pcVar2 + 1;
      param_1 = param_1 / param_3;
    } while (param_1 != 0);
    while (pcVar2 != local_28) {
      pcVar2 = pcVar2 + -1;
      cVar1 = *pcVar2;
      if (cVar1 < '\n') {
        *pcVar3 = cVar1 + '0';
        pcVar3 = pcVar3 + 1;
      }
      else {
        *pcVar3 = cVar1 + param_5 + -10;
        pcVar3 = pcVar3 + 1;
      }
    }
  }
  *pcVar3 = '\0';
  return param_2;
}



void __cdecl FUN_00421834(uint param_1,char *param_2)

{
  FUN_004217c0(param_1,param_2,10,'\0','a');
  return;
}



undefined4 __cdecl FUN_00421850(undefined4 *param_1)

{
  uint uVar1;
  undefined4 uVar2;
  uint uVar3;
  
  uVar1 = param_1[1];
  uVar3 = 0;
  if (uVar1 != 0) {
    uVar3 = uVar1 % 10;
    param_1[1] = uVar1 / 10;
  }
  uVar2 = *param_1;
  *param_1 = (int)(CONCAT44(uVar3,uVar2) / 10);
  return (int)(CONCAT44(uVar3,uVar2) % 10);
}



undefined4 __cdecl FUN_00421878(uint *param_1,uint param_2)

{
  longlong lVar1;
  ulonglong uVar2;
  
  uVar2 = (ulonglong)*param_1 * 10 + (ulonglong)param_2;
  *param_1 = (uint)uVar2;
  lVar1 = (ulonglong)param_1[1] * 10 + (uVar2 >> 0x20);
  param_1[1] = (uint)lVar1;
  return (int)((ulonglong)lVar1 >> 0x20);
}



uint __cdecl FUN_004218a8(char param_1,uint param_2,uint param_3)

{
  uint uVar1;
  char *pcVar2;
  
  pcVar2 = (char *)FUN_004209a8(0xe);
  if ((param_1 == 'G') || (uVar1 = param_3, param_1 == 'g')) {
    do {
      uVar1 = param_3;
      if (*(char *)(param_3 - 1) != '0') goto LAB_004218da;
      param_3 = param_3 - 1;
    } while (param_2 < param_3);
  }
  else {
LAB_004218da:
    param_2 = uVar1;
    if (*pcVar2 == *(char *)(param_2 - 1)) {
      param_2 = param_2 - 1;
    }
  }
  return param_2;
}



byte * __cdecl
FUN_004218ec(longdouble *param_1,uint param_2,byte *param_3,byte param_4,char param_5,int param_6)

{
  char cVar1;
  byte *pbVar2;
  int iVar3;
  char *pcVar4;
  byte *pbVar5;
  uint uVar6;
  uint uVar7;
  byte *pbVar8;
  int iVar9;
  char *pcVar10;
  byte local_3c [46];
  byte local_e;
  byte local_d;
  uint local_c;
  uint local_8;
  
  pbVar2 = (byte *)FUN_004209a8(0xe);
  local_e = *pbVar2;
  if (0x28 < (int)param_2) {
    param_2 = 0x28;
  }
  local_c = param_2;
  local_d = param_4 & 0xdf;
  if (local_d == 0x46) {
    uVar6 = -param_2;
    if (0 < (int)-param_2) {
      param_2 = 0;
      uVar6 = 0;
    }
  }
  else if ((int)param_2 < 1) {
    uVar6 = 1;
  }
  else {
    uVar6 = param_2;
    if (local_d == 0x45) {
      uVar6 = param_2 + 1;
      param_2 = param_2 + 1;
    }
  }
  iVar3 = FUN_004228c4(param_1,uVar6,&local_8,(undefined4 *)local_3c,param_6);
  if (iVar3 == 0x7fff) {
    if (local_8 == 0) {
      pcVar4 = &DAT_0042f0b9;
    }
    else {
      pcVar4 = &DAT_0042f0b4;
    }
    uVar6 = 0xffffffff;
    do {
      pcVar10 = pcVar4;
      if (uVar6 == 0) break;
      uVar6 = uVar6 - 1;
      pcVar10 = pcVar4 + 1;
      cVar1 = *pcVar4;
      pcVar4 = pcVar10;
    } while (cVar1 != '\0');
    uVar6 = ~uVar6;
    pbVar2 = (byte *)(pcVar10 + -uVar6);
    pbVar5 = param_3;
    for (uVar7 = uVar6 >> 2; uVar7 != 0; uVar7 = uVar7 - 1) {
      *(undefined4 *)pbVar5 = *(undefined4 *)pbVar2;
      pbVar2 = pbVar2 + 4;
      pbVar5 = pbVar5 + 4;
    }
    for (uVar6 = uVar6 & 3; uVar6 != 0; uVar6 = uVar6 - 1) {
      *pbVar5 = *pbVar2;
      pbVar2 = pbVar2 + 1;
      pbVar5 = pbVar5 + 1;
    }
    return param_3;
  }
  if (iVar3 == 0x7ffe) {
    if (local_8 == 0) {
      pcVar4 = &DAT_0042f0c3;
    }
    else {
      pcVar4 = &DAT_0042f0be;
    }
    uVar6 = 0xffffffff;
    do {
      pcVar10 = pcVar4;
      if (uVar6 == 0) break;
      uVar6 = uVar6 - 1;
      pcVar10 = pcVar4 + 1;
      cVar1 = *pcVar4;
      pcVar4 = pcVar10;
    } while (cVar1 != '\0');
    uVar6 = ~uVar6;
    pbVar2 = (byte *)(pcVar10 + -uVar6);
    pbVar5 = param_3;
    for (uVar7 = uVar6 >> 2; uVar7 != 0; uVar7 = uVar7 - 1) {
      *(undefined4 *)pbVar5 = *(undefined4 *)pbVar2;
      pbVar2 = pbVar2 + 4;
      pbVar5 = pbVar5 + 4;
    }
    for (uVar6 = uVar6 & 3; uVar6 != 0; uVar6 = uVar6 - 1) {
      *pbVar5 = *pbVar2;
      pbVar2 = pbVar2 + 1;
      pbVar5 = pbVar5 + 1;
    }
    return param_3;
  }
  pbVar2 = param_3;
  if (local_8 != 0) {
    *param_3 = 0x2d;
    pbVar2 = param_3 + 1;
  }
  if (local_d != 0x46) {
    if ((local_d != 0x47) || (iVar3 < -3)) goto LAB_00421ab6;
    uVar6 = param_2;
    if (param_2 == 0) {
      uVar6 = 1;
    }
    if ((int)uVar6 < iVar3) goto LAB_00421ab6;
  }
  if (iVar3 < 0x29) {
    if (iVar3 < 1) {
      *pbVar2 = 0x30;
      pbVar2[1] = local_e;
      pbVar2 = pbVar2 + 2;
      for (iVar9 = iVar3; iVar3 = 0, iVar9 != 0; iVar9 = iVar9 + 1) {
        *pbVar2 = 0x30;
        pbVar2 = pbVar2 + 1;
      }
    }
    iVar9 = 0;
    for (pbVar5 = local_3c; *pbVar5 != 0; pbVar5 = pbVar5 + 1) {
      *pbVar2 = *pbVar5;
      pbVar8 = pbVar2 + 1;
      iVar3 = iVar3 + -1;
      if (iVar3 == 0) {
        *pbVar8 = local_e;
        pbVar8 = pbVar2 + 2;
        iVar9 = iVar9 + 1;
      }
      pbVar2 = pbVar8;
    }
    if ((int)(iVar9 + local_c) < (int)param_2) {
      uVar6 = param_2 - (iVar9 + local_c);
      pbVar5 = (byte *)FUN_0041e880((undefined4 *)pbVar2,0x30,uVar6);
      pbVar2 = pbVar2 + uVar6;
    }
    else if ((iVar3 != 1) && (param_5 == '\0')) {
      pbVar5 = (byte *)FUN_004218a8(param_4,(uint)param_3,(uint)pbVar2);
      pbVar2 = pbVar5;
    }
    if (pbVar2 == param_3) {
      *pbVar2 = 0x30;
      pbVar2 = pbVar2 + 1;
    }
    *pbVar2 = 0;
    return pbVar5;
  }
LAB_00421ab6:
  *pbVar2 = local_3c[0];
  pbVar5 = pbVar2 + 1;
  if (local_3c[1] == 0) {
    if (param_5 != '\0') {
      *pbVar5 = local_e;
      pbVar5 = pbVar2 + 2;
    }
  }
  else {
    *pbVar5 = local_e;
    pbVar5 = pbVar2 + 2;
    pbVar2 = local_3c + 2;
    while (local_3c[1] != 0) {
      *pbVar5 = local_3c[1];
      pbVar5 = pbVar5 + 1;
      local_3c[1] = *pbVar2;
      pbVar2 = pbVar2 + 1;
    }
    if (param_5 == '\0') {
      pbVar5 = (byte *)FUN_004218a8(param_4,(uint)param_3,(uint)pbVar5);
    }
  }
  *pbVar5 = param_4 & 0x20 | 0x45;
  pbVar2 = (byte *)(iVar3 + -1);
  if ((int)pbVar2 < 0) {
    pbVar2 = (byte *)-(int)pbVar2;
    pbVar5[1] = 0x2d;
  }
  else {
    pbVar5[1] = 0x2b;
  }
  if ((int)pbVar2 < 1000) {
    if ((int)pbVar2 < 100) {
      iVar3 = 2;
    }
    else {
      iVar3 = 3;
    }
  }
  else {
    iVar3 = 4;
  }
  pbVar5[iVar3 + 2] = 0;
  pbVar5 = pbVar5 + iVar3 + 2;
  pbVar8 = pbVar5;
  for (; iVar3 != 0; iVar3 = iVar3 + -1) {
    pbVar5 = pbVar5 + -1;
    *pbVar5 = (char)((int)pbVar2 % 10) + 0x30;
    pbVar8 = (byte *)((int)pbVar2 / 10);
    pbVar2 = pbVar8;
  }
  return pbVar8;
}



int __cdecl FUN_00421b74(int param_1,int param_2)

{
  int iVar1;
  
  if (param_2 == 0) {
    iVar1 = param_1 + 8;
  }
  else {
    iVar1 = param_1 + 0xc;
  }
  return iVar1;
}



uint __cdecl FUN_00421ba4(short param_1,uint param_2,uint param_3)

{
  uint uVar1;
  short *psVar2;
  
  psVar2 = (short *)FUN_004209a8(0xe);
  if ((param_1 == 0x47) || (uVar1 = param_3, param_1 == 0x67)) {
    do {
      uVar1 = param_3;
      if (*(short *)(param_3 - 2) != 0x30) goto LAB_00421bdb;
      param_3 = param_3 - 2;
    } while (param_2 < param_3);
  }
  else {
LAB_00421bdb:
    param_2 = uVar1;
    if (*psVar2 == *(short *)(param_2 - 2)) {
      param_2 = param_2 - 2;
    }
  }
  return param_2;
}



void __cdecl
FUN_00421bf0(longdouble *param_1,int param_2,ushort *param_3,ushort param_4,short param_5,
            int param_6)

{
  ushort *puVar1;
  int iVar2;
  wchar16 *pwVar3;
  ushort *puVar4;
  uint uVar5;
  ushort *puVar6;
  int iVar7;
  ushort local_68 [44];
  ushort local_10;
  ushort local_e;
  int local_c;
  uint local_8;
  
  puVar1 = (ushort *)FUN_004209a8(0xe);
  local_10 = *puVar1;
  if (0x28 < param_2) {
    param_2 = 0x28;
  }
  local_c = param_2;
  local_e = param_4 & 0xdf;
  if (local_e == 0x46) {
    iVar2 = -param_2;
    if (0 < -param_2) {
      param_2 = 0;
      iVar2 = 0;
    }
  }
  else if (param_2 < 1) {
    iVar2 = 1;
  }
  else {
    iVar2 = param_2;
    if (local_e == 0x45) {
      iVar2 = param_2 + 1;
      param_2 = param_2 + 1;
    }
  }
  iVar2 = FUN_00422b98(param_1,iVar2,&local_8,local_68,param_6);
  if (iVar2 == 0x7fff) {
    if (local_8 == 0) {
      pwVar3 = u__INF_0042f0d2;
    }
    else {
      pwVar3 = u__INF_0042f0c8;
    }
    FUN_0041ea74((short *)param_3,pwVar3);
    return;
  }
  if (iVar2 == 0x7ffe) {
    if (local_8 == 0) {
      pwVar3 = u__NAN_0042f0e6;
    }
    else {
      pwVar3 = u__NAN_0042f0dc;
    }
    FUN_0041ea74((short *)param_3,pwVar3);
    return;
  }
  puVar1 = param_3;
  if (local_8 != 0) {
    *param_3 = 0x2d;
    puVar1 = param_3 + 1;
  }
  if (local_e != 0x46) {
    if ((local_e != 0x47) || (iVar2 < -3)) goto LAB_00421dbb;
    iVar7 = param_2;
    if (param_2 == 0) {
      iVar7 = 1;
    }
    if (iVar7 < iVar2) goto LAB_00421dbb;
  }
  if (iVar2 < 0x29) {
    if (iVar2 < 1) {
      *puVar1 = 0x30;
      puVar1[1] = local_10;
      puVar1 = puVar1 + 2;
      for (iVar7 = iVar2; iVar2 = 0, iVar7 != 0; iVar7 = iVar7 + 1) {
        *puVar1 = 0x30;
        puVar1 = puVar1 + 1;
      }
    }
    iVar7 = 0;
    for (puVar4 = local_68; *puVar4 != 0; puVar4 = puVar4 + 1) {
      *puVar1 = *puVar4;
      puVar6 = puVar1 + 1;
      iVar2 = iVar2 + -1;
      if (iVar2 == 0) {
        *puVar6 = local_10;
        puVar6 = puVar1 + 2;
        iVar7 = iVar7 + 1;
      }
      puVar1 = puVar6;
    }
    if (iVar7 + local_c < param_2) {
      uVar5 = param_2 - (iVar7 + local_c);
      FUN_0041e880((undefined4 *)puVar1,0x30,uVar5);
      puVar1 = puVar1 + uVar5;
    }
    else if ((iVar2 != 1) && (param_5 == 0)) {
      puVar1 = (ushort *)FUN_00421ba4(param_4,(uint)param_3,(uint)puVar1);
    }
    if (puVar1 == param_3) {
      *puVar1 = 0x30;
      puVar1 = puVar1 + 1;
    }
    *puVar1 = 0;
    return;
  }
LAB_00421dbb:
  *puVar1 = local_68[0];
  puVar4 = puVar1 + 1;
  if (local_68[1] == 0) {
    if (param_5 != 0) {
      *puVar4 = local_10;
      puVar4 = puVar1 + 2;
    }
  }
  else {
    *puVar4 = local_10;
    puVar4 = puVar1 + 2;
    puVar1 = local_68 + 2;
    while (local_68[1] != 0) {
      *puVar4 = local_68[1];
      puVar4 = puVar4 + 1;
      local_68[1] = *puVar1;
      puVar1 = puVar1 + 1;
    }
    if (param_5 == 0) {
      puVar4 = (ushort *)FUN_00421ba4(param_4,(uint)param_3,(uint)puVar4);
    }
  }
  *puVar4 = param_4 & 0x20 | 0x45;
  iVar2 = iVar2 + -1;
  if (iVar2 < 0) {
    iVar2 = -iVar2;
    puVar4[1] = 0x2d;
  }
  else {
    puVar4[1] = 0x2b;
  }
  if (iVar2 < 1000) {
    if (iVar2 < 100) {
      iVar7 = 2;
    }
    else {
      iVar7 = 3;
    }
  }
  else {
    iVar7 = 4;
  }
  puVar4[iVar7 + 2] = 0;
  puVar4 = puVar4 + iVar7 + 2;
  for (; iVar7 != 0; iVar7 = iVar7 + -1) {
    puVar4 = puVar4 + -1;
    *puVar4 = (short)(iVar2 % 10) + 0x30;
    iVar2 = iVar2 / 10;
  }
  return;
}



int __cdecl FUN_00421ea8(int param_1,int param_2)

{
  int iVar1;
  
  if (param_2 == 0) {
    iVar1 = param_1 + 8;
  }
  else {
    iVar1 = param_1 + 0xc;
  }
  return iVar1;
}



void __cdecl
FUN_00421ed8(undefined4 *param_1,undefined *param_2,undefined *param_3,uint param_4,int param_5,
            int *param_6,undefined4 *param_7)

{
  byte bVar1;
  bool bVar2;
  byte *pbVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  longdouble in_ST0;
  longdouble in_ST1;
  uint uVar11;
  uint local_40;
  undefined4 local_3c;
  undefined2 *local_38;
  undefined1 local_28 [8];
  undefined2 uStack_20;
  undefined4 local_1c;
  int local_18;
  char local_11;
  char local_10;
  char local_f;
  char local_e;
  char local_d;
  int local_c;
  int local_8;
  
  local_38 = (undefined2 *)local_28;
  local_8 = 0x8000;
  local_c = 0x13;
  local_d = '\0';
  local_e = '\x01';
  local_f = '\0';
  local_10 = '\0';
  local_11 = '\0';
  local_18 = 0;
  iVar9 = -2;
  local_1c = 1;
  _local_28 = (longdouble)(unkuint10)0;
  uVar11 = 0xe;
  pbVar3 = (byte *)FUN_004209a8(0xe);
  bVar1 = *pbVar3;
  uVar6 = (uint)bVar1;
  do {
    local_18 = local_18 + 1;
    uVar5 = param_4;
    uVar4 = (*(code *)param_2)(uVar6,param_4,uVar11);
    uVar11 = uVar5;
    uVar5 = uVar4 + 1;
    if (uVar5 == 0) {
      local_1c = 0xffffffff;
      goto LAB_0042220c;
    }
    if ((uVar4 & 0x80) != 0) break;
    uVar11 = uVar4;
    uVar6 = FUN_00422fc8(uVar4);
    uVar5 = 0;
  } while (uVar6 != 0);
  param_5 = param_5 + -1;
  if (-1 < param_5) {
    iVar10 = local_c;
    if (uVar4 == 0x2b) {
      local_f = '\x01';
    }
    else {
      iVar8 = iVar9;
      if (uVar4 != 0x2d) goto LAB_00421f99;
      local_f = '\x01';
      local_d = '\x01';
    }
    while (local_c = iVar10, param_5 = param_5 + -1, -1 < param_5) {
      local_18 = local_18 + 1;
      uVar11 = param_4;
      uVar5 = (*(code *)param_2)();
      uVar4 = uVar5;
      iVar8 = iVar9;
LAB_00421f99:
      if ((local_e == '\x01') && (local_f != '\0')) {
        if (uVar4 == 0x49) {
          local_18 = local_18 + 1;
          iVar9 = (*(code *)param_2)(uVar5,param_4,uVar11);
          iVar10 = param_5 + -1;
          if ((iVar10 < 0) || (iVar9 != 0x4e)) goto LAB_00422207;
          local_18 = local_18 + 1;
          iVar9 = (*(code *)param_2)();
          iVar10 = param_5 + -2;
          if ((iVar10 < 0) || (iVar9 != 0x46)) goto LAB_00422207;
          if (local_d == '\0') {
            local_28 = (undefined1  [8])CONCAT44(DAT_0042f0f4,DAT_0042f0f0);
            _local_28 = (longdouble)CONCAT28(DAT_0042f0f8,local_28);
          }
          else {
            local_28 = (undefined1  [8])CONCAT44(DAT_0042f0fe,DAT_0042f0fa);
            _local_28 = (longdouble)CONCAT28(DAT_0042f102,local_28);
          }
          goto LAB_004221dc;
        }
        if (uVar4 == 0x4e) {
          local_18 = local_18 + 1;
          iVar9 = (*(code *)param_2)();
          iVar10 = param_5 + -1;
          if ((iVar10 < 0) || (iVar9 != 0x41)) goto LAB_00422207;
          local_18 = local_18 + 1;
          iVar9 = (*(code *)param_2)();
          iVar10 = param_5 + -2;
          if ((iVar10 < 0) || (iVar9 != 0x4e)) goto LAB_00422207;
          if (local_d == '\0') {
            local_28 = (undefined1  [8])CONCAT44(DAT_0042f108,DAT_0042f104);
            _local_28 = (longdouble)CONCAT28(DAT_0042f10c,local_28);
          }
          else {
            local_28 = (undefined1  [8])CONCAT44(DAT_0042f112,DAT_0042f10e);
            _local_28 = (longdouble)CONCAT28(DAT_0042f116,local_28);
          }
          goto LAB_004221dc;
        }
      }
      local_e = '\0';
      iVar9 = iVar8;
      iVar10 = local_c;
      if (uVar4 == (int)(char)bVar1) {
        if (local_8 != 0x8000) goto LAB_0042207f;
        local_8 = iVar8;
        if (iVar8 < 1) {
          local_8 = 0;
        }
      }
      else {
        if (((int)uVar4 < 0x30) || (0x39 < (int)uVar4)) goto LAB_0042207f;
        uVar4 = uVar4 - 0x30;
        iVar9 = iVar8 + 1;
        if (iVar9 < 1) {
          *local_38 = (short)uVar4;
          iVar10 = local_c;
          if (uVar4 == 0) {
            iVar9 = -1;
            if (local_8 != 0x8000) {
              local_8 = local_8 + -1;
            }
          }
          else {
            iVar9 = 1;
          }
        }
        else if (iVar9 < 10) {
          _local_28 = (longdouble)CONCAT64(stack0xffffffdc,local_28._0_4_ * 10 + uVar4);
        }
        else if (iVar9 <= local_c) {
          local_40 = local_28._0_4_;
          local_3c = local_28._4_4_;
          iVar7 = FUN_00421878(&local_40,uVar4);
          iVar10 = iVar8;
          if (iVar7 == 0) {
            local_28 = (undefined1  [8])CONCAT44(local_3c,local_40);
            iVar10 = local_c;
          }
        }
      }
    }
  }
  uVar4 = 0x65;
LAB_0042207f:
  iVar10 = param_5;
  if (iVar9 == -2) {
LAB_00422207:
    param_5 = iVar10;
    local_1c = 0;
LAB_0042220c:
    if (-1 < param_5) {
      (*(code *)param_3)();
      local_18 = local_18 + -1;
    }
    _local_28 = (longdouble)(unkuint10)0;
  }
  else {
    if (local_8 == 0x8000) {
      local_8 = iVar9;
    }
    iVar10 = 0;
    if ((uVar4 == 0x65) || (uVar4 == 0x45)) {
      bVar2 = true;
LAB_004220a7:
      while (param_5 = param_5 + -1, -1 < param_5) {
        local_18 = local_18 + 1;
        iVar8 = (*(code *)param_2)();
        if (!bVar2) goto LAB_004220d4;
        bVar2 = false;
        if (iVar8 != 0x2d) goto LAB_004220cf;
        local_10 = '\x01';
      }
    }
    else {
LAB_004220fa:
      (*(code *)param_3)();
      local_18 = local_18 + -1;
    }
    if (local_10 != '\0') {
      iVar10 = -iVar10;
      local_11 = -local_11;
    }
    if (iVar9 < 0) {
      _local_28 = (longdouble)(unkuint10)0;
    }
    else if (local_11 == '\0') {
      if (local_c < iVar9) {
        iVar9 = local_c;
      }
      iVar10 = iVar10 + (local_8 - iVar9);
      FUN_0042158c((int)local_28);
      _local_28 = in_ST0;
      if (iVar10 != 0) {
        FUN_00421368();
        if (iVar10 < 0) {
          _local_28 = _local_28 / in_ST1;
        }
        else {
          _local_28 = in_ST1 * _local_28;
        }
      }
    }
    else {
      if (local_11 == '\x01') {
        local_38[3] = 0xffff;
        local_38[2] = 0xffff;
        local_38[1] = 0xffff;
        *local_38 = 0xffff;
        local_38[4] = 0x7ffe;
      }
      else {
        _local_28 = (longdouble)(unkuint10)0;
      }
      local_1c = 2;
    }
  }
  if (local_d != '\0') {
    _local_28 = -_local_28;
  }
LAB_004221dc:
  *param_6 = *param_6 + local_18;
  *param_7 = local_1c;
  *param_1 = local_28._0_4_;
  param_1[1] = local_28._4_4_;
  *(undefined2 *)(param_1 + 2) = uStack_20;
  return;
LAB_004220cf:
  if (iVar8 != 0x2b) {
LAB_004220d4:
    if ((iVar8 < 0x30) || (0x39 < iVar8)) goto LAB_004220fa;
    iVar10 = iVar8 + iVar10 * 10 + -0x30;
    if (0x1344 < iVar10) {
      local_11 = '\x01';
    }
  }
  goto LAB_004220a7;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __cdecl FUN_00422338(float *param_1,double *param_2,uint param_3)

{
  longdouble in_ST0;
  
  if ((param_3 & 4) == 0) {
    if ((param_3 & 8) == 0) {
      FUN_004216a8(0,*param_1,param_1[1],*(ushort *)(param_1 + 2));
      *(float *)param_2 = (float)in_ST0;
    }
    else {
      *(float *)param_2 = *param_1;
      *(float *)((int)param_2 + 4) = param_1[1];
      *(undefined2 *)(param_2 + 1) = *(undefined2 *)(param_1 + 2);
    }
  }
  else {
    FUN_004216a8(1,*param_1,param_1[1],*(ushort *)(param_1 + 2));
    *param_2 = (double)in_ST0;
  }
  return;
}



void __cdecl
FUN_004223d0(undefined4 *param_1,undefined *param_2,undefined *param_3,uint param_4,int param_5,
            int *param_6,undefined4 *param_7)

{
  ushort uVar1;
  bool bVar2;
  ushort *puVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  longdouble in_ST0;
  longdouble in_ST1;
  uint uVar10;
  uint local_40;
  undefined4 local_3c;
  undefined2 *local_38;
  undefined1 local_28 [8];
  undefined2 uStack_20;
  undefined4 local_1c;
  int local_18;
  char local_11;
  char local_10;
  char local_f;
  char local_e;
  char local_d;
  int local_c;
  int local_8;
  
  local_38 = (undefined2 *)local_28;
  local_8 = 0x8000;
  local_c = 0x13;
  local_d = '\0';
  local_e = '\x01';
  local_f = '\0';
  local_10 = '\0';
  local_11 = '\0';
  local_18 = 0;
  iVar8 = -2;
  local_1c = 1;
  _local_28 = (longdouble)(unkuint10)0;
  uVar10 = 0xe;
  puVar3 = (ushort *)FUN_004209a8(0xe);
  uVar1 = *puVar3;
  uVar5 = (uint)uVar1;
  do {
    local_18 = local_18 + 1;
    uVar4 = (*(code *)param_2)(uVar5,param_4,uVar10,param_4);
    if (uVar4 == 0xffffffff) {
      local_1c = 0xffffffff;
      goto LAB_00422701;
    }
    uVar10 = uVar4;
    uVar5 = FUN_00420bc4((ushort)uVar4);
  } while (uVar5 != 0);
  param_5 = param_5 + -1;
  if (-1 < param_5) {
    iVar9 = local_c;
    if (uVar4 == 0x2b) {
      local_f = '\x01';
    }
    else {
      uVar5 = 0;
      iVar7 = iVar8;
      if (uVar4 != 0x2d) goto LAB_0042248e;
      local_f = '\x01';
      local_d = '\x01';
    }
    while (local_c = iVar9, param_5 = param_5 + -1, -1 < param_5) {
      local_18 = local_18 + 1;
      uVar10 = param_4;
      uVar5 = (*(code *)param_2)();
      uVar4 = uVar5;
      iVar7 = iVar8;
LAB_0042248e:
      if ((local_e == '\x01') && (local_f != '\0')) {
        if (uVar4 == 0x49) {
          local_18 = local_18 + 1;
          iVar8 = (*(code *)param_2)(uVar5,param_4,uVar10);
          iVar9 = param_5 + -1;
          if ((iVar9 < 0) || (iVar8 != 0x4e)) goto LAB_004226fc;
          local_18 = local_18 + 1;
          iVar8 = (*(code *)param_2)();
          iVar9 = param_5 + -2;
          if ((iVar9 < 0) || (iVar8 != 0x46)) goto LAB_004226fc;
          if (local_d == '\0') {
            local_28 = (undefined1  [8])CONCAT44(DAT_0042f11c,DAT_0042f118);
            _local_28 = (longdouble)CONCAT28(DAT_0042f120,local_28);
          }
          else {
            local_28 = (undefined1  [8])CONCAT44(DAT_0042f126,DAT_0042f122);
            _local_28 = (longdouble)CONCAT28(DAT_0042f12a,local_28);
          }
          goto LAB_004226d1;
        }
        if (uVar4 == 0x4e) {
          local_18 = local_18 + 1;
          iVar8 = (*(code *)param_2)();
          iVar9 = param_5 + -1;
          if ((iVar9 < 0) || (iVar8 != 0x41)) goto LAB_004226fc;
          local_18 = local_18 + 1;
          iVar8 = (*(code *)param_2)();
          iVar9 = param_5 + -2;
          if ((iVar9 < 0) || (iVar8 != 0x4e)) goto LAB_004226fc;
          if (local_d == '\0') {
            local_28 = (undefined1  [8])CONCAT44(DAT_0042f130,DAT_0042f12c);
            _local_28 = (longdouble)CONCAT28(DAT_0042f134,local_28);
          }
          else {
            local_28 = (undefined1  [8])CONCAT44(DAT_0042f13a,DAT_0042f136);
            _local_28 = (longdouble)CONCAT28(DAT_0042f13e,local_28);
          }
          goto LAB_004226d1;
        }
      }
      local_e = '\0';
      iVar8 = iVar7;
      iVar9 = local_c;
      if (uVar4 == uVar1) {
        if (local_8 != 0x8000) goto LAB_00422574;
        local_8 = iVar7;
        if (iVar7 < 1) {
          local_8 = 0;
        }
      }
      else {
        if (((int)uVar4 < 0x30) || (0x39 < (int)uVar4)) goto LAB_00422574;
        uVar4 = uVar4 - 0x30;
        iVar8 = iVar7 + 1;
        if (iVar8 < 1) {
          *local_38 = (short)uVar4;
          iVar9 = local_c;
          if (uVar4 == 0) {
            iVar8 = -1;
            if (local_8 != 0x8000) {
              local_8 = local_8 + -1;
            }
          }
          else {
            iVar8 = 1;
          }
        }
        else if (iVar8 < 10) {
          _local_28 = (longdouble)CONCAT64(stack0xffffffdc,local_28._0_4_ * 10 + uVar4);
        }
        else if (iVar8 <= local_c) {
          local_40 = local_28._0_4_;
          local_3c = local_28._4_4_;
          iVar6 = FUN_00421878(&local_40,uVar4);
          iVar9 = iVar7;
          if (iVar6 == 0) {
            local_28 = (undefined1  [8])CONCAT44(local_3c,local_40);
            iVar9 = local_c;
          }
        }
      }
    }
  }
  uVar4 = 0x65;
LAB_00422574:
  iVar9 = param_5;
  if (iVar8 == -2) {
LAB_004226fc:
    param_5 = iVar9;
    local_1c = 0;
LAB_00422701:
    if (-1 < param_5) {
      (*(code *)param_3)();
      local_18 = local_18 + -1;
    }
    _local_28 = (longdouble)(unkuint10)0;
  }
  else {
    if (local_8 == 0x8000) {
      local_8 = iVar8;
    }
    iVar9 = 0;
    if ((uVar4 == 0x65) || (uVar4 == 0x45)) {
      bVar2 = true;
LAB_0042259c:
      while (param_5 = param_5 + -1, -1 < param_5) {
        local_18 = local_18 + 1;
        iVar7 = (*(code *)param_2)();
        if (!bVar2) goto LAB_004225c9;
        bVar2 = false;
        if (iVar7 != 0x2d) goto LAB_004225c4;
        local_10 = '\x01';
      }
    }
    else {
LAB_004225ef:
      (*(code *)param_3)();
      local_18 = local_18 + -1;
    }
    if (local_10 != '\0') {
      iVar9 = -iVar9;
      local_11 = -local_11;
    }
    if (iVar8 < 0) {
      _local_28 = (longdouble)(unkuint10)0;
    }
    else if (local_11 == '\0') {
      if (local_c < iVar8) {
        iVar8 = local_c;
      }
      iVar9 = iVar9 + (local_8 - iVar8);
      FUN_0042158c((int)local_28);
      _local_28 = in_ST0;
      if (iVar9 != 0) {
        FUN_00421368();
        if (iVar9 < 0) {
          _local_28 = _local_28 / in_ST1;
        }
        else {
          _local_28 = in_ST1 * _local_28;
        }
      }
    }
    else {
      if (local_11 == '\x01') {
        local_38[3] = 0xffff;
        local_38[2] = 0xffff;
        local_38[1] = 0xffff;
        *local_38 = 0xffff;
        local_38[4] = 0x7ffe;
      }
      else {
        _local_28 = (longdouble)(unkuint10)0;
      }
      local_1c = 2;
    }
  }
  if (local_d != '\0') {
    _local_28 = -_local_28;
  }
LAB_004226d1:
  *param_6 = *param_6 + local_18;
  *param_7 = local_1c;
  *param_1 = local_28._0_4_;
  param_1[1] = local_28._4_4_;
  *(undefined2 *)(param_1 + 2) = uStack_20;
  return;
LAB_004225c4:
  if (iVar7 != 0x2b) {
LAB_004225c9:
    if ((iVar7 < 0x30) || (0x39 < iVar7)) goto LAB_004225ef;
    iVar9 = iVar7 + iVar9 * 10 + -0x30;
    if (0x1344 < iVar9) {
      local_11 = '\x01';
    }
  }
  goto LAB_0042259c;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __cdecl FUN_0042282c(float *param_1,double *param_2,uint param_3)

{
  longdouble in_ST0;
  
  if ((param_3 & 4) == 0) {
    if ((param_3 & 8) == 0) {
      FUN_004216a8(0,*param_1,param_1[1],*(ushort *)(param_1 + 2));
      *(float *)param_2 = (float)in_ST0;
    }
    else {
      *(float *)param_2 = *param_1;
      *(float *)((int)param_2 + 4) = param_1[1];
      *(undefined2 *)(param_2 + 1) = *(undefined2 *)(param_1 + 2);
    }
  }
  else {
    FUN_004216a8(1,*param_1,param_1[1],*(ushort *)(param_1 + 2));
    *param_2 = (double)in_ST0;
  }
  return;
}



// WARNING: Restarted to delay deadcode elimination for space: stack

int __cdecl
FUN_004228c4(longdouble *param_1,uint param_2,uint *param_3,undefined4 *param_4,int param_5)

{
  ushort uVar1;
  uint uVar2;
  undefined4 uVar3;
  int iVar4;
  char *pcVar5;
  byte bVar6;
  int iVar7;
  uint uVar8;
  longdouble in_ST0;
  longdouble in_ST1;
  longdouble in_ST2;
  longdouble in_ST3;
  longdouble in_ST4;
  longdouble in_ST5;
  longdouble in_ST6;
  undefined4 *local_28;
  int local_18;
  undefined1 local_10 [8];
  ushort uStack_8;
  short local_6;
  
  local_6 = 10;
  if (param_5 == 2) {
    _local_10 = (longdouble)*(float *)param_1;
  }
  else if (param_5 == 6) {
    _local_10 = (longdouble)*(double *)param_1;
  }
  else if (param_5 == 8) {
    _local_10 = *param_1;
  }
  uVar1 = uStack_8;
  uVar2 = (uint)uStack_8;
  _local_10 = (longdouble)CONCAT28(uStack_8 & 0x7fff,local_10);
  *param_3 = (uint)((uVar1 & 0x8000) != 0);
  uVar1 = FUN_004215e0((longdouble *)local_10);
  uVar1 = uVar1 & 0x4700;
  if (uVar1 != 0x4000) {
    if (uVar1 == 0x500) {
      return 0x7fff;
    }
    if (uVar1 == 0x100) {
      return 0x7ffe;
    }
    uVar2 = ((uVar2 & 0x7fff) - 0x3fff) * 0x4d10 + ((uint)local_10[7] * 2 & 0xff) * 0x4d;
    local_18 = (int)uVar2 >> 0x10;
    if ((uVar2 & 0xffff) != 0) {
      local_18 = local_18 + 1;
    }
    uVar2 = param_2;
    if ((0 < (int)param_2) || (uVar2 = local_18 - param_2, -1 < (int)uVar2)) {
      if (0x13 < (int)uVar2) {
        uVar2 = 0x13;
      }
      for (iVar7 = uVar2 - local_18; iVar7 != 0; iVar7 = iVar7 + iVar4) {
        iVar4 = iVar7;
        if (iVar7 < 0) {
          iVar4 = -iVar7;
        }
        if (0x1344 < iVar4) {
          iVar4 = 0x1344;
        }
        FUN_00421368();
        if (iVar7 < 0) {
          _local_10 = _local_10 / in_ST0;
        }
        else {
          _local_10 = _local_10 * in_ST0;
          iVar4 = -iVar4;
        }
        in_ST0 = in_ST1;
        in_ST1 = in_ST2;
        in_ST2 = in_ST3;
        in_ST3 = in_ST4;
        in_ST4 = in_ST5;
        in_ST5 = in_ST6;
      }
      FUN_00421368();
      if (_local_10 <= in_ST0) {
        FUN_00421368();
        uVar8 = uVar2;
        if (_local_10 < in_ST1) {
          local_18 = local_18 + -1;
          uVar8 = uVar2 - 1;
          if (0 < (int)param_2) {
            _local_10 = _local_10 * (longdouble)(int)local_6;
            uVar8 = uVar2;
          }
        }
      }
      else {
        local_18 = local_18 + 1;
        uVar8 = uVar2 + 1;
        if (((int)uVar8 < 0x14) && (0 < (int)param_2)) {
          _local_10 = _local_10 / (longdouble)(int)local_6;
          uVar8 = uVar2;
        }
      }
      if (-1 < (int)uVar8) {
        FUN_004215bc((longlong *)local_10,(longdouble *)local_10);
        local_28 = (undefined4 *)((int)param_4 + uVar8);
        bVar6 = 0;
        *(undefined1 *)local_28 = 0;
        pcVar5 = (char *)((int)local_28 + -1);
        if (uVar8 == 0) {
          if (local_10[0] != '\x01') goto LAB_0042294d;
          bVar6 = 0;
        }
        else {
          do {
            uVar3 = FUN_00421850((undefined4 *)local_10);
            bVar6 = bVar6 | (byte)uVar3;
            *pcVar5 = (byte)uVar3 + 0x30;
            pcVar5 = pcVar5 + -1;
            uVar8 = uVar8 - 1;
          } while (uVar8 != 0);
        }
        if (bVar6 == 0) {
          local_18 = local_18 + 1;
          if ((int)param_2 < 1) {
            *(undefined1 *)local_28 = 0x30;
          }
          local_28 = (undefined4 *)((int)local_28 + 1);
          pcVar5[1] = '1';
        }
        if ((int)param_2 < 1) {
          param_2 = local_18 - param_2;
        }
        if (0x28 < (int)param_2) {
          param_2 = 0x28;
        }
        *(undefined1 *)local_28 = 0;
        uVar2 = param_2 - ((int)local_28 - (int)param_4);
        if ((int)uVar2 < 1) {
          return local_18;
        }
        FUN_0041e880(local_28,0x30,uVar2);
        *(undefined1 *)((int)local_28 + uVar2) = 0;
        return local_18;
      }
    }
  }
LAB_0042294d:
  if ((int)param_2 < 1) {
    param_2 = 1 - param_2;
  }
  if (0x28 < (int)param_2) {
    param_2 = 0x28;
  }
  FUN_0041e880(param_4,0x30,param_2);
  *(undefined1 *)((int)param_4 + param_2) = 0;
  return 1;
}



// WARNING: Restarted to delay deadcode elimination for space: stack

int __cdecl
FUN_00422b98(longdouble *param_1,int param_2,uint *param_3,undefined2 *param_4,int param_5)

{
  ushort uVar1;
  uint uVar2;
  undefined4 uVar3;
  int iVar4;
  short *psVar5;
  byte bVar6;
  int iVar7;
  int iVar8;
  longdouble in_ST0;
  longdouble in_ST1;
  longdouble in_ST2;
  longdouble in_ST3;
  longdouble in_ST4;
  longdouble in_ST5;
  longdouble in_ST6;
  undefined2 *local_28;
  int local_18;
  undefined1 local_10 [8];
  ushort uStack_8;
  short local_6;
  
  local_6 = 10;
  if (param_5 == 2) {
    _local_10 = (longdouble)*(float *)param_1;
  }
  else if (param_5 == 6) {
    _local_10 = (longdouble)*(double *)param_1;
  }
  else if (param_5 == 8) {
    _local_10 = *param_1;
  }
  uVar1 = uStack_8;
  uVar2 = (uint)uStack_8;
  _local_10 = (longdouble)CONCAT28(uStack_8 & 0x7fff,local_10);
  *param_3 = (uint)((uVar1 & 0x8000) != 0);
  uVar1 = FUN_004215e0((longdouble *)local_10);
  uVar1 = uVar1 & 0x4700;
  if (uVar1 != 0x4000) {
    if (uVar1 == 0x500) {
      return 0x7fff;
    }
    if (uVar1 == 0x100) {
      return 0x7ffe;
    }
    uVar2 = ((uVar2 & 0x7fff) - 0x3fff) * 0x4d10 + ((uint)local_10[7] * 2 & 0xff) * 0x4d;
    local_18 = (int)uVar2 >> 0x10;
    if ((uVar2 & 0xffff) != 0) {
      local_18 = local_18 + 1;
    }
    iVar8 = param_2;
    if ((0 < param_2) || (iVar8 = local_18 - param_2, -1 < iVar8)) {
      if (0x13 < iVar8) {
        iVar8 = 0x13;
      }
      for (iVar7 = iVar8 - local_18; iVar7 != 0; iVar7 = iVar7 + iVar4) {
        iVar4 = iVar7;
        if (iVar7 < 0) {
          iVar4 = -iVar7;
        }
        if (0x1344 < iVar4) {
          iVar4 = 0x1344;
        }
        FUN_00421368();
        if (iVar7 < 0) {
          _local_10 = _local_10 / in_ST0;
        }
        else {
          _local_10 = _local_10 * in_ST0;
          iVar4 = -iVar4;
        }
        in_ST0 = in_ST1;
        in_ST1 = in_ST2;
        in_ST2 = in_ST3;
        in_ST3 = in_ST4;
        in_ST4 = in_ST5;
        in_ST5 = in_ST6;
      }
      FUN_00421368();
      if (_local_10 <= in_ST0) {
        FUN_00421368();
        iVar7 = iVar8;
        if (_local_10 < in_ST1) {
          local_18 = local_18 + -1;
          iVar7 = iVar8 + -1;
          if (0 < param_2) {
            _local_10 = _local_10 * (longdouble)(int)local_6;
            iVar7 = iVar8;
          }
        }
      }
      else {
        local_18 = local_18 + 1;
        iVar7 = iVar8 + 1;
        if ((iVar7 < 0x14) && (0 < param_2)) {
          _local_10 = _local_10 / (longdouble)(int)local_6;
          iVar7 = iVar8;
        }
      }
      if (-1 < iVar7) {
        FUN_004215bc((longlong *)local_10,(longdouble *)local_10);
        bVar6 = 0;
        local_28 = param_4 + iVar7;
        *local_28 = 0;
        psVar5 = local_28 + -1;
        if (iVar7 == 0) {
          if (local_10[0] != '\x01') goto LAB_00422c21;
          bVar6 = 0;
        }
        else {
          do {
            uVar3 = FUN_00421850((undefined4 *)local_10);
            bVar6 = bVar6 | (byte)uVar3;
            *psVar5 = (char)(byte)uVar3 + 0x30;
            psVar5 = psVar5 + -1;
            iVar7 = iVar7 + -1;
          } while (iVar7 != 0);
        }
        if (bVar6 == 0) {
          local_18 = local_18 + 1;
          if (param_2 < 1) {
            *local_28 = 0x30;
          }
          local_28 = local_28 + 1;
          psVar5[1] = 0x31;
        }
        if (param_2 < 1) {
          param_2 = local_18 - param_2;
        }
        if (0x28 < param_2) {
          param_2 = 0x28;
        }
        *local_28 = 0;
        iVar8 = (int)local_28 - (int)param_4 >> 1;
        if (iVar8 < 0) {
          iVar8 = iVar8 + (uint)(((int)local_28 - (int)param_4 & 1U) != 0);
        }
        iVar8 = param_2 - iVar8;
        if (iVar8 < 1) {
          return local_18;
        }
        FUN_0041e90c(local_28,0x30,iVar8);
        local_28[iVar8] = 0;
        return local_18;
      }
    }
  }
LAB_00422c21:
  if (param_2 < 1) {
    param_2 = 1 - param_2;
  }
  if (0x28 < param_2) {
    param_2 = 0x28;
  }
  FUN_0041e90c(param_4,0x30,param_2);
  param_4[param_2] = 0;
  return 1;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 __cdecl FUN_00422e8c(UINT param_1)

{
  BOOL BVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  BYTE *pBVar5;
  undefined1 *puVar6;
  byte *pbVar7;
  _cpinfo local_18;
  
  if (param_1 == 0xfffffffe) {
    param_1 = GetOEMCP();
  }
  else if (param_1 == 0xfffffffd) {
    param_1 = GetACP();
  }
  else if (param_1 == 0xfffffffc) {
    param_1 = *(UINT *)PTR_DAT_0042ed78;
  }
  if ((param_1 == 0) || (BVar1 = GetCPInfo(param_1,&local_18), BVar1 != 0)) {
    iVar3 = 0;
    puVar6 = &DAT_00431488;
    do {
      *puVar6 = 0;
      iVar3 = iVar3 + 1;
      puVar6 = puVar6 + 1;
    } while (iVar3 < 0x101);
    if ((param_1 == 0) || (local_18.MaxCharSize < 2)) {
      _DAT_0043158c = 0;
    }
    else {
      for (pBVar5 = local_18.LeadByte; (uVar4 = (uint)*pBVar5, *pBVar5 != 0 && (pBVar5[1] != 0));
          pBVar5 = pBVar5 + 2) {
        puVar6 = &DAT_00431489 + uVar4;
        for (; (int)uVar4 <= (int)(uint)pBVar5[1]; uVar4 = uVar4 + 1) {
          *puVar6 = 4;
          puVar6 = puVar6 + 1;
        }
      }
      iVar3 = 1;
      pbVar7 = &DAT_0043148a;
      do {
        *pbVar7 = *pbVar7 | 8;
        iVar3 = iVar3 + 1;
        pbVar7 = pbVar7 + 1;
      } while (iVar3 < 0xff);
      _DAT_0043158c = param_1;
      if (param_1 == 0x3a4) {
        uVar4 = (uint)DAT_0042f140;
        pbVar7 = &DAT_00431489 + uVar4;
        for (; (int)uVar4 <= (int)(uint)DAT_0042f141; uVar4 = uVar4 + 1) {
          *pbVar7 = *pbVar7 | 1;
          pbVar7 = pbVar7 + 1;
        }
        uVar4 = (uint)DAT_0042f142;
        pbVar7 = &DAT_00431489 + uVar4;
        for (; (int)uVar4 <= (int)(uint)DAT_0042f143; uVar4 = uVar4 + 1) {
          *pbVar7 = *pbVar7 | 2;
          pbVar7 = pbVar7 + 1;
        }
      }
    }
    uVar2 = 0;
  }
  else {
    FUN_004231f4((uint *)s_Error__system_code_page_access_f_0042f144);
    uVar2 = 0xffffffff;
  }
  return uVar2;
}



undefined4 __cdecl FUN_00422fc8(uint param_1)

{
  undefined4 uVar1;
  
  if (0xff < param_1) {
    return 0;
  }
  uVar1 = FUN_00420ac0(param_1);
  return uVar1;
}



byte * __cdecl FUN_00422fe4(byte *param_1,uint param_2)

{
  byte bVar1;
  byte *pbVar2;
  
  pbVar2 = (byte *)0x0;
  do {
    if (((&DAT_00431489)[*param_1] & 4) == 0) {
      if (param_2 == *param_1) {
        pbVar2 = param_1;
      }
    }
    else {
      if (param_1[1] == 0) {
        if (param_2 != 0) {
          return pbVar2;
        }
        return param_1 + 1;
      }
      if (param_2 == CONCAT11(*param_1,param_1[1])) {
        pbVar2 = param_1;
      }
      param_1 = param_1 + 1;
    }
    bVar1 = *param_1;
    param_1 = param_1 + 1;
  } while (bVar1 != 0);
  return pbVar2;
}



void __cdecl FUN_0042303c(uint *param_1,int param_2)

{
  char *pcVar1;
  char *pcVar2;
  uint uVar3;
  undefined4 *local_8;
  
  *(undefined1 *)param_1 = 0;
  local_8 = (undefined4 *)&stack0x0000000c;
  while( true ) {
    pcVar1 = (char *)*local_8;
    if (pcVar1 == (char *)0x0) {
      return;
    }
    pcVar2 = FUN_0041e924(param_1);
    uVar3 = (param_2 - (int)pcVar2) - 1;
    if ((int)uVar3 < 1) break;
    FUN_0041e980((char *)param_1,pcVar1,uVar3);
    local_8 = local_8 + 1;
  }
  return;
}



void __cdecl FUN_00423084(undefined4 param_1,undefined4 param_2,uint param_3)

{
  char *pcVar1;
  
  FUN_0042303c((uint *)&DAT_00431590,0xf6);
  pcVar1 = FUN_0041e924((uint *)&DAT_00431590);
  FUN_00421834(param_3,pcVar1 + 0x431590);
  FUN_004231f4((uint *)&DAT_00431590);
  FUN_004233d8();
  return;
}



undefined * __stdcall FUN_004230e4(void)

{
  _SYSTEMTIME local_10;
  
  GetLocalTime(&local_10);
  wsprintfA((LPSTR)&param_1_00431690,s__02d__02d__04d__02d__02d__02d__0_0042f1ac,
            (uint)local_10.wMonth,(uint)local_10.wDay,(uint)local_10.wYear,(uint)local_10.wHour,
            (uint)local_10.wMinute,(uint)local_10.wSecond,(uint)local_10.wMilliseconds);
  return (undefined *)&param_1_00431690;
}



void __cdecl FUN_00423134(LPCSTR param_1,uint *param_2)

{
  HANDLE hFile;
  uint *lpBuffer;
  char *pcVar1;
  DWORD *pDVar2;
  LPOVERLAPPED p_Var3;
  DWORD local_8;
  
  hFile = CreateFileA(param_1,0xc0000000,0,(LPSECURITY_ATTRIBUTES)0x0,2,0x80,(HANDLE)0x0);
  if (hFile != (HANDLE)0x0) {
    lpBuffer = (uint *)FUN_004230e4();
    pDVar2 = &local_8;
    p_Var3 = (LPOVERLAPPED)0x0;
    pcVar1 = FUN_0041e924(lpBuffer);
    WriteFile(hFile,lpBuffer,(DWORD)pcVar1,pDVar2,p_Var3);
    p_Var3 = (LPOVERLAPPED)0x0;
    pDVar2 = &local_8;
    pcVar1 = FUN_0041e924(param_2);
    WriteFile(hFile,param_2,(DWORD)pcVar1,pDVar2,p_Var3);
    CloseHandle(hFile);
  }
  return;
}



// lpfn parameter of EnumThreadWindows
// 

undefined4 __stdcall lpfn_0042319c(undefined4 param_1,undefined4 *param_2)

{
  *param_2 = 1;
  return 0;
}



undefined4 __stdcall FUN_004231b0(void)

{
  DWORD DVar1;
  undefined4 uVar2;
  code *lpfn;
  int *lParam;
  int local_4;
  
  lParam = &local_4;
  local_4 = 0;
  DVar1 = GetVersion();
  if ((DVar1 >> 0x10 & 0x8000) == 0) {
    return 0x2000;
  }
  lpfn = lpfn_0042319c;
  DVar1 = GetCurrentThreadId();
  EnumThreadWindows(DVar1,lpfn,(LPARAM)lParam);
  uVar2 = 0x2000;
  if (local_4 == 0) {
    uVar2 = 0x1000;
  }
  return uVar2;
}



void __cdecl FUN_004231f4(uint *param_1)

{
  byte *pbVar1;
  uint uVar2;
  HANDLE hFile;
  char *nNumberOfBytesToWrite;
  DWORD *lpNumberOfBytesWritten;
  LPOVERLAPPED lpOverlapped;
  byte local_88 [128];
  DWORD local_8;
  
  if ((DAT_0042f1d8 == (LPCSTR)0x0) && (DAT_0042f1d4 == (code *)0x0)) {
    if (DAT_0042f2f4 != 0) {
      GetModuleFileNameA((HMODULE)0x0,(LPSTR)local_88,0x80);
      pbVar1 = FUN_00422fe4(local_88,0x5c);
      if ((pbVar1 == (byte *)0x0) && (pbVar1 = FUN_00422fe4(local_88,0x3a), pbVar1 == (byte *)0x0))
      {
        pbVar1 = local_88;
      }
      else {
        pbVar1 = pbVar1 + 1;
      }
      uVar2 = FUN_004231b0();
      MessageBoxA((HWND)0x0,(LPCSTR)param_1,(LPCSTR)pbVar1,uVar2 | 0x10010);
      return;
    }
    hFile = GetStdHandle(0xfffffff4);
    WriteFile(hFile,&lpBuffer_0042f1dc,2,&local_8,(LPOVERLAPPED)0x0);
    lpOverlapped = (LPOVERLAPPED)0x0;
    lpNumberOfBytesWritten = &local_8;
    nNumberOfBytesToWrite = FUN_0041e924(param_1);
    WriteFile(hFile,param_1,(DWORD)nNumberOfBytesToWrite,lpNumberOfBytesWritten,lpOverlapped);
    WriteFile(hFile,(LPCVOID)((int)&lpBuffer_0042f1dc + 3),2,&local_8,(LPOVERLAPPED)0x0);
    return;
  }
  if (DAT_0042f1d4 != (code *)0x0) {
    if (DAT_0042f1d4 == (code *)0xffffffff) {
      return;
    }
    (*DAT_0042f1d4)();
  }
  if (DAT_0042f1d8 == (LPCSTR)0x0) {
    return;
  }
  if (DAT_0042f1d8 == (LPCSTR)0xffffffff) {
    return;
  }
  if (*DAT_0042f1d8 == '\0') {
    return;
  }
  FUN_00423134(DAT_0042f1d8,param_1);
  return;
}



void __cdecl FUN_00423310(uint *param_1)

{
  FUN_004231f4(param_1);
  return;
}



void __cdecl FUN_00423320(uint *param_1)

{
  FUN_004231f4(param_1);
  FUN_00423458(1);
  return;
}



void __stdcall FUN_004233c4(void)

{
  FUN_004231f4((uint *)s_Abnormal_program_termination_0042f20c);
  FUN_00423458(3);
  return;
}



void __stdcall FUN_004233d8(void)

{
  FUN_00423b4c(0x16);
  FUN_004233c4();
  return;
}



void __stdcall FUN_004233e8(void)

{
  return;
}



void __cdecl FUN_004233ec(int param_1,int param_2,UINT param_3)

{
  if (param_1 == 0) {
    if (DAT_0042f22c != (code *)0x0) {
      (*DAT_0042f22c)();
    }
    FUN_00423ca8();
    (*(code *)PTR_FUN_0042f230)();
  }
  else {
    DAT_0042f22c = (code *)0x0;
  }
  if (param_2 == 0) {
    if (param_1 == 0) {
      (*(code *)PTR_FUN_0042f234)();
      (*(code *)PTR_FUN_0042f238)();
    }
    FUN_00423bbc(param_3);
  }
  return;
}



void __cdecl FUN_00423440(UINT param_1)

{
  FUN_004233ec(0,0,param_1);
  return;
}



void __cdecl FUN_00423458(UINT param_1)

{
  FUN_004233ec(1,0,param_1);
  return;
}



void __cdecl FUN_00423490(undefined4 param_1,undefined *param_2)

{
  (*(code *)param_2)();
  return;
}



void __cdecl FUN_004234a4(undefined4 param_1,undefined *param_2)

{
  (*(code *)param_2)();
  return;
}



undefined4 __cdecl FUN_004234b8(undefined4 param_1,uint *param_2,undefined *param_3)

{
  char cVar1;
  bool bVar2;
  uint *puVar3;
  undefined1 *local_c;
  uint *local_8;
  
  local_8 = param_2;
  (*(code *)param_3)();
  for (; ((char)*local_8 == ' ' || ((char)*local_8 == '\t')); local_8 = (uint *)((int)local_8 + 1))
  {
  }
  if ((char)*local_8 == '\"') {
    do {
      puVar3 = local_8;
      local_8 = (uint *)((int)puVar3 + 1);
      if (*(char *)local_8 == '\"') break;
    } while (*(char *)local_8 != '\0');
    if (*(char *)local_8 == '\"') {
      local_8 = (uint *)((int)puVar3 + 2);
    }
  }
  else {
    for (; ((cVar1 = (char)*local_8, cVar1 != ' ' && (cVar1 != '\t')) && (cVar1 != '\0'));
        local_8 = (uint *)((int)local_8 + 1)) {
    }
  }
  FUN_0041e924(local_8);
  DAT_004316b0 = (undefined1 *)FUN_0041da4c();
  local_c = DAT_004316b0;
  if (DAT_004316b0 == (undefined1 *)0x0) {
    FUN_00423320((uint *)s_No_space_for_copy_of_command_lin_0042f244);
  }
  while( true ) {
    if ((char)*local_8 == '\0') {
      return 1;
    }
    bVar2 = false;
    for (; ((char)*local_8 == ' ' || ((char)*local_8 == '\t')); local_8 = (uint *)((int)local_8 + 1)
        ) {
    }
    if ((char)*local_8 == '\0') break;
    while (((cVar1 = (char)*local_8, cVar1 != ' ' && (cVar1 != '\t')) && (cVar1 != '\0'))) {
      if (cVar1 == '\"') {
        local_8 = (uint *)((int)local_8 + 1);
        while (((char)*local_8 != '\"' && ((char)*local_8 != '\0'))) {
          FUN_0042362c((int *)&local_c,(int *)&local_8);
        }
        if ((char)*local_8 == '\"') {
          local_8 = (uint *)((int)local_8 + 1);
        }
      }
      else {
        if (((char)*local_8 == '*') || ((char)*local_8 == '?')) {
          bVar2 = true;
        }
        FUN_0042362c((int *)&local_c,(int *)&local_8);
      }
    }
    *local_c = 0;
    local_c = local_c + 1;
    if (bVar2) {
      (*(code *)PTR_FUN_0042f28c)();
    }
    else {
      (*(code *)param_3)();
    }
    if ((char)*local_8 == '\0') {
      return 1;
    }
    local_8 = (uint *)((int)local_8 + 1);
  }
  return 1;
}



void __cdecl FUN_0042362c(int *param_1,int *param_2)

{
  byte bVar1;
  
  if ((*(char *)*param_2 == '\\') && (((char *)*param_2)[1] == '\"')) {
    *(undefined1 *)*param_1 = 0x22;
    *param_2 = *param_2 + 2;
  }
  else if ((DAT_0042f240 == 0) || ((*(char *)*param_2 != '\\' || (((char *)*param_2)[1] != '\\'))))
  {
    bVar1 = *(byte *)*param_2;
    if ((((&DAT_00431489)[bVar1] & 4) != 0) && (((byte *)*param_2)[1] != 0)) {
      *(byte *)*param_1 = bVar1;
      *param_2 = *param_2 + 1;
      *param_1 = *param_1 + 1;
    }
    *(undefined1 *)*param_1 = *(undefined1 *)*param_2;
    *param_2 = *param_2 + 1;
  }
  else {
    *(undefined1 *)*param_1 = 0x5c;
    *param_2 = *param_2 + 2;
  }
  *param_1 = *param_1 + 1;
  return;
}



undefined4 __cdecl FUN_004236a0(undefined4 param_1,short *param_2,undefined *param_3)

{
  short sVar1;
  bool bVar2;
  short *psVar3;
  undefined2 *local_c;
  short *local_8;
  
  local_8 = param_2;
  (*(code *)param_3)();
  for (; (*local_8 == 0x20 || (*local_8 == 9)); local_8 = local_8 + 1) {
  }
  if (*local_8 == 0x22) {
    do {
      psVar3 = local_8;
      local_8 = psVar3 + 1;
      if (*local_8 == 0x22) break;
    } while (*local_8 != 0);
    if (*local_8 == 0x22) {
      local_8 = psVar3 + 2;
    }
  }
  else {
    for (; ((sVar1 = *local_8, sVar1 != 0x20 && (sVar1 != 9)) && (sVar1 != 0));
        local_8 = local_8 + 1) {
    }
  }
  FUN_0041ea5c(local_8);
  DAT_004316b4 = (undefined2 *)FUN_0041da4c();
  local_c = DAT_004316b4;
  if (DAT_004316b4 == (undefined2 *)0x0) {
    FUN_00423320((uint *)s_No_space_for_copy_of_command_lin_0042f268);
  }
  while( true ) {
    if (*local_8 == 0) {
      return 1;
    }
    bVar2 = false;
    for (; (*local_8 == 0x20 || (*local_8 == 9)); local_8 = local_8 + 1) {
    }
    if (*local_8 == 0) break;
    while (((sVar1 = *local_8, sVar1 != 0x20 && (sVar1 != 9)) && (sVar1 != 0))) {
      if (sVar1 == 0x22) {
        local_8 = local_8 + 1;
        while ((*local_8 != 0x22 && (*local_8 != 0))) {
          FUN_00423850((int *)&local_c,(int *)&local_8);
        }
        if (*local_8 == 0x22) {
          local_8 = local_8 + 1;
        }
      }
      else {
        if ((*local_8 == 0x2a) || (*local_8 == 0x3f)) {
          bVar2 = true;
        }
        FUN_00423850((int *)&local_c,(int *)&local_8);
      }
    }
    *local_c = 0;
    local_c = local_c + 1;
    if (bVar2) {
      (*(code *)PTR_FUN_0042f290)();
    }
    else {
      (*(code *)param_3)();
    }
    if (*local_8 == 0) {
      return 1;
    }
    local_8 = local_8 + 1;
  }
  return 1;
}



void __cdecl FUN_00423850(int *param_1,int *param_2)

{
  if ((*(short *)*param_2 == 0x5c) && (((short *)*param_2)[1] == 0x22)) {
    *(undefined2 *)*param_1 = 0x22;
    *param_2 = *param_2 + 4;
  }
  else if ((DAT_0042f240 == 0) || ((*(short *)*param_2 != 0x5c || (((short *)*param_2)[1] != 0x5c)))
          ) {
    *(undefined2 *)*param_1 = *(undefined2 *)*param_2;
    *param_2 = *param_2 + 2;
  }
  else {
    *(undefined2 *)*param_1 = 0x5c;
    *param_2 = *param_2 + 4;
  }
  *param_1 = *param_1 + 2;
  return;
}



int __cdecl FUN_004238c8(int param_1)

{
  int iVar1;
  int *piVar2;
  
  iVar1 = 0;
  piVar2 = &DAT_0042f2c0;
  do {
    if (param_1 == *piVar2) {
      return iVar1;
    }
    iVar1 = iVar1 + 1;
    piVar2 = piVar2 + 1;
  } while (iVar1 < 10);
  return -1;
}



undefined4 __cdecl FUN_004238e8(int *param_1,undefined4 param_2,int param_3)

{
  code *pcVar1;
  int iVar2;
  uint uVar3;
  undefined4 local_c;
  
  uVar3 = 0;
  local_c = 0;
  iVar2 = *param_1;
  if (iVar2 < -0x3fffff6f) {
    if (iVar2 == -0x3fffff70) {
      iVar2 = 2;
      uVar3 = 1;
    }
    else if (iVar2 < -0x3ffffffa) {
      if (iVar2 == -0x3ffffffb) {
        iVar2 = 3;
      }
      else if (iVar2 == -0x7ffffffd) {
        iVar2 = 1;
      }
      else {
        if (iVar2 != -0x7ffffffc) {
          return 1;
        }
        iVar2 = 1;
      }
    }
    else if (iVar2 == -0x3fffffe3) {
      iVar2 = 1;
    }
    else {
      if (iVar2 != -0x3fffff72) {
        return 1;
      }
      iVar2 = 2;
      uVar3 = 4;
    }
  }
  else {
    switch(iVar2) {
    case -0x3fffff6f:
      iVar2 = 2;
      uVar3 = 8;
      break;
    case -0x3fffff6e:
      iVar2 = 2;
      uVar3 = 0x49;
      break;
    case -0x3fffff6d:
      iVar2 = 2;
      uVar3 = 0x10;
      break;
    case -0x3fffff6c:
      iVar2 = 2;
      break;
    default:
      return 1;
    case -0x3fffff6a:
      iVar2 = 1;
    }
  }
  pcVar1 = *(code **)(&DAT_0042f298 + iVar2 * 4);
  if (pcVar1 == (code *)0x1) {
    local_c = 0;
  }
  else if (pcVar1 == (code *)0x0) {
    local_c = 1;
  }
  else {
    *(undefined4 *)(&DAT_0042f298 + iVar2 * 4) = 0;
    if (((iVar2 == 1) || (iVar2 == 3)) || (iVar2 == 2)) {
      FUN_004214e8();
      FUN_004214fc(DAT_0042f02c);
      (*pcVar1)();
      *(uint *)(param_3 + 0x20) = *(uint *)(param_3 + 0x20) & ~uVar3;
    }
    else {
      (*pcVar1)();
    }
  }
  return local_c;
}



// HandlerRoutine parameter of SetConsoleCtrlHandler
// 

undefined4 __stdcall HandlerRoutine_00423ac8(int param_1)

{
  int iVar1;
  
  iVar1 = 2;
  if (param_1 != 0) {
    iVar1 = 0x15;
  }
  FUN_00423b4c(iVar1);
  return 1;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 __cdecl FUN_00423aec(int param_1,undefined4 param_2)

{
  undefined4 uVar1;
  int iVar2;
  
  if (DAT_0042f294 == '\0') {
    DAT_004312ec = FUN_004238e8;
    SetConsoleCtrlHandler(HandlerRoutine_00423ac8,1);
    DAT_0042f294 = '\x01';
  }
  iVar2 = FUN_004238c8(param_1);
  if (iVar2 == -1) {
    _DAT_0042f1d0 = 0x13;
    return 0xffffffff;
  }
  uVar1 = *(undefined4 *)(&DAT_0042f298 + iVar2 * 4);
  *(undefined4 *)(&DAT_0042f298 + iVar2 * 4) = param_2;
  return uVar1;
}



undefined4 __cdecl FUN_00423b4c(int param_1)

{
  code *pcVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  
  iVar4 = param_1;
  iVar2 = FUN_004238c8(param_1);
  if (iVar2 == -1) {
    uVar3 = 1;
  }
  else {
    pcVar1 = *(code **)(&DAT_0042f298 + iVar2 * 4);
    if (pcVar1 != (code *)0x1) {
      if (pcVar1 == (code *)0x0) {
        if ((1 < param_1 - 0x10U) && (param_1 != 0x14)) {
          if (param_1 == 0x16) {
            FUN_004233c4();
          }
          else {
            FUN_00423458(3);
          }
        }
      }
      else {
        *(undefined4 *)(&DAT_0042f298 + iVar2 * 4) = 0;
        (*pcVar1)(pcVar1,(&DAT_0042f2e8)[iVar2],iVar4,param_1,(&DAT_0042f2e8)[iVar2],0);
      }
    }
    uVar3 = 0;
  }
  return uVar3;
}



void __stdcall FUN_00423bbc(UINT param_1)

{
                    // WARNING: Subroutine does not return
  ExitProcess(param_1);
}



undefined4 __stdcall FUN_00423bcc(void)

{
  return 0;
}



undefined4 __stdcall FUN_00423bd0(void)

{
  return 0;
}



void __cdecl FUN_00423bd4(int *param_1,int param_2)

{
  uint *puVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  int *local_18;
  int local_14;
  int local_10;
  int local_c;
  
  if (param_2 == 0) {
    local_10 = 0x100;
    local_14 = 1;
    iVar4 = 0;
  }
  else {
    local_10 = -1;
    local_14 = -1;
    iVar4 = 0xff;
  }
  for (; iVar4 != local_10; iVar4 = iVar4 + local_14) {
    local_18 = param_1;
    for (local_c = 0; local_18 = local_18 + 1, local_c < *param_1; local_c = local_c + 1) {
      puVar1 = (uint *)*local_18;
      if (param_2 == 0) {
        uVar2 = puVar1[1];
        for (uVar5 = *puVar1; uVar5 < uVar2; uVar5 = uVar5 + 6) {
          if ((char)iVar4 == *(char *)(uVar5 + 1)) {
            (**(code **)(uVar5 + 2))();
          }
        }
      }
      else {
        uVar5 = puVar1[2];
        uVar2 = puVar1[3];
        while (uVar3 = uVar2, uVar2 = uVar3 - 6, uVar5 <= uVar2) {
          if ((char)iVar4 == *(char *)(uVar3 - 5)) {
            (**(code **)(uVar3 - 4))();
          }
        }
      }
    }
  }
  return;
}



void __stdcall FUN_00423ca8(void)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  
  if (DAT_0042f2f8 == 0) {
    DAT_0042f2f8 = 1;
    FUN_00423bd4((int *)&DAT_004316f0,1);
    FUN_00423bd4((int *)&DAT_004316d4,1);
    piVar1 = (int *)FUN_00423bd0();
    if (piVar1 != (int *)0x0) {
      FUN_00423bd4(piVar1,1);
      piVar2 = piVar1;
      for (iVar3 = 0; piVar2 = piVar2 + 1, iVar3 < *piVar1; iVar3 = iVar3 + 1) {
        (**(code **)(*piVar2 + 0x18))();
      }
    }
  }
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 __cdecl FUN_00423d14(int param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  int iVar1;
  int *piVar2;
  undefined4 uVar3;
  int local_c;
  int local_8;
  
  iVar1 = DAT_004316fc;
  local_c = 1;
  local_8 = param_1;
  if ((param_4 == 1) && (DAT_004316fc = DAT_004316fc + 1, iVar1 == 0)) {
    DAT_004316cc = GetEnvironmentStrings();
    _DAT_0042819b = param_3;
    FUN_0041fa50(*(undefined4 **)(param_1 + 0x44));
    if ((DAT_0042f23c == 0) ||
       ((piVar2 = (int *)FUN_00423bcc(), piVar2 == (int *)0x0 || (piVar2[*piVar2 + 1] == -1)))) {
      DAT_0042f23c = 0;
      FUN_00423bd4(&local_c,0);
    }
    else {
      iVar1 = *piVar2;
      *piVar2 = *piVar2 + 1;
      piVar2[iVar1 + 1] = param_1;
    }
  }
  uVar3 = FUN_00423ea8(*(undefined **)(param_1 + 0x18),param_3);
  if (param_4 == 0) {
    DAT_004316fc = DAT_004316fc + -1;
    if ((DAT_004316fc == 0) && (DAT_004316cc != (LPCH)0x0)) {
      FreeEnvironmentStringsA(DAT_004316cc);
      DAT_004316cc = (LPCH)0x0;
    }
    if ((DAT_004316fc == 0) && (DAT_0042f23c == 0)) {
      if (DAT_0042f22c != (code *)0x0) {
        (*DAT_0042f22c)();
      }
      FUN_00423bd4(&local_c,1);
    }
  }
  return uVar3;
}



undefined4 __stdcall FUN_00423e14(undefined4 *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  CHAR *pCVar4;
  CHAR local_304 [256];
  uint local_204 [128];
  
  uVar1 = *param_1;
  puVar3 = &DAT_0042f300;
  pCVar4 = local_304;
  for (iVar2 = 0x40; iVar2 != 0; iVar2 = iVar2 + -1) {
    *(undefined4 *)pCVar4 = *puVar3;
    puVar3 = puVar3 + 1;
    pCVar4 = pCVar4 + 4;
  }
  if (DAT_0042f2fc != 0) {
    DAT_0042f2fc = 0;
    GetModuleFileNameA(DAT_00431700,local_304,0x100);
    wsprintfA((LPSTR)local_204,s_An_exception___08X__occurred_dur_0042f401,uVar1,local_304);
    FUN_004231f4(local_204);
  }
  return 1;
}



void __cdecl FUN_00423e98(undefined4 *param_1)

{
  undefined4 *puVar1;
  undefined2 in_FS;
  
  puVar1 = (undefined4 *)segment(in_FS,0);
  *puVar1 = *param_1;
  return;
}



undefined4 __stdcall FUN_00423ea8(undefined *param_1,undefined4 param_2)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  undefined2 in_FS;
  int *in_FS_OFFSET;
  undefined4 local_c;
  code *local_8;
  
  DAT_00431700 = param_2;
  local_8 = FUN_00423e14;
  puVar1 = (undefined4 *)segment(in_FS,0);
  local_c = *puVar1;
  *in_FS_OFFSET = (int)&local_c;
  DAT_0042f2fc = 1;
  uVar2 = (*(code *)param_1)();
  DAT_0042f2fc = 1;
  FUN_00423e98(&local_c);
  return uVar2;
}



void __stdcall FUN_00423f0c(void)

{
  return;
}



void __stdcall FUN_00423f10(void)

{
  return;
}



void __stdcall FUN_00423f14(void)

{
  TlsAlloc();
  return;
}



void __stdcall FUN_00423f1c(DWORD param_1)

{
  TlsFree(param_1);
  return;
}



void __stdcall FUN_00423f2c(DWORD param_1)

{
  TlsGetValue(param_1);
  return;
}



void __stdcall FUN_00423f3c(DWORD param_1,LPVOID param_2)

{
  TlsSetValue(param_1,param_2);
  return;
}



void __stdcall FUN_00423f50(void)

{
  return;
}



void __stdcall FUN_00423f58(void)

{
  return;
}



void FUN_00423f8f(undefined *UNRECOVERED_JUMPTABLE)

{
                    // WARNING: Could not recover jumptable at 0x00423f93. Too many branches
                    // WARNING: Treating indirect jump as call
  (*(code *)UNRECOVERED_JUMPTABLE)();
  return;
}



int __cdecl FUN_00423f96(uint param_1)

{
  int iVar1;
  
  iVar1 = FUN_0041da4c();
  if (iVar1 != 0) {
    return iVar1;
  }
  if (param_1 < 0x81) {
    iVar1 = FUN_00401202();
    if ((*(byte *)(iVar1 + 0x14) & 1) == 0) goto LAB_00423fc3;
  }
  FUN_004233d8();
LAB_00423fc3:
  iVar1 = FUN_00401202();
  *(uint *)(iVar1 + 0x14) = *(uint *)(iVar1 + 0x14) | 1;
  iVar1 = FUN_00401202();
  return *(int *)(iVar1 + 0x24);
}



void __cdecl FUN_00423fe0(int param_1)

{
  int iVar1;
  
  iVar1 = FUN_00401202();
  if (param_1 == *(int *)(iVar1 + 0x24)) {
    iVar1 = FUN_00401202();
    *(uint *)(iVar1 + 0x14) = *(uint *)(iVar1 + 0x14) & 0xfffffffe;
    return;
  }
  FUN_0041da5c();
  return;
}



undefined4 __cdecl FUN_0042400d(int *param_1,undefined4 param_2,int *param_3,byte param_4)

{
  ushort uVar1;
  ushort uVar2;
  int iVar3;
  
  iVar3 = FUN_0041f0ec(param_1,param_3,0,param_2);
  if (iVar3 != 0) {
    return 1;
  }
  uVar1 = *(ushort *)(param_1 + 1);
  uVar2 = *(ushort *)(param_3 + 1);
  if ((uVar2 & 0x20) != 0) {
    iVar3 = FUN_0041f0ec(param_1,(int *)param_3[2],1,param_2);
    if (iVar3 != 0) {
      return 1;
    }
    uVar2 = *(ushort *)((int *)param_3[2] + 1);
    if ((uVar2 & 0x10) == 0) {
      return 0;
    }
    iVar3 = FUN_0041f0ec(param_1,(int *)param_3[2],0,param_2);
    if (iVar3 != 0) {
      return 1;
    }
  }
  if ((uVar2 & 0x10) != 0) {
    if ((param_4 & 1) != 0) {
      return 1;
    }
    if ((uVar1 & 0x10) == 0) {
      return 0;
    }
    if ((((uVar2 & 0x40) != 0) && (((uVar2 & 0x100) != 0 || ((uVar1 & 0x100) == 0)))) &&
       (((uVar2 & 0x200) != 0 || ((uVar1 & 0x200) == 0)))) {
      return 1;
    }
  }
  return 0;
}



void __cdecl FUN_004240d3(undefined4 param_1,undefined4 param_2,undefined *param_3,uint param_4)

{
  uint uVar1;
  
  if (((param_4 & 0x100) == 0) && ((param_4 & 0x20) != 0)) {
    FUN_00423084(s__ctorMask___0x0100_____0_____cto_0042f528,s_xx_cpp_0042f55d,0x205);
  }
  if ((param_4 & 0x80) != 0) {
    FUN_00423084(s__ctorMask___0x0080_____0_0042f564,s_xx_cpp_0042f57d,0x207);
  }
  if ((param_4 & 0x1000) == 0) {
    uVar1 = param_4 & 7;
    if (uVar1 == 1) {
      (*(code *)param_3)();
    }
    else if (uVar1 == 2) {
      (*(code *)param_3)();
    }
    else if (uVar1 == 3) {
      (*(code *)param_3)(param_1,param_2);
    }
    else if (uVar1 == 5) {
      (*(code *)param_3)();
    }
    else {
      FUN_00423084(s___what___0042f5a0,s_xx_cpp_0042f5a9,0x25e);
    }
  }
  else {
    uVar1 = param_4 & 7;
    if (uVar1 == 1) {
      (*(code *)param_3)();
    }
    else if (uVar1 == 2) {
      (*(code *)param_3)();
    }
    else if (uVar1 == 3) {
      (*(code *)param_3)(param_1,0,param_2);
    }
    else if (uVar1 == 5) {
      (*(code *)param_3)();
    }
    else {
      FUN_00423084(s___what___0042f58a,s_xx_cpp_0042f593,0x232);
    }
  }
  return;
}



int __cdecl FUN_004241e1(char *param_1)

{
  char cVar1;
  int iVar2;
  
  iVar2 = 0;
  if (param_1 != (char *)0x0) {
    while (cVar1 = *param_1, param_1 = param_1 + 1, cVar1 != '\0') {
      iVar2 = iVar2 + 1;
    }
  }
  return iVar2;
}



void __cdecl FUN_004241f9(undefined4 param_1,int param_2)

{
  RaiseException(0xeedfae6,0,param_2 + 2,&param_1);
  return;
}



void __cdecl
FUN_00424216(undefined4 param_1,int param_2,byte param_3,undefined *param_4,uint param_5,int param_6
            )

{
  char *pcVar1;
  uint uVar2;
  uint uVar3;
  
  uVar2 = 2;
  if (param_6 == 0) {
    uVar2 = 0;
  }
  if ((param_3 & 2) != 0) {
    uVar2 = uVar2 | 0x80;
  }
  if (*DAT_00431708 == 2) {
    pcVar1 = FUN_0041efd8(param_2);
    FUN_004241e1(pcVar1);
    FUN_004241f9(0,3);
  }
  if ((param_5 & 0x80) != 0) {
    FUN_00423084(s__dtorMask___0x0080_____0_0042f5b0,s_xx_cpp_0042f5c9,0x2cd);
  }
  uVar3 = param_5 & 7;
  if (uVar3 == 1) {
    (*(code *)param_4)();
  }
  else if (uVar3 == 2) {
    (*(code *)param_4)();
  }
  else if (uVar3 == 3) {
    (*(code *)param_4)(param_1,uVar2);
  }
  else if (uVar3 == 5) {
    (*(code *)param_4)();
  }
  else {
    FUN_00423084(s___what___0042f5d6,s_xx_cpp_0042f5df,0x2f4);
  }
  return;
}



void __cdecl FUN_004242e5(undefined4 param_1,undefined *param_2,uint param_3)

{
  uint uVar1;
  
  if ((param_3 & 0x80) != 0) {
    FUN_00423084(s__mfnMask___0x0080_____0_0042f5e6,s_xx_cpp_0042f5fe,0x2fc);
  }
  uVar1 = param_3 & 7;
  if (uVar1 == 1) {
    (*(code *)param_2)();
    return;
  }
  if (uVar1 == 2) {
    (*(code *)param_2)();
    return;
  }
  if (uVar1 == 3) {
    (*(code *)param_2)(param_1);
    return;
  }
  if (uVar1 == 5) {
    (*(code *)param_2)();
    return;
  }
  FUN_00423084(s___what___0042f60b,s_xx_cpp_0042f614,0x323);
  return;
}



void __cdecl
FUN_00424363(uint *param_1,undefined4 *param_2,undefined4 param_3,undefined *param_4,uint param_5,
            undefined4 param_6,undefined4 param_7,undefined4 param_8,undefined4 param_9)

{
  undefined4 *puVar1;
  ushort uVar2;
  uint uVar3;
  int iVar4;
  char *pcVar5;
  int iVar6;
  undefined2 in_FS;
  ULONG_PTR local_44;
  undefined4 local_40;
  undefined4 *local_3c;
  undefined4 local_38;
  undefined4 *local_34;
  uint local_30;
  uint *local_2c;
  undefined4 local_28;
  undefined2 local_18;
  
  BCB_InitExceptionHandler(0x42f470);
  iVar4 = FUN_00401202();
  *(uint *)(iVar4 + 0x14) = *(uint *)(iVar4 + 0x14) | 2;
  iVar4 = FUN_00401202();
  if (param_1 == (uint *)0x0) {
    iVar6 = 0;
  }
  else {
    iVar6 = (uint)*(ushort *)((int)param_1 + 6) + (int)param_1;
  }
  *(int *)(iVar4 + 0x20) = iVar6;
  iVar4 = FUN_00401202();
  *(undefined4 *)(iVar4 + 0x1c) = param_7;
  iVar4 = FUN_00401202();
  *(undefined4 *)(iVar4 + 0x18) = param_8;
  local_2c = param_1;
  uVar2 = (ushort)param_1[1];
  uVar3 = *param_1;
  if ((uVar2 & 2) == 0) {
    local_30 = 0;
  }
  else {
    local_30 = param_1[3];
  }
  if ((uVar2 & 0x30) != 0) {
    local_2c = (uint *)param_1[2];
  }
  local_34 = (undefined4 *)FUN_00423f96(uVar3 + 0x52);
  *local_34 = 0;
  local_34[1] = param_1;
  local_34[3] = param_6;
  local_34[4] = uVar3;
  *(ushort *)(local_34 + 6) = uVar2;
  *(undefined2 *)((int)local_34 + 0x1a) = (undefined2)local_30;
  local_34[5] = local_2c;
  local_34[2] = param_3;
  local_34[10] = 0;
  local_34[0xb] = 0;
  local_34[7] = FUN_00423fe0;
  local_34[0xd] = param_7;
  local_34[0xe] = param_8;
  local_34[8] = param_4;
  local_34[9] = param_5;
  *(undefined1 *)((int)local_34 + 0x45) = 0;
  *(undefined1 *)(local_34 + 0x11) = 1;
  BCB_memcpy((undefined4 *)((int)local_34 + 0x52),param_2,uVar3);
  if ((*DAT_00431708 == 1) || (*DAT_00431708 == 2)) {
    pcVar5 = FUN_0041efd8((int)param_1);
    FUN_004241e1(pcVar5);
    FUN_004241f9(1,0xb);
  }
  if ((local_30 & 1) != 0) {
    if (param_4 == (undefined *)0x0) {
      FUN_00423084(s_cctrAddr_0042f61b,s_xx_cpp_0042f624,0x445);
    }
    local_38 = DAT_00431704;
    local_18 = 8;
    FUN_004240d3((int)local_34 + 0x52,param_2,param_4,param_5);
    local_18 = 0;
    DAT_00431704 = local_38;
  }
  iVar4 = FUN_00401202();
  local_44 = *(ULONG_PTR *)(iVar4 + 0x20);
  local_40 = param_9;
  local_3c = local_34;
  RaiseException(0xeefface,1,3,&local_44);
  puVar1 = (undefined4 *)segment(in_FS,0);
  *puVar1 = local_28;
  return;
}



void __cdecl
FUN_004245b3(uint *param_1,undefined4 *param_2,undefined4 param_3,undefined *param_4,uint param_5,
            undefined4 param_6,undefined4 param_7,undefined4 param_8)

{
  undefined4 in_stack_00000000;
  
  FUN_00424363(param_1,param_2,param_3,param_4,param_5,param_6,param_7,param_8,in_stack_00000000);
  return;
}



void __cdecl FUN_004245e8(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 *puVar2;
  undefined4 in_stack_00000000;
  
  iVar1 = FUN_00401202();
  iVar1 = *(int *)(iVar1 + 0x10);
  if (iVar1 == 0) {
    FUN_0041f344();
  }
  if (*(char *)(iVar1 + 0x44) == '\0') {
    puVar2 = *(undefined4 **)(iVar1 + 0x40);
  }
  else {
    puVar2 = (undefined4 *)(iVar1 + 0x52);
  }
  FUN_00424363(*(uint **)(iVar1 + 4),puVar2,*(undefined4 *)(iVar1 + 8),*(undefined **)(iVar1 + 0x20)
               ,*(uint *)(iVar1 + 0x24),*(undefined4 *)(iVar1 + 0xc),param_1,param_2,
               in_stack_00000000);
  return;
}



void __cdecl FUN_00424653(PVOID param_1,PEXCEPTION_RECORD param_2)

{
  EXCEPTION_RECORD local_54;
  
  if (param_2 == (PEXCEPTION_RECORD)0x0) {
    local_54.ExceptionCode = 0x26;
    local_54.ExceptionFlags = 2;
    param_2 = &local_54;
    local_54.NumberParameters = 0;
  }
  param_2->ExceptionFlags = param_2->ExceptionFlags | 2;
  FUN_0041f63a(param_1,param_2);
  return;
}



void __cdecl FUN_00424692(undefined4 param_1,int param_2,undefined *param_3,uint param_4)

{
  undefined4 *puVar1;
  undefined2 in_FS;
  undefined4 local_28;
  
  BCB_InitExceptionHandler(0x42f4a4);
  if (param_3 == (undefined *)0x0) {
    FUN_00423084(s_dtorAddr_0042f62b,s_xx_cpp_0042f634,0x5b9);
  }
  FUN_00424216(param_1,param_2,0,param_3,param_4,1);
  puVar1 = (undefined4 *)segment(in_FS,0);
  *puVar1 = local_28;
  return;
}



void __cdecl FUN_00424707(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  uVar2 = DAT_00431704;
  if (*(char *)(param_1 + 0x44) != '\0') {
    if ((*(byte *)(param_1 + 0x1a) & 2) != 0) {
      iVar1 = *(int *)(param_1 + 0x14);
      FUN_00424692(param_1 + 0x52,iVar1,*(undefined **)(iVar1 + 0x28),
                   (uint)*(ushort *)(iVar1 + 0x2c));
    }
    DAT_00431704 = uVar2;
    *(undefined1 *)(param_1 + 0x44) = 0;
  }
  if (*(char *)(param_1 + 0x45) != '\0') {
    iVar1 = *(int *)(param_1 + 0x3c);
    if (iVar1 == 0) {
      FUN_00423084(s_argType_0042f63b,s_xx_cpp_0042f643,0x5fe);
    }
    if (((*(byte *)(iVar1 + 4) & 2) != 0) && ((*(byte *)(iVar1 + 0xc) & 2) != 0)) {
      FUN_00424692(*(undefined4 *)(param_1 + 0x40),iVar1,*(undefined **)(iVar1 + 0x28),
                   (uint)*(ushort *)(iVar1 + 0x2c));
    }
    *(undefined1 *)(param_1 + 0x45) = 0;
  }
  return;
}



// WARNING: Unable to track spacebase fully for stack

void __stdcall FUN_0042478f(void)

{
  int *piVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  undefined2 in_FS;
  undefined4 in_stack_00000000;
  
  iVar4 = FUN_00401202();
  if (*(int *)(iVar4 + 0x10) == 0) {
    FUN_00423084(s___CPPexceptionList_0042f64a,s_xx_cpp_0042f65d,0x629);
  }
  iVar4 = FUN_00401202();
  puVar2 = *(undefined4 **)(iVar4 + 0x10);
  iVar4 = FUN_00401202();
  *(undefined4 *)(iVar4 + 0x10) = *puVar2;
  iVar4 = puVar2[10];
  piVar1 = (int *)segment(in_FS,0);
  iVar3 = *piVar1;
  if (iVar3 == 0) {
    FUN_00423084(&DAT_0042f664,s_xx_cpp_0042f667,0x637);
  }
  if (iVar3 != puVar2[10]) {
    FUN_00423084(s_xdrPtr_>xdERRaddr____xl_0042f66e,s_xx_cpp_0042f686,0x638);
  }
  *(undefined2 *)(iVar4 + 0x10) =
       *(undefined2 *)(*(int *)(iVar4 + 8) + (uint)*(ushort *)(iVar4 + 0x10));
  FUN_00424707((int)puVar2);
  (*(code *)puVar2[7])();
  if (*DAT_00431708 == 2) {
    FUN_004241f9(6,1);
  }
  *(undefined4 *)(*(int *)(iVar4 + 0xc) + -4) = in_stack_00000000;
  return;
}



void __cdecl FUN_00424880(int param_1,int param_2,int param_3,int *param_4,int param_5)

{
  ushort uVar1;
  uint uVar2;
  int *piVar3;
  int iVar4;
  ushort uVar5;
  uint *puVar6;
  bool bVar7;
  int *local_8;
  
  if (*(int *)(param_3 + 0x28) != param_2) {
    FUN_00423084(s_dscPtr_>xdERRaddr____errPtr_0042f68d,s_xx_cpp_0042f6a9,0x722);
  }
  if (*(int *)(param_3 + 0x2c) != param_1) {
    FUN_00423084(s_dscPtr_>xdHtabAdr____hdtPtr_0042f6b0,s_xx_cpp_0042f6cc,0x723);
  }
  if (*(char *)(param_3 + 0x45) != '\0') {
    FUN_00423084(s_dscPtr_>xdArgCopy____0_0042f6d3,s_xx_cpp_0042f6ea,0x725);
  }
  puVar6 = *(uint **)(param_1 + 4);
  *(uint **)(param_3 + 0x3c) = puVar6;
  if ((puVar6 != (uint *)0x0) && ((*(byte *)(param_1 + 8) & 0x80) == 0)) {
    bVar7 = (*(byte *)(param_1 + 8) & 1) == 0;
    local_8 = (int *)(param_3 + 0x52);
    *(undefined1 *)(param_3 + 0x45) = 1;
    *(int *)(param_3 + 0x40) = *param_4 + param_5;
    uVar1 = (ushort)puVar6[1];
    uVar2 = *puVar6;
    uVar5 = uVar1;
    if ((uVar1 & 0x30) != 0) {
      puVar6 = (uint *)puVar6[2];
      uVar5 = (ushort)puVar6[1];
    }
    if (((uVar1 & 0x10) == 0) || ((*(byte *)(param_3 + 0xc) & 1) == 0)) {
      if (((uVar5 & 1) == 0) || ((uVar1 & 0x30) == 0)) {
        if ((*(byte *)(param_3 + 0x18) & 1) == 0) {
          if ((uVar1 & 0x20) == 0) {
            if (uVar2 != *(uint *)(param_3 + 0x10)) {
              FUN_00423084(s_dscPtr_>xdSize____size_0042f798,s_xx_cpp_0042f7af,0x7f8);
            }
            BCB_memcpy(*(undefined4 **)(param_3 + 0x40),local_8,uVar2);
          }
          else {
            BCB_memcpy(*(undefined4 **)(param_3 + 0x40),&local_8,uVar2);
            bVar7 = true;
          }
        }
        else {
          if (*(int *)(param_3 + 4) != *(int *)(param_3 + 0x14)) {
            FUN_00423084(s_dscPtr_>xdTypeID____dscPtr_>xdBa_0042f754,s_xx_cpp_0042f777,0x7b6);
          }
          iVar4 = FUN_0041f010(*(int **)(param_3 + 0x14),(int *)puVar6);
          if (iVar4 == 0) {
            local_8 = (int *)FUN_0041f1fc((int)local_8,*(int *)(param_3 + 0x14),(int *)puVar6);
          }
          bVar7 = iVar4 == 0 || bVar7;
          if ((puVar6[3] & 1) == 0) {
            BCB_memcpy(*(undefined4 **)(param_3 + 0x40),local_8,uVar2);
          }
          else {
            if (*(int *)(param_1 + 0xc) == 0) {
              FUN_00423084(s_hdtPtr_>HDcctrAddr_0042f77e,s_xx_cpp_0042f791,2000);
            }
            FUN_004240d3(*(undefined4 *)(param_3 + 0x40),local_8,*(undefined **)(param_1 + 0xc),
                         *(uint *)(param_1 + 0x10));
            bVar7 = true;
          }
        }
      }
      else {
        if ((uVar1 & 0x20) == 0) {
          if ((uVar1 & 0x10) == 0) {
            FUN_00423084(s_mask___TM_IS_PTR_0042f71a,s_xx_cpp_0042f72b,0x795);
          }
          if ((*(byte *)(param_3 + 0x18) & 0x10) == 0) {
            FUN_00423084(s_dscPtr_>xdMask___TM_IS_PTR_0042f732,s_xx_cpp_0042f74d,0x796);
          }
          local_8 = (int *)*local_8;
        }
        else {
          if ((*(byte *)(param_3 + 0x18) & 0x10) != 0) {
            FUN_00423084(s__dscPtr_>xdMask___TM_IS_PTR_____0_0042f6f1,s_xx_cpp_0042f713,0x782);
          }
          bVar7 = true;
        }
        iVar4 = FUN_0041f010(*(int **)(param_3 + 0x14),(int *)puVar6);
        piVar3 = local_8;
        if (iVar4 == 0) {
          local_8 = (int *)FUN_0041f1fc((int)local_8,*(int *)(param_3 + 0x14),(int *)puVar6);
          if (local_8 != piVar3) {
            bVar7 = true;
          }
        }
        BCB_memcpy(*(undefined4 **)(param_3 + 0x40),&local_8,uVar2);
      }
    }
    else {
      FUN_0041e880(*(undefined4 **)(param_3 + 0x40),0,uVar2);
      bVar7 = true;
    }
    if (!bVar7) {
      if ((puVar6[3] & 2) != 0) {
        FUN_00424692(local_8,(int)puVar6,(undefined *)puVar6[10],(uint)(ushort)puVar6[0xb]);
      }
      *(undefined1 *)(param_3 + 0x44) = 0;
    }
  }
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __cdecl FUN_00424b49(int param_1)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  uint uVar5;
  undefined1 *puVar6;
  uint uVar7;
  
  puVar6 = &stack0xfffffffc;
  uVar7 = (uint)*(ushort *)(param_1 + 0x10);
  while ((uVar5 = uVar7, uVar5 != 0 && (uVar5 != *(uint *)(puVar6 + 0xc)))) {
    uVar7 = (uint)*(ushort *)(*(int *)(puVar6 + -4) + uVar5);
    uVar4 = (uint)*(ushort *)(*(int *)(puVar6 + -4) + uVar5 + 2);
    *(ushort *)(*(int *)(puVar6 + 8) + 0x10) = *(ushort *)(*(int *)(puVar6 + -4) + uVar5);
    if (uVar4 == 0) {
      _DAT_0043170c = *(undefined4 *)(*(int *)(puVar6 + -4) + 4 + uVar5 + 4);
      *(undefined2 *)(*(int *)(puVar6 + 8) + 0x12) = 1;
      FUN_0041f64f();
      *(undefined2 *)(*(int *)(puVar6 + 8) + 0x12) = 0;
    }
    else if (2 < uVar4 - 1) {
      if (uVar4 == 4) {
        iVar2 = FUN_00401202();
        *(int *)(puVar6 + -0x10) = iVar2 + 0x10;
        while ((puVar1 = (undefined4 *)**(int **)(puVar6 + -0x10), puVar1 != (undefined4 *)0x0 &&
               ((puVar1[10] != *(int *)(puVar6 + 8) || (uVar5 != puVar1[0xc]))))) {
          *(undefined4 **)(puVar6 + -0x10) = puVar1;
        }
        if ((puVar1 == (undefined4 *)0x0) || (puVar1 != (undefined4 *)**(int **)(puVar6 + -0x10))) {
          FUN_00423084(s_xdrPtr____xdrPtr_____xdrLPP_0042f7b6,s_xx_cpp_0042f7d2,0x8ba);
        }
        **(undefined4 **)(puVar6 + -0x10) = *puVar1;
        FUN_00424707((int)puVar1);
        (*(code *)puVar1[7])();
      }
      else if (uVar4 - 1 == 4) {
        uVar3 = FUN_00425622(*(int **)(*(int *)(puVar6 + -4) + uVar5 + 8),
                             *(int *)(*(int *)(puVar6 + -4) + uVar5 + 4) + *(int *)(puVar6 + -0xc),
                             *(int *)(puVar6 + 8),*(int *)(puVar6 + -8));
        *(undefined4 *)(puVar6 + -0xc) = uVar3;
      }
      else {
        FUN_00423084(s___bogus_context_in_Local_unwind__0042f7f9,s_xx_cpp_0042f81c,0x8e9);
      }
    }
  }
  return;
}



void __cdecl FUN_00424cb4(int param_1)

{
  FUN_00424b49(param_1);
  return;
}



void __cdecl FUN_00424cc7(undefined4 *param_1)

{
  undefined4 *puVar1;
  undefined2 in_FS;
  
  FUN_00424b49((int)param_1);
  puVar1 = (undefined4 *)segment(in_FS,0);
  *puVar1 = *param_1;
  return;
}



int * __cdecl FUN_00424ce3(int param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = (int *)(param_1 + 8);
  while( true ) {
    if (*piVar2 == 0) {
      return (int *)0x0;
    }
    if ((int *)piVar2[1] == (int *)0x0) break;
    iVar1 = FUN_0042400d(*(int **)(param_2 + 4),*(undefined4 *)(param_2 + 8),(int *)piVar2[1],
                         (byte)*(undefined4 *)(param_2 + 0xc));
    if (iVar1 != 0) {
      return piVar2;
    }
    piVar2 = piVar2 + 5;
  }
  return piVar2;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 __cdecl FUN_00424d28(int *param_1,undefined4 *param_2)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  int iVar3;
  int *piVar4;
  char *pcVar5;
  int *piVar6;
  uint uVar7;
  undefined1 *puVar8;
  int iVar9;
  undefined *UNRECOVERED_JUMPTABLE;
  undefined2 in_FS;
  
  puVar8 = &stack0xfffffffc;
  iVar9 = param_1[7];
  piVar6 = (int *)param_2[2];
  if (*param_1 == 0xeedfae6) {
    uVar2 = 0;
  }
  else {
    FUN_00421574();
    if ((*(byte *)(param_1 + 1) & 6) == 0) {
      uVar7 = (uint)*(ushort *)(param_2 + 4);
      while (uVar7 != 0) {
        *(uint *)(puVar8 + -8) = (uint)*(ushort *)((int)piVar6 + uVar7);
        *(uint *)(puVar8 + -0x14) = (uint)*(ushort *)((int)piVar6 + uVar7 + 2);
        iVar9 = uVar7 + 4;
        switch(*(undefined4 *)(puVar8 + -0x14)) {
        case 0:
        case 4:
        case 5:
          break;
        case 1:
          if (**(int **)(puVar8 + 8) != 0xeefface) {
            *(undefined4 *)(puVar8 + -0x28) = *(undefined4 *)(puVar8 + 8);
            *(undefined4 *)(puVar8 + -0x24) = *(undefined4 *)(puVar8 + 0x10);
            *(undefined4 *)(*(int *)(puVar8 + 0xc) + 0x14) = **(undefined4 **)(puVar8 + 8);
            *(undefined1 **)(*(int *)(puVar8 + 0xc) + 0x18) = puVar8 + -0x28;
            _DAT_00431710 = *(undefined4 *)((int)piVar6 + iVar9);
            iVar3 = FUN_0041f64f();
LAB_00425013:
            if (iVar3 < 0) {
              if ((*(byte *)(*(int *)(puVar8 + 8) + 4) & 1) != 0) {
                *(undefined2 *)(*(int *)(puVar8 + 0xc) + 0x10) = *(undefined2 *)(puVar8 + -8);
              }
              return 0;
            }
            if (iVar3 != 0) {
              *(undefined4 *)(*(int *)(puVar8 + 0xc) + 0x18) = 0;
              *(undefined4 *)(puVar8 + -0x20) = *(undefined4 *)(puVar8 + -8);
              UNRECOVERED_JUMPTABLE = *(undefined **)((int)piVar6 + iVar9 + 4);
LAB_00424ee6:
              FUN_00424653(*(PVOID *)(puVar8 + 0xc),*(PEXCEPTION_RECORD *)(puVar8 + 8));
              FUN_00424b49(*(int *)(puVar8 + 0xc));
              *(undefined2 *)(*(int *)(puVar8 + 0xc) + 0x10) = *(undefined2 *)(puVar8 + -0x20);
              if (*(int *)(puVar8 + -0x14) == 3) {
                FUN_00424880(*(int *)(puVar8 + -0x18),*(int *)(puVar8 + 0xc),*(int *)(puVar8 + -4),
                             *(int **)(puVar8 + -0x1c),*(int *)(puVar8 + -0xc));
              }
              iVar9 = FUN_00401202();
              *(uint *)(iVar9 + 0x14) = *(uint *)(iVar9 + 0x14) & 0xfffffffd;
              if ((*DAT_00431708 == 2) && (*(int *)(puVar8 + -0x14) == 3)) {
                pcVar5 = FUN_0041efd8(*(int *)(*(int *)(puVar8 + -4) + 4));
                FUN_004241e1(pcVar5);
                FUN_004241f9(3,5);
              }
              FUN_00423f8f(UNRECOVERED_JUMPTABLE);
            }
          }
          break;
        case 2:
          if (**(int **)(puVar8 + 8) != 0xeefface) {
            *(undefined4 *)(*(int *)(puVar8 + 0xc) + 0x14) = **(undefined4 **)(puVar8 + 8);
            iVar3 = *(int *)((int)piVar6 + iVar9);
            goto LAB_00425013;
          }
          break;
        case 3:
          if (**(int **)(puVar8 + 8) == 0xeefface) {
            *(undefined4 *)(puVar8 + -0x1c) = *(undefined4 *)((int)piVar6 + iVar9);
            piVar4 = FUN_00424ce3(*(int *)(puVar8 + -0x1c),*(int *)(puVar8 + -4));
            *(int **)(puVar8 + -0x18) = piVar4;
            if (*(int *)(puVar8 + -0x18) != 0) {
              if (((*DAT_00431708 == 1) || (*DAT_00431708 == 2)) &&
                 ((**(uint **)(puVar8 + 8) < 0xeedface || (0xeefface < **(uint **)(puVar8 + 8))))) {
                FUN_004241f9(2,3);
              }
              iVar9 = FUN_00401202();
              **(undefined4 **)(puVar8 + -4) = *(undefined4 *)(iVar9 + 0x10);
              iVar9 = FUN_00401202();
              *(undefined4 *)(iVar9 + 0x10) = *(undefined4 *)(puVar8 + -4);
              *(undefined4 *)(*(int *)(puVar8 + -4) + 0x28) = *(undefined4 *)(puVar8 + 0xc);
              *(undefined4 *)(*(int *)(puVar8 + -4) + 0x2c) = *(undefined4 *)(puVar8 + -0x18);
              *(uint *)(*(int *)(puVar8 + -4) + 0x30) = uVar7 + 8;
              *(uint *)(puVar8 + -0x20) = uVar7 + 8;
              UNRECOVERED_JUMPTABLE = (undefined *)**(undefined4 **)(puVar8 + -0x18);
              goto LAB_00424ee6;
            }
          }
          break;
        default:
          FUN_00423084(s___bogus_context_in__ExceptionHan_0042f848,s_xx_cpp_0042f870,0xc00);
        }
        uVar7 = *(uint *)(puVar8 + -8);
      }
      uVar2 = 1;
    }
    else {
      FUN_00424b49((int)param_2);
      if ((*param_1 == 0xeefface) && (piVar6 = (int *)*piVar6, piVar6 != (int *)0x0)) {
        for (; *piVar6 != 0; piVar6 = piVar6 + 1) {
          iVar3 = FUN_0042400d(*(int **)(iVar9 + 4),*(undefined4 *)(iVar9 + 8),(int *)*piVar6,
                               (byte)*(undefined4 *)(iVar9 + 0xc));
          if (iVar3 != 0) goto LAB_00424dd8;
        }
        puVar1 = (undefined4 *)segment(in_FS,0);
        uVar2 = *puVar1;
        puVar1 = (undefined4 *)segment(in_FS,0);
        *puVar1 = *param_2;
        FUN_0041f3b8();
        puVar1 = (undefined4 *)segment(in_FS,0);
        *puVar1 = uVar2;
      }
LAB_00424dd8:
      uVar2 = 1;
    }
  }
  return uVar2;
}



void __cdecl FUN_00425085(undefined4 param_1,int param_2,uint param_3,int param_4,int param_5)

{
  undefined4 *puVar1;
  int iVar2;
  undefined2 in_FS;
  undefined4 local_28;
  
  BCB_InitExceptionHandler(0x42f4f8);
  if ((*(byte *)(param_2 + 0xc) & 2) == 0) {
    FUN_00423084(s_varType_>tpClass_tpcFlags___CF_H_0042f877,s_xx_cpp_0042f89f,0xc1c);
  }
  if (*(int *)(param_2 + 0x28) == 0) {
    FUN_00423084(s_varType_>tpClass_tpcDtorAddr_0042f8a6,s_xx_cpp_0042f8c3,0xc1d);
  }
  if ((*(uint *)(param_5 + 0x1c) < *(uint *)(param_2 + 0x20)) && (param_3 == 0)) {
    FUN_00423084(s__errPtr_>ERRcInitDtc_>__varType__0042f8ca,s_xx_cpp_0042f90a,0xc28);
  }
  if ((param_3 & 2) == 0) {
    if (param_4 == 0) {
      iVar2 = *(int *)(param_2 + 0x24);
    }
    else {
      iVar2 = *(int *)(param_2 + 0x20);
    }
    *(int *)(param_5 + 0x1c) = *(int *)(param_5 + 0x1c) - iVar2;
  }
  FUN_00424216(param_1,param_2,(byte)param_3,*(undefined **)(param_2 + 0x28),
               (uint)*(ushort *)(param_2 + 0x2c),param_4);
  puVar1 = (undefined4 *)segment(in_FS,0);
  *puVar1 = local_28;
  return;
}



void __cdecl FUN_0042517c(int param_1,uint param_2,int *param_3,int *param_4)

{
  undefined4 *puVar1;
  int *piVar2;
  uint in_stack_00000014;
  int in_stack_00000018;
  undefined4 in_stack_0000001c;
  int in_stack_00000020;
  
  while (piVar2 = param_3 + -3, param_4 <= piVar2) {
    puVar1 = (undefined4 *)(param_3[-2] + param_1);
    if (in_stack_00000018 != 0) {
      puVar1 = (undefined4 *)*puVar1;
    }
    param_3 = piVar2;
    if ((*(byte *)(*piVar2 + 0xc) & 2) != 0) {
      FUN_004251c7(puVar1,*piVar2,param_2,in_stack_00000014,0,in_stack_0000001c,in_stack_00000020);
      in_stack_00000014 = 0;
    }
  }
  return;
}



void __cdecl
FUN_004251c7(int param_1,int param_2,uint param_3,uint param_4,int param_5,undefined4 param_6,
            int param_7)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  uint uVar4;
  int *piVar5;
  int *piVar6;
  uint local_1c;
  int *local_14;
  int *local_c;
  
  if ((*(byte *)(param_2 + 0xc) & 2) == 0) {
    FUN_00423084(s_varType_>tpClass_tpcFlags___CF_H_0042f911,s_xx_cpp_0042f939,0xcb3);
  }
  if (param_5 == 0) {
    uVar4 = *(uint *)(param_2 + 0x24);
  }
  else {
    uVar4 = *(uint *)(param_2 + 0x20);
  }
  if ((param_4 == 0) || (uVar4 <= param_4)) {
    FUN_00425085(param_1,param_2,param_3,param_5,param_7);
  }
  else {
    if (uVar4 <= param_4) {
      FUN_00423084(s_dtorCnt_<_varCount_0042f940,s_xx_cpp_0042f953,0xced);
    }
    piVar3 = (int *)((uint)*(ushort *)(param_2 + 0x12) + param_2);
    local_14 = piVar3;
    if (param_5 != 0) {
      for (; iVar2 = *local_14, iVar2 != 0; local_14 = local_14 + 3) {
        if ((*(byte *)(iVar2 + 4) & 1) == 0) {
          FUN_00423084(s_IS_STRUC_blType_>tpMask__0042f95a,s_xx_cpp_0042f973,0xd18);
        }
        if ((*(byte *)(iVar2 + 0xc) & 2) != 0) {
          if (param_4 <= *(uint *)(iVar2 + 0x24)) {
            FUN_0042517c(param_1,param_3,local_14 + 3,piVar3);
            return;
          }
          param_4 = param_4 - *(uint *)(iVar2 + 0x24);
        }
      }
    }
    piVar1 = (int *)((uint)*(ushort *)(param_2 + 0x10) + param_2);
    for (local_c = piVar1; iVar2 = *local_c, iVar2 != 0; local_c = local_c + 3) {
      if ((*(byte *)(iVar2 + 4) & 1) == 0) {
        FUN_00423084(s_IS_STRUC_blType_>tpMask__0042f97a,s_xx_cpp_0042f993,0xd3f);
      }
      if ((*(byte *)(iVar2 + 0xc) & 2) != 0) {
        if (param_4 <= *(uint *)(iVar2 + 0x24)) {
          FUN_0042517c(param_1,param_3,local_c + 3,piVar1);
          if (param_5 == 0) {
            return;
          }
          FUN_0042517c(param_1,param_3,local_14,piVar3);
          return;
        }
        param_4 = param_4 - *(uint *)(iVar2 + 0x24);
      }
    }
    piVar5 = (int *)((uint)*(ushort *)(param_2 + 0x2e) + param_2);
    piVar6 = piVar5;
    while( true ) {
      iVar2 = *piVar6;
      if (iVar2 == 0) {
        FUN_00423084(s_memType_0042f99a,s_xx_cpp_0042f9a2,0xd65);
      }
      local_1c = 1;
      if ((*(byte *)(iVar2 + 5) & 4) != 0) {
        local_1c = *(int *)(iVar2 + 0xc);
        iVar2 = *(int *)(iVar2 + 8);
      }
      if ((*(byte *)(iVar2 + 0xc) & 2) == 0) {
        FUN_00423084(s_memType_>tpClass_tpcFlags___CF_H_0042f9a9,s_xx_cpp_0042f9d1,0xd73);
      }
      local_1c = local_1c * *(int *)(iVar2 + 0x20);
      if (param_4 <= local_1c) break;
      param_4 = param_4 - local_1c;
      piVar6 = piVar6 + 2;
    }
    do {
      iVar2 = *piVar6;
      local_1c = 1;
      if ((*(byte *)(iVar2 + 5) & 4) != 0) {
        local_1c = *(uint *)(iVar2 + 0xc);
        iVar2 = *(int *)(iVar2 + 8);
      }
      if (local_1c < 2) {
        FUN_004251c7(piVar6[1] + param_1,iVar2,0,param_4,1,param_6,param_7);
      }
      else {
        FUN_00425477(piVar6[1] + param_1,*piVar6,param_4,param_6,param_7);
      }
      param_4 = 0;
      piVar6 = piVar6 + -2;
    } while (piVar5 <= piVar6);
    FUN_0042517c(param_1,param_3,local_c,piVar1);
    if (param_5 != 0) {
      FUN_0042517c(param_1,param_3,local_14,piVar3);
    }
  }
  return;
}



void __cdecl FUN_00425477(int param_1,int param_2,uint param_3,undefined4 param_4,int param_5)

{
  uint uVar1;
  int *piVar2;
  uint uVar3;
  int iVar4;
  uint local_c;
  
  if ((*(byte *)(param_2 + 5) & 4) == 0) {
    FUN_00423084(s_varType_>tpMask___TM_IS_ARRAY_0042f9d8,s_xx_cpp_0042f9f6,0xdcc);
  }
  if ((*(byte *)(*(int *)(param_2 + 8) + 0xc) & 2) == 0) {
    FUN_00423084(s_varType_>tpArr_tpaElemType_>tpCl_0042f9fd,s_xx_cpp_0042fa38,0xdcd);
  }
  uVar1 = *(uint *)(param_2 + 0xc);
  piVar2 = *(int **)(param_2 + 8);
  uVar3 = piVar2[8];
  if (uVar3 == 0) {
    FUN_00423084(s_vdtCount_0042fa3f,s_xx_cpp_0042fa48,0xdd6);
  }
  if (param_3 == 0) {
    param_3 = uVar3 * uVar1;
  }
  local_c = param_3 / uVar3;
  if ((uVar1 < local_c) && (uVar1 != 0)) {
    FUN_00423084(s_etdCount_<__elemCount____elemCou_0042fa4f,s_xx_cpp_0042fa77,0xddf);
  }
  uVar1 = param_3 - local_c * uVar3;
  if (uVar3 < uVar1) {
    FUN_00423084(s_dtrCount_<__vdtCount_0042fa7e,s_xx_cpp_0042fa93,0xde0);
  }
  iVar4 = param_1 + local_c * *piVar2;
  if (uVar1 != 0) {
    FUN_004251c7(iVar4,(int)piVar2,0,uVar1,1,param_4,param_5);
  }
  while (local_c != 0) {
    iVar4 = iVar4 - *piVar2;
    FUN_004251c7(iVar4,(int)piVar2,0,uVar3,1,param_4,param_5);
    local_c = local_c - 1;
  }
  return;
}



int __cdecl FUN_0042559b(int param_1,int *param_2,int param_3)

{
  int iVar1;
  
  iVar1 = *param_2;
  if ((*(byte *)(iVar1 + 4) & 2) == 0) {
    FUN_00423084(s_IS_CLASS_varType_>tpMask__0042fa9a,s_xx_cpp_0042fab4,0xe13);
  }
  if (((*(uint *)(iVar1 + 0xc) & 0x50) == 0x50) && (*(int *)(iVar1 + 8) != -1)) {
    if (param_3 == 0) {
      param_3 = *(int *)(*(int *)(iVar1 + 8) + param_1);
    }
    param_1 = param_1 - *(int *)(param_3 + -8);
    iVar1 = param_3 - *(int *)(param_3 + -4);
    if (*(int *)(iVar1 + -4) != 0) {
      FUN_00423084(s___unsigned___far___vftAddr___1____0042fabb,s_xx_cpp_0042fae0,0xe39);
    }
    *param_2 = *(int *)(iVar1 + -0xc);
  }
  return param_1;
}



undefined4 __cdecl FUN_00425622(int *param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  bool bVar2;
  undefined4 uVar3;
  uint uVar4;
  int *piVar5;
  int iVar6;
  uint uVar7;
  int iVar8;
  int *piVar9;
  int *local_20;
  int *local_1c;
  int local_18;
  int *local_14;
  int local_10;
  int local_c;
  uint local_8;
  
  local_c = 0;
  if (param_1 == (int *)0x0) {
    uVar3 = 0;
  }
  else {
    local_8 = *(int *)(param_3 + 0x1c) - param_2;
    if ((*(byte *)(param_1 + 1) & 0x20) != 0) {
      if ((*(byte *)(param_1 + 1) & 0x11) == 0) {
        FUN_00423084(s_dttPtr_>dttFlags____DTCVF_PTRVAL_0042fae7,s_xx_cpp_0042fb16,0xe78);
      }
      if ((*(byte *)(*param_1 + 4) & 0x10) == 0) {
        FUN_00423084(s_dttPtr_>dttType_>tpMask___TM_IS__0042fb1d,s_xx_cpp_0042fb41,0xe7c);
      }
      if ((*(byte *)(*(int *)(*param_1 + 8) + 0xc) & 2) == 0) {
        FUN_00423084(s_dttPtr_>dttType_>tpPtr_tppBaseTy_0042fb48,s_xx_cpp_0042fb8b,0xe7d);
      }
      local_8 = *(uint *)(*(int *)(*param_1 + 8) + 0x20);
    }
    if ((*(byte *)((int)param_1 + 5) & 1) == 0) {
      piVar5 = param_1;
      if (((int)local_8 < 1) && ((*(byte *)((int)param_1 + 5) & 4) == 0)) {
        if ((param_1[1] & 3U) != 3) {
          return 0;
        }
        local_c = local_c + 1;
      }
      else {
        for (; *piVar5 != 0; piVar5 = piVar5 + 3) {
          if ((*(byte *)((int)piVar5 + 5) & 0x10) == 0) {
            local_10 = *piVar5;
          }
          else {
            if ((*(byte *)(*piVar5 + 5) & 4) == 0) {
              FUN_00423084(s_dtvtPtr_>dttType_>tpMask___TM_IS_0042fbf0,s_xx_cpp_0042fc17,0xed7);
            }
            local_10 = *(int *)(*piVar5 + 8);
          }
          iVar8 = 1;
          if (((((*(byte *)(local_10 + 4) & 0x10) != 0) &&
               (local_10 = *(int *)(local_10 + 8), (*(byte *)(local_10 + 4) & 2) != 0)) &&
              ((*(byte *)(local_10 + 0xc) & 0x20) != 0)) && ((*(byte *)(piVar5 + 1) & 8) != 0)) {
            iVar6 = 0;
            if ((*(byte *)(local_10 + 0xc) & 2) == 0) {
              FUN_00423084(s_varType_>tpClass_tpcFlags___CF_H_0042fc1e,s_xx_cpp_0042fc46,0xef7);
            }
            if ((*(byte *)(piVar5 + 1) & 4) == 0) {
              local_14 = (int *)piVar5[2];
            }
            else {
              local_14 = (int *)(param_4 + piVar5[2]);
            }
            iVar1 = *local_14;
            if ((*(byte *)((int)piVar5 + 5) & 0x10) != 0) {
              iVar6 = local_14[1];
            }
            local_14 = (int *)iVar1;
            if ((*(byte *)(piVar5 + 1) & 0x40) != 0) {
              local_14 = (int *)(iVar1 + 4);
            }
            local_14 = (int *)FUN_0042559b((int)local_14,&local_10,iVar6);
          }
          iVar6 = local_10;
          if ((*(byte *)(local_10 + 5) & 4) != 0) {
            iVar8 = *(int *)(local_10 + 0xc);
            iVar6 = *(int *)(local_10 + 8);
          }
          if ((*(byte *)(iVar6 + 0xc) & 2) == 0) {
            FUN_00423084(s_elemType_>tpClass_tpcFlags___CF__0042fc4d,s_xx_cpp_0042fc76,0xf22);
          }
          uVar7 = iVar8 * *(int *)(iVar6 + 0x20);
          if (local_8 <= uVar7) goto LAB_0042585d;
          local_8 = local_8 - uVar7;
        }
        piVar5 = piVar5 + -3;
      }
LAB_0042585d:
      do {
        local_18 = *piVar5;
        uVar7 = piVar5[1];
        if ((uVar7 & 4) == 0) {
          local_1c = (int *)piVar5[2];
        }
        else {
          local_1c = (int *)(param_4 + piVar5[2]);
        }
        if ((uVar7 & 0x1000) != 0) {
          if ((*(byte *)(local_18 + 5) & 4) == 0) {
            FUN_00423084(s_varType_>tpMask___TM_IS_ARRAY_0042fc7d,s_xx_cpp_0042fc9b,0xf52);
          }
          local_18 = *(int *)(local_18 + 8);
        }
        if ((uVar7 & 0x11) != 0) {
          iVar8 = 0;
          if ((*(byte *)(local_18 + 4) & 0x10) == 0) {
            FUN_00423084(s_varType_>tpMask___TM_IS_PTR_0042fca2,s_xx_cpp_0042fcbe,0xf5c);
          }
          local_18 = *(int *)(local_18 + 8);
          local_20 = (int *)*local_1c;
          if ((uVar7 & 0x1000) != 0) {
            iVar8 = local_1c[1];
          }
          local_1c = local_20;
          if ((uVar7 & 0x48) == 0x40) {
            local_1c = local_20 + 1;
          }
          if ((((*(byte *)(local_18 + 4) & 2) != 0) && ((*(byte *)(local_18 + 0xc) & 0x20) != 0)) &&
             ((uVar7 & 8) != 0)) {
            local_20 = (int *)FUN_0042559b((int)local_1c,&local_18,iVar8);
            local_1c = local_20;
          }
        }
        if (local_c == 0) {
          if ((uVar7 & 0x400) == 0) {
            uVar4 = 0;
          }
          else {
            if (*(short *)(local_18 + 0x10) == 0) {
              uVar4 = 0;
            }
            else {
              piVar9 = (int *)((uint)*(ushort *)(local_18 + 0x10) + local_18);
              if (piVar9 == (int *)0x0) {
                FUN_00423084(&DAT_0042fcc5,s_xx_cpp_0042fcc8,0xf92);
              }
              uVar4 = *(uint *)(*piVar9 + 0x20);
            }
            if (local_8 < uVar4) {
              uVar4 = 2;
            }
            else {
              uVar4 = 1;
            }
          }
          if ((*(byte *)(local_18 + 5) & 4) == 0) {
            FUN_004251c7(local_1c,local_18,uVar4,local_8,1,param_4,param_3);
          }
          else {
            FUN_00425477((int)local_1c,local_18,local_8,param_4,param_3);
          }
          if ((((uVar7 & 0x400) != 0) && ((*(byte *)(param_3 + 0x20) & 1) != 0)) &&
             (*(char *)(param_3 + 0x20) != -1)) {
            (**(code **)(*local_1c + -8))(local_1c);
          }
        }
        if ((uVar7 & 3) == 3) {
          if ((uVar7 & 0x48) == 0x48) {
            local_20 = local_20 + -1;
          }
          if ((*(byte *)(local_18 + 5) & 4) == 0) {
            if (*(int *)(local_18 + 0x14) == 0) {
              BCB_FreeObject();
            }
            else {
              FUN_004242e5(local_20,*(undefined **)(local_18 + 0x14),
                           (uint)*(ushort *)(local_18 + 0x18));
            }
          }
          else {
            local_18 = *(int *)(local_18 + 8);
            if (*(int *)(local_18 + 0x1c) == 0) {
              FUN_0041d600();
            }
            else {
              FUN_004242e5(local_20,*(undefined **)(local_18 + 0x1c),
                           (uint)*(ushort *)(local_18 + 0x1a));
            }
          }
        }
        local_8 = 0;
        bVar2 = param_1 < piVar5;
        piVar5 = piVar5 + -3;
      } while (bVar2);
      uVar3 = 0;
    }
    else {
      if (((*(byte *)(*param_1 + 4) & 2) == 0) || ((*(byte *)(*param_1 + 0xc) & 2) == 0)) {
        FUN_00423084(s_IS_CLASS_dttPtr_>dttType_>tpMask_0042fb92,s_xx_cpp_0042fbe9,0xe8b);
      }
      uVar3 = *(undefined4 *)(*param_1 + 0x24);
    }
  }
  return uVar3;
}



uint __cdecl FUN_00425aa6(int param_1)

{
  int iVar1;
  
  if (param_1 == -1) {
    return 0xffffffff;
  }
  if (**(short **)(param_1 + 4) == 0x25ff) {
    iVar1 = **(int **)(*(int *)(param_1 + 4) + 2);
  }
  else {
    iVar1 = *(int *)(param_1 + 4);
  }
  iVar1 = FUN_0041e9f0((byte *)(iVar1 + -8),(byte *)s___BCCxh1_0042fccf,8);
  if (iVar1 != 0) {
    return 0xffffffff;
  }
  return (uint)*(ushort *)(param_1 + 0x10);
}



int __cdecl FUN_00425afc(int *param_1)

{
  return *param_1 + -0xc;
}



undefined4 FUN_00425b0c(int *param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  if (*param_1 == 0) {
    uVar2 = 0;
  }
  else {
    iVar1 = FUN_00425afc(param_1);
    uVar2 = *(undefined4 *)(iVar1 + 8);
  }
  return uVar2;
}



void __cdecl FUN_00425b28(int *param_1,int param_2)

{
  int iVar1;
  undefined4 *puVar2;
  undefined2 in_FS;
  undefined4 uVar3;
  code *pcVar4;
  uint uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 local_2c;
  int local_8;
  
  BCB_InitExceptionHandler(0x42fd0c);
  if (0 < param_2) {
    iVar1 = FUN_00425b0c(param_1);
    if (param_2 <= iVar1) goto LAB_00425b9d;
  }
  uVar8 = 0;
  uVar7 = 0;
  uVar6 = 0;
  uVar5 = 1;
  pcVar4 = FUN_00425f6c;
  uVar3 = 0;
  local_8 = 0;
  FUN_004265d4(&DAT_004309d0,&local_8);
  puVar2 = (undefined4 *)FUN_00425ec0((int *)&DAT_00413890,'\x01',local_8);
  FUN_004245b3((uint *)&DAT_004262dc,puVar2,uVar3,pcVar4,uVar5,uVar6,uVar7,uVar8);
LAB_00425b9d:
  puVar2 = (undefined4 *)segment(in_FS,0);
  *puVar2 = local_2c;
  return;
}



int * FUN_00425bac(int *param_1)

{
  undefined4 *puVar1;
  char *extraout_EDX;
  undefined2 in_FS;
  undefined4 local_2c;
  
  BCB_InitExceptionHandler(0x42fd30);
  *param_1 = 0;
  FUN_0041b9f8(param_1,extraout_EDX);
  puVar1 = (undefined4 *)segment(in_FS,0);
  *puVar1 = local_2c;
  return param_1;
}



int * FUN_00425be4(int *param_1)

{
  undefined4 *puVar1;
  undefined4 *extraout_EDX;
  undefined2 in_FS;
  undefined4 local_2c;
  
  BCB_InitExceptionHandler(0x42fd54);
  puVar1 = (undefined4 *)*extraout_EDX;
  *param_1 = 0;
  FUN_0041b894(param_1,puVar1);
  puVar1 = (undefined4 *)segment(in_FS,0);
  *puVar1 = local_2c;
  return param_1;
}



int * FUN_00425c20(int *param_1)

{
  undefined4 *puVar1;
  undefined2 in_FS;
  undefined4 local_2c;
  
  BCB_InitExceptionHandler(0x42fd78);
  *param_1 = 0;
  FUN_00425de8(param_1,&DAT_0042fcd8);
  puVar1 = (undefined4 *)segment(in_FS,0);
  *puVar1 = local_2c;
  return param_1;
}



int * FUN_00425c6c(int *param_1)

{
  undefined4 *puVar1;
  undefined2 in_FS;
  undefined4 local_2c;
  
  BCB_InitExceptionHandler(0x42fd9c);
  *param_1 = 0;
  FUN_00425de8(param_1,&DAT_0042fce3);
  puVar1 = (undefined4 *)segment(in_FS,0);
  *puVar1 = local_2c;
  return param_1;
}



void AnsiString_Free(int *param_1,uint param_2)

{
  if (param_1 != (int *)0x0) {
    FUN_0041b840(param_1);
    if ((param_2 & 1) != 0) {
      BCB_FreeObject();
    }
  }
  return;
}



int * AnsiString_Assign(int *param_1,undefined4 *param_2)

{
  FUN_0041b894(param_1,(undefined4 *)*param_2);
  return param_1;
}



int * FUN_00425cf4(int *param_1,undefined4 *param_2)

{
  FUN_0041bac8(param_1,(undefined4 *)*param_2);
  return param_1;
}



int * FUN_00425d08(undefined4 *param_1,undefined4 *param_2,int *param_3)

{
  undefined4 *puVar1;
  undefined2 in_FS;
  undefined4 local_30;
  int local_8;
  
  BCB_InitExceptionHandler(0x42fdf0);
  local_8 = 0;
  FUN_0041bb0c(&local_8,(undefined4 *)*param_1,(undefined4 *)*param_2);
  AnsiString_Assign(param_3,&local_8);
  AnsiString_Free(&local_8,2);
  puVar1 = (undefined4 *)segment(in_FS,0);
  *puVar1 = local_30;
  return param_3;
}



byte FUN_00425d94(undefined4 *param_1,undefined4 *param_2)

{
  byte in_ZF;
  
  FUN_0041bbd0((uint *)*param_1,(uint *)*param_2);
  return in_ZF & 1;
}



void __cdecl FUN_00425dac(int *param_1,byte *param_2,uint *param_3)

{
  uint uVar1;
  
  uVar1 = FUN_0041fdf8((undefined1 *)0x0,0,param_2,param_3);
  FUN_00425e18(param_1,uVar1);
  FUN_0041fdf8((undefined1 *)*param_1,uVar1,param_2,param_3);
  return;
}



int * __cdecl FUN_00425de8(int *param_1,byte *param_2)

{
  FUN_00425dac(param_1,param_2,(uint *)&stack0x0000000c);
  return param_1;
}



int * FUN_00425e04(int *param_1)

{
  FUN_0041bc80(param_1);
  return param_1;
}



int * FUN_00425e18(int *param_1,uint param_2)

{
  FUN_0041bd88(param_1,param_2);
  return param_1;
}



int * FUN_00425e38(undefined4 param_1,undefined4 *param_2,int *param_3)

{
  undefined4 *puVar1;
  undefined2 in_FS;
  undefined4 local_30;
  int local_8;
  
  BCB_InitExceptionHandler(0x42fe68);
  FUN_00425bac(&local_8);
  FUN_0041bac8(&local_8,(undefined4 *)*param_2);
  AnsiString_Assign(param_3,&local_8);
  AnsiString_Free(&local_8,2);
  puVar1 = (undefined4 *)segment(in_FS,0);
  *puVar1 = local_30;
  return param_3;
}



void FUN_00425ec0(int *param_1,char param_2,int param_3)

{
  undefined4 *puVar1;
  int *piVar2;
  int extraout_ECX;
  char extraout_DL;
  undefined2 in_FS;
  undefined4 local_34;
  int local_10;
  int *local_c;
  int local_8;
  
  if ('\0' < param_2) {
    param_1 = (int *)FUN_00426f74((int)param_1);
    param_3 = extraout_ECX;
    param_2 = extraout_DL;
  }
  local_c = param_1;
  local_8 = param_3;
  BCB_InitExceptionHandler(0x42fed0);
  FUN_00425be4(&local_8);
  piVar2 = FUN_00425be4(&local_10);
  FUN_00425fc8(local_c,'\0',*piVar2);
  AnsiString_Free(&local_10,2);
  AnsiString_Free(&local_8,2);
  puVar1 = (undefined4 *)segment(in_FS,0);
  *puVar1 = local_34;
  if (param_2 != '\0') {
    FUN_00426f81(local_c);
  }
  return;
}



void __cdecl FUN_00425f6c(int *param_1,char param_2,int param_3)

{
  undefined4 *puVar1;
  undefined2 in_FS;
  undefined4 local_28;
  
  if ('\0' < param_2) {
    param_1 = (int *)FUN_00426f74((int)param_1);
  }
  BCB_InitExceptionHandler(0x42ff08);
  FUN_00426074(param_1,'\0',param_3);
  puVar1 = (undefined4 *)segment(in_FS,0);
  *puVar1 = local_28;
  if (param_2 != '\0') {
    FUN_00426f81(param_1);
  }
  return;
}



void FUN_00425fc8(int *param_1,char param_2,int param_3)

{
  undefined4 *puVar1;
  int *piVar2;
  int extraout_ECX;
  char extraout_DL;
  undefined2 in_FS;
  undefined4 local_34;
  int local_10;
  int *local_c;
  int local_8;
  
  if ('\0' < param_2) {
    param_1 = (int *)FUN_00426f74((int)param_1);
    param_3 = extraout_ECX;
    param_2 = extraout_DL;
  }
  local_c = param_1;
  local_8 = param_3;
  BCB_InitExceptionHandler(0x42ff40);
  FUN_00425be4(&local_8);
  piVar2 = FUN_00425be4(&local_10);
  FUN_004260d0(local_c,'\0',*piVar2);
  AnsiString_Free(&local_10,2);
  AnsiString_Free(&local_8,2);
  puVar1 = (undefined4 *)segment(in_FS,0);
  *puVar1 = local_34;
  if (param_2 != '\0') {
    FUN_00426f81(local_c);
  }
  return;
}



void __cdecl FUN_00426074(int *param_1,char param_2,int param_3)

{
  undefined4 *puVar1;
  undefined2 in_FS;
  undefined4 local_28;
  
  if ('\0' < param_2) {
    param_1 = (int *)FUN_00426f74((int)param_1);
  }
  BCB_InitExceptionHandler(0x42ff78);
  FUN_0042617c(param_1,'\0',param_3);
  puVar1 = (undefined4 *)segment(in_FS,0);
  *puVar1 = local_28;
  if (param_2 != '\0') {
    FUN_00426f81(param_1);
  }
  return;
}



void FUN_004260d0(int *param_1,char param_2,int param_3)

{
  undefined4 *puVar1;
  int *piVar2;
  int extraout_ECX;
  char extraout_DL;
  undefined2 in_FS;
  undefined4 local_34;
  int local_10;
  int *local_c;
  int local_8;
  
  if ('\0' < param_2) {
    param_1 = (int *)FUN_00426f74((int)param_1);
    param_3 = extraout_ECX;
    param_2 = extraout_DL;
  }
  local_c = param_1;
  local_8 = param_3;
  BCB_InitExceptionHandler(0x42ffb0);
  FUN_00425be4(&local_8);
  piVar2 = FUN_00425be4(&local_10);
  FUN_00416c94(local_c,'\0',(undefined4 *)*piVar2);
  AnsiString_Free(&local_10,2);
  AnsiString_Free(&local_8,2);
  puVar1 = (undefined4 *)segment(in_FS,0);
  *puVar1 = local_34;
  if (param_2 != '\0') {
    FUN_00426f81(local_c);
  }
  return;
}



void __cdecl FUN_0042617c(int *param_1,char param_2,int param_3)

{
  undefined4 *puVar1;
  undefined2 in_FS;
  undefined4 local_28;
  
  if ('\0' < param_2) {
    param_1 = (int *)FUN_00426f74((int)param_1);
  }
  BCB_InitExceptionHandler(0x42ffe8);
  FUN_004261dc(param_1,'\0',param_3);
  param_1[3] = *(int *)(param_3 + 0xc);
  puVar1 = (undefined4 *)segment(in_FS,0);
  *puVar1 = local_28;
  if (param_2 != '\0') {
    FUN_00426f81(param_1);
  }
  return;
}



void __cdecl FUN_004261dc(int *param_1,char param_2,int param_3)

{
  undefined4 *puVar1;
  undefined2 in_FS;
  undefined4 local_28;
  
  if ('\0' < param_2) {
    param_1 = (int *)FUN_00426f74((int)param_1);
  }
  BCB_InitExceptionHandler(0x42fff0);
  FUN_0042624c(param_1,'\0');
  FUN_00425be4(param_1 + 1);
  param_1[2] = *(int *)(param_3 + 8);
  puVar1 = (undefined4 *)segment(in_FS,0);
  *puVar1 = local_28;
  if (param_2 != '\0') {
    FUN_00426f81(param_1);
  }
  return;
}



void __cdecl FUN_0042624c(int *param_1,char param_2)

{
  undefined4 *puVar1;
  undefined2 in_FS;
  undefined4 local_28;
  
  if ('\0' < param_2) {
    param_1 = (int *)FUN_00426f74((int)param_1);
  }
  BCB_InitExceptionHandler(0x42fff8);
  puVar1 = (undefined4 *)segment(in_FS,0);
  *puVar1 = local_28;
  if (param_2 != '\0') {
    FUN_00426f81(param_1);
  }
  return;
}



void FUN_00426390(int *param_1,char param_2)

{
  undefined4 *puVar1;
  int *piVar2;
  char extraout_DL;
  undefined2 in_FS;
  undefined4 local_2c;
  
  piVar2 = FUN_00426f89(param_1,param_2);
  BCB_InitExceptionHandler(0x430034);
  if (-1 < extraout_DL) {
    FUN_004263e4(piVar2,'\0');
  }
  puVar1 = (undefined4 *)segment(in_FS,0);
  *puVar1 = local_2c;
  if ('\0' < extraout_DL) {
    FUN_00426f7c(piVar2);
  }
  return;
}



void FUN_004263e4(int *param_1,char param_2)

{
  undefined4 *puVar1;
  int *piVar2;
  char extraout_DL;
  undefined2 in_FS;
  undefined4 local_2c;
  
  piVar2 = FUN_00426f89(param_1,param_2);
  BCB_InitExceptionHandler(0x430058);
  if (-1 < extraout_DL) {
    FUN_00426438(piVar2,'\0');
  }
  puVar1 = (undefined4 *)segment(in_FS,0);
  *puVar1 = local_2c;
  if ('\0' < extraout_DL) {
    FUN_00426f7c(piVar2);
  }
  return;
}



void FUN_00426438(int *param_1,char param_2)

{
  undefined4 *puVar1;
  int *piVar2;
  char extraout_DL;
  undefined2 in_FS;
  undefined4 local_2c;
  
  piVar2 = FUN_00426f89(param_1,param_2);
  BCB_InitExceptionHandler(0x43007c);
  if (-1 < extraout_DL) {
    AnsiString_Free(piVar2 + 1,2);
    FUN_0041ae44(piVar2,'\0');
  }
  puVar1 = (undefined4 *)segment(in_FS,0);
  *puVar1 = local_2c;
  if ('\0' < extraout_DL) {
    FUN_00426f7c(piVar2);
  }
  return;
}



int * FUN_004265d4(undefined4 *param_1,int *param_2)

{
  undefined4 *puVar1;
  undefined2 in_FS;
  undefined4 local_30;
  int local_8;
  
  BCB_InitExceptionHandler(0x4300d0);
  local_8 = 0;
  FUN_0041d2c4(param_1,&local_8);
  AnsiString_Assign(param_2,&local_8);
  AnsiString_Free(&local_8,2);
  puVar1 = (undefined4 *)segment(in_FS,0);
  *puVar1 = local_30;
  return param_2;
}



void __stdcall FUN_00426654(void)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_28;
  
  BCB_InitExceptionHandler(0x430110);
  if (DAT_00430109 != '\0') {
    FUN_00426a08((VARIANTARG *)&DAT_00431724,2);
  }
  if (DAT_00430108 != '\0') {
    FUN_00426a08((VARIANTARG *)&DAT_00431714,2);
  }
  *in_FS_OFFSET = local_28;
  return;
}



int __cdecl FUN_004266a8(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  CHAR local_414 [520];
  CHAR local_20c [520];
  
  wsprintfA(local_20c,s__s____s__d_00430173,param_1,param_2,param_3);
  iVar1 = FUN_00426744(s__ASSERTE__0043017e,local_20c);
  if (iVar1 == 6) {
    wsprintfA(local_20c,s__s_failed____s__d_00430189,param_1,param_2,param_3);
    wsprintfA(local_414,&param_2_0043019b,param_2);
    FUN_00426780(local_20c,0x80004005);
  }
  return iVar1;
}



void __cdecl FUN_00426744(LPCSTR param_1,undefined4 param_2)

{
  CHAR local_20c [520];
  
  wsprintfA(local_20c,s__s_Press__Y_es_to_terminate___N__0043019e,param_2);
  MessageBoxA((HWND)0x0,local_20c,param_1,0x2023);
  return;
}



void __cdecl FUN_00426780(undefined4 param_1,DWORD param_2)

{
  int *piVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  undefined1 *puVar7;
  int local_10;
  int local_c;
  int local_8;
  
  BCB_InitExceptionHandler(0x4303e4);
  puVar7 = &LAB_00426870;
  uVar6 = 0;
  piVar1 = FUN_00425bac(&local_c);
  puVar5 = (undefined4 *)*piVar1;
  piVar1 = FUN_00425bac(&local_10);
  puVar4 = (undefined4 *)*piVar1;
  iVar3 = 0;
  piVar1 = FUN_00425bac(&local_8);
  puVar2 = (undefined4 *)
           FUN_00411cfc((int *)&DAT_00411b80,'\x01',(undefined4 *)*piVar1,iVar3,puVar4,puVar5,
                        param_2);
  FUN_004245b3((uint *)&DAT_00426800,puVar2,iVar3,(undefined *)puVar4,(uint)puVar5,param_2,uVar6,
               puVar7);
  return;
}



VARIANTARG * FUN_004268ec(VARIANTARG *param_1)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  
  BCB_InitExceptionHandler(0x430408);
  VariantInit(param_1);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



VARIANTARG * FUN_00426920(VARIANTARG *param_1,undefined4 *param_2)

{
  undefined4 *in_FS_OFFSET;
  VARIANTARG local_3c;
  undefined4 local_2c;
  undefined2 local_1c;
  VARIANTARG *local_8;
  
  local_8 = param_1;
  BCB_InitExceptionHandler(0x43042c);
  local_1c = 8;
  BCB_memcpy((undefined4 *)&local_3c,param_2,0x10);
  VariantInit(local_8);
  FUN_00426a30(local_8,&local_3c);
  *in_FS_OFFSET = local_2c;
  return local_8;
}



VARIANTARG * FUN_00426970(VARIANTARG *param_1,undefined4 param_2)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  
  BCB_InitExceptionHandler(0x430450);
  VariantInit(param_1);
  (param_1->n1).n2.vt = 3;
  *(undefined4 *)((int)&param_1->n1 + 8) = param_2;
  *in_FS_OFFSET = local_2c;
  return param_1;
}



VARIANTARG * FUN_004269b4(VARIANTARG *param_1,undefined4 *param_2)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  
  BCB_InitExceptionHandler(0x430474);
  VariantInit(param_1);
  (param_1->n1).n2.vt = 0x100;
  *(undefined4 *)((int)&param_1->n1 + 8) = 0;
  AnsiString_Assign((int *)((int)&param_1->n1 + 8),param_2);
  *in_FS_OFFSET = local_2c;
  return param_1;
}



void FUN_00426a08(VARIANTARG *param_1,uint param_2)

{
  if (param_1 != (VARIANTARG *)0x0) {
    FUN_00426ba0(param_1);
    if ((param_2 & 1) != 0) {
      BCB_FreeObject();
    }
  }
  return;
}



VARIANTARG * FUN_00426a30(VARIANTARG *param_1,VARIANTARG *param_2)

{
  int iVar1;
  
  FUN_00426ba0(param_1);
  FUN_0041c570(param_1,param_2);
  if ((param_1->n1).n2.vt != (param_2->n1).n2.vt) {
    iVar1 = FUN_004266a8(s_vt____rhs_vt_00430205,s_VARIANT_CPP_00430212,0x1a5);
    if (iVar1 == 2) {
      DebugBreak();
    }
  }
  return param_1;
}



uint * FUN_00426a78(uint *param_1,uint *param_2)

{
  FUN_0041ceb8(param_1,param_2);
  return param_1;
}



VARIANTARG * FUN_00426a8c(undefined4 *param_1,uint *param_2,VARIANTARG *param_3)

{
  VARIANTARG *pVVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 local_3c;
  VARIANTARG local_14;
  
  BCB_InitExceptionHandler(0x4304b4);
  pVVar1 = FUN_00426920(&local_14,param_1);
  pVVar1 = (VARIANTARG *)FUN_00426a78((uint *)pVVar1,param_2);
  FUN_00426a30(param_3,pVVar1);
  FUN_00426a08(&local_14,2);
  *in_FS_OFFSET = local_3c;
  return param_3;
}



int * FUN_00426b00(VARIANTARG *param_1,int *param_2)

{
  int *piVar1;
  undefined4 *in_FS_OFFSET;
  undefined4 local_40;
  int local_18;
  VARIANTARG local_14;
  
  BCB_InitExceptionHandler(0x430520);
  FUN_004268ec(&local_14);
  FUN_0041c784(&local_14,param_1,0x100);
  piVar1 = FUN_00425be4(&local_18);
  AnsiString_Assign(param_2,piVar1);
  AnsiString_Free(&local_18,2);
  FUN_00426a08(&local_14,2);
  *in_FS_OFFSET = local_40;
  return param_2;
}



void FUN_00426ba0(VARIANTARG *param_1)

{
  FUN_0041c52c(param_1);
  return;
}



void __cdecl FUN_00426bac(int *param_1,char param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_28;
  
  if ('\0' < param_2) {
    param_1 = (int *)FUN_00426f74((int)param_1);
  }
  BCB_InitExceptionHandler(0x430560);
  FUN_00426c0c(param_1,'\0',param_3);
  param_1[3] = *(int *)(param_3 + 0xc);
  *in_FS_OFFSET = local_28;
  if (param_2 != '\0') {
    FUN_00426f81(param_1);
  }
  return;
}



void __cdecl FUN_00426c0c(int *param_1,char param_2,int param_3)

{
  undefined4 *in_FS_OFFSET;
  undefined4 local_28;
  
  if ('\0' < param_2) {
    param_1 = (int *)FUN_00426f74((int)param_1);
  }
  BCB_InitExceptionHandler(0x430568);
  FUN_004261dc(param_1,'\0',param_3);
  *in_FS_OFFSET = local_28;
  if (param_2 != '\0') {
    FUN_00426f81(param_1);
  }
  return;
}



void FUN_00426c68(int *param_1,char param_2)

{
  int *piVar1;
  char extraout_DL;
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  
  piVar1 = FUN_00426f89(param_1,param_2);
  BCB_InitExceptionHandler(0x430580);
  if (-1 < extraout_DL) {
    AnsiString_Free(piVar1 + 5,2);
    AnsiString_Free(piVar1 + 4,2);
    FUN_00426d44(piVar1,'\0');
  }
  *in_FS_OFFSET = local_2c;
  if ('\0' < extraout_DL) {
    FUN_00426f7c(piVar1);
  }
  return;
}



void FUN_00426d44(int *param_1,char param_2)

{
  int *piVar1;
  char extraout_DL;
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  
  piVar1 = FUN_00426f89(param_1,param_2);
  BCB_InitExceptionHandler(0x4305a4);
  if (-1 < extraout_DL) {
    FUN_00426db8(piVar1,'\0');
  }
  *in_FS_OFFSET = local_2c;
  if ('\0' < extraout_DL) {
    FUN_00426f7c(piVar1);
  }
  return;
}



void FUN_00426db8(int *param_1,char param_2)

{
  int *piVar1;
  char extraout_DL;
  undefined4 *in_FS_OFFSET;
  undefined4 local_2c;
  
  piVar1 = FUN_00426f89(param_1,param_2);
  BCB_InitExceptionHandler(0x4305c8);
  if (-1 < extraout_DL) {
    FUN_00426438(piVar1,'\0');
  }
  *in_FS_OFFSET = local_2c;
  if ('\0' < extraout_DL) {
    FUN_00426f7c(piVar1);
  }
  return;
}



undefined4 * FUN_00426ea0(undefined4 *param_1,undefined4 param_2,char param_3)

{
  int *piVar1;
  undefined4 *in_FS_OFFSET;
  longdouble in_ST0;
  undefined8 local_40;
  undefined4 local_34;
  int local_10;
  int local_c;
  int local_8;
  
  BCB_InitExceptionHandler(0x43060c);
  if (param_3 == '\x02') {
    piVar1 = FUN_00425be4(&local_8);
    FUN_00416408(*piVar1);
    local_40 = (double)in_ST0;
    AnsiString_Free(&local_8,2);
  }
  else if (param_3 == '\0') {
    piVar1 = FUN_00425be4(&local_c);
    FUN_00416368(*piVar1);
    local_40 = (double)in_ST0;
    AnsiString_Free(&local_c,2);
  }
  else {
    piVar1 = FUN_00425be4(&local_10);
    FUN_004163b8(*piVar1);
    local_40 = (double)in_ST0;
    AnsiString_Free(&local_10,2);
  }
  *param_1 = (undefined4)local_40;
  param_1[1] = local_40._4_4_;
  *in_FS_OFFSET = local_34;
  return param_1;
}



void FUN_00426f74(int param_1)

{
  (**(code **)(param_1 + -0xc))();
  return;
}



void FUN_00426f7c(int *param_1)

{
                    // WARNING: Could not recover jumptable at 0x00426f7e. Too many branches
                    // WARNING: Treating indirect jump as call
  (**(code **)(*param_1 + -8))();
  return;
}



int * FUN_00426f81(int *param_1)

{
  (**(code **)(*param_1 + -0x1c))();
  return param_1;
}



int * FUN_00426f89(int *param_1,char param_2)

{
  if (param_2 < '\x01') {
    return param_1;
  }
  (**(code **)(*param_1 + -0x18))();
  return param_1;
}



LSTATUS __stdcall RegCloseKey(HKEY hKey)

{
  LSTATUS LVar1;
  
                    // WARNING: Could not recover jumptable at 0x00426fa8. Too many branches
                    // WARNING: Treating indirect jump as call
  LVar1 = RegCloseKey(hKey);
  return LVar1;
}



LSTATUS __stdcall
RegOpenKeyExA(HKEY hKey,LPCSTR lpSubKey,DWORD ulOptions,REGSAM samDesired,PHKEY phkResult)

{
  LSTATUS LVar1;
  
                    // WARNING: Could not recover jumptable at 0x00426fae. Too many branches
                    // WARNING: Treating indirect jump as call
  LVar1 = RegOpenKeyExA(hKey,lpSubKey,ulOptions,samDesired,phkResult);
  return LVar1;
}



LSTATUS __stdcall
RegQueryValueExA(HKEY hKey,LPCSTR lpValueName,LPDWORD lpReserved,LPDWORD lpType,LPBYTE lpData,
                LPDWORD lpcbData)

{
  LSTATUS LVar1;
  
                    // WARNING: Could not recover jumptable at 0x00426fb4. Too many branches
                    // WARNING: Treating indirect jump as call
  LVar1 = RegQueryValueExA(hKey,lpValueName,lpReserved,lpType,lpData,lpcbData);
  return LVar1;
}



BOOL __stdcall CloseHandle(HANDLE hObject)

{
  BOOL BVar1;
  
                    // WARNING: Could not recover jumptable at 0x00426fbc. Too many branches
                    // WARNING: Treating indirect jump as call
  BVar1 = CloseHandle(hObject);
  return BVar1;
}



int __stdcall
CompareStringA(LCID Locale,DWORD dwCmpFlags,PCNZCH lpString1,int cchCount1,PCNZCH lpString2,
              int cchCount2)

{
  int iVar1;
  
                    // WARNING: Could not recover jumptable at 0x00426fc2. Too many branches
                    // WARNING: Treating indirect jump as call
  iVar1 = CompareStringA(Locale,dwCmpFlags,lpString1,cchCount1,lpString2,cchCount2);
  return iVar1;
}



HANDLE __stdcall
CreateEventA(LPSECURITY_ATTRIBUTES lpEventAttributes,BOOL bManualReset,BOOL bInitialState,
            LPCSTR lpName)

{
  HANDLE pvVar1;
  
                    // WARNING: Could not recover jumptable at 0x00426fc8. Too many branches
                    // WARNING: Treating indirect jump as call
  pvVar1 = CreateEventA(lpEventAttributes,bManualReset,bInitialState,lpName);
  return pvVar1;
}



HANDLE __stdcall
CreateFileA(LPCSTR lpFileName,DWORD dwDesiredAccess,DWORD dwShareMode,
           LPSECURITY_ATTRIBUTES lpSecurityAttributes,DWORD dwCreationDisposition,
           DWORD dwFlagsAndAttributes,HANDLE hTemplateFile)

{
  HANDLE pvVar1;
  
                    // WARNING: Could not recover jumptable at 0x00426fce. Too many branches
                    // WARNING: Treating indirect jump as call
  pvVar1 = CreateFileA(lpFileName,dwDesiredAccess,dwShareMode,lpSecurityAttributes,
                       dwCreationDisposition,dwFlagsAndAttributes,hTemplateFile);
  return pvVar1;
}



void __stdcall DebugBreak(void)

{
                    // WARNING: Could not recover jumptable at 0x00426fd4. Too many branches
                    // WARNING: Treating indirect jump as call
  DebugBreak();
  return;
}



void __stdcall DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection)

{
                    // WARNING: Could not recover jumptable at 0x00426fda. Too many branches
                    // WARNING: Treating indirect jump as call
  DeleteCriticalSection(lpCriticalSection);
  return;
}



void __stdcall EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection)

{
                    // WARNING: Could not recover jumptable at 0x00426fe0. Too many branches
                    // WARNING: Treating indirect jump as call
  EnterCriticalSection(lpCriticalSection);
  return;
}



BOOL __stdcall
EnumCalendarInfoA(CALINFO_ENUMPROCA lpCalInfoEnumProc,LCID Locale,CALID Calendar,CALTYPE CalType)

{
  BOOL BVar1;
  
                    // WARNING: Could not recover jumptable at 0x00426fe6. Too many branches
                    // WARNING: Treating indirect jump as call
  BVar1 = EnumCalendarInfoA(lpCalInfoEnumProc,Locale,Calendar,CalType);
  return BVar1;
}



void __stdcall ExitProcess(UINT uExitCode)

{
                    // WARNING: Could not recover jumptable at 0x00426fec. Too many branches
                    // WARNING: Subroutine does not return
                    // WARNING: Treating indirect jump as call
  ExitProcess(uExitCode);
  return;
}



DWORD __stdcall
FormatMessageA(DWORD dwFlags,LPCVOID lpSource,DWORD dwMessageId,DWORD dwLanguageId,LPSTR lpBuffer,
              DWORD nSize,va_list *Arguments)

{
  DWORD DVar1;
  
                    // WARNING: Could not recover jumptable at 0x00426ff2. Too many branches
                    // WARNING: Treating indirect jump as call
  DVar1 = FormatMessageA(dwFlags,lpSource,dwMessageId,dwLanguageId,lpBuffer,nSize,Arguments);
  return DVar1;
}



BOOL __stdcall FreeEnvironmentStringsA(LPCH param_1)

{
  BOOL BVar1;
  
                    // WARNING: Could not recover jumptable at 0x00426ff8. Too many branches
                    // WARNING: Treating indirect jump as call
  BVar1 = FreeEnvironmentStringsA(param_1);
  return BVar1;
}



BOOL __stdcall FreeLibrary(HMODULE hLibModule)

{
  BOOL BVar1;
  
                    // WARNING: Could not recover jumptable at 0x00426ffe. Too many branches
                    // WARNING: Treating indirect jump as call
  BVar1 = FreeLibrary(hLibModule);
  return BVar1;
}



UINT __stdcall GetACP(void)

{
  UINT UVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427004. Too many branches
                    // WARNING: Treating indirect jump as call
  UVar1 = GetACP();
  return UVar1;
}



BOOL __stdcall GetCPInfo(UINT CodePage,LPCPINFO lpCPInfo)

{
  BOOL BVar1;
  
                    // WARNING: Could not recover jumptable at 0x0042700a. Too many branches
                    // WARNING: Treating indirect jump as call
  BVar1 = GetCPInfo(CodePage,lpCPInfo);
  return BVar1;
}



LPSTR __stdcall GetCommandLineA(void)

{
  LPSTR pCVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427010. Too many branches
                    // WARNING: Treating indirect jump as call
  pCVar1 = GetCommandLineA();
  return pCVar1;
}



DWORD __stdcall GetCurrentThreadId(void)

{
  DWORD DVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427016. Too many branches
                    // WARNING: Treating indirect jump as call
  DVar1 = GetCurrentThreadId();
  return DVar1;
}



int __stdcall
GetDateFormatA(LCID Locale,DWORD dwFlags,SYSTEMTIME *lpDate,LPCSTR lpFormat,LPSTR lpDateStr,
              int cchDate)

{
  int iVar1;
  
                    // WARNING: Could not recover jumptable at 0x0042701c. Too many branches
                    // WARNING: Treating indirect jump as call
  iVar1 = GetDateFormatA(Locale,dwFlags,lpDate,lpFormat,lpDateStr,cchDate);
  return iVar1;
}



BOOL __stdcall
GetDiskFreeSpaceA(LPCSTR lpRootPathName,LPDWORD lpSectorsPerCluster,LPDWORD lpBytesPerSector,
                 LPDWORD lpNumberOfFreeClusters,LPDWORD lpTotalNumberOfClusters)

{
  BOOL BVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427022. Too many branches
                    // WARNING: Treating indirect jump as call
  BVar1 = GetDiskFreeSpaceA(lpRootPathName,lpSectorsPerCluster,lpBytesPerSector,
                            lpNumberOfFreeClusters,lpTotalNumberOfClusters);
  return BVar1;
}



LPCH __stdcall GetEnvironmentStrings(void)

{
  LPCH pCVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427028. Too many branches
                    // WARNING: Treating indirect jump as call
  pCVar1 = GetEnvironmentStrings();
  return pCVar1;
}



DWORD __stdcall GetFileSize(HANDLE hFile,LPDWORD lpFileSizeHigh)

{
  DWORD DVar1;
  
                    // WARNING: Could not recover jumptable at 0x0042702e. Too many branches
                    // WARNING: Treating indirect jump as call
  DVar1 = GetFileSize(hFile,lpFileSizeHigh);
  return DVar1;
}



DWORD __stdcall GetFileType(HANDLE hFile)

{
  DWORD DVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427034. Too many branches
                    // WARNING: Treating indirect jump as call
  DVar1 = GetFileType(hFile);
  return DVar1;
}



DWORD __stdcall GetLastError(void)

{
  DWORD DVar1;
  
                    // WARNING: Could not recover jumptable at 0x0042703a. Too many branches
                    // WARNING: Treating indirect jump as call
  DVar1 = GetLastError();
  return DVar1;
}



void __stdcall GetLocalTime(LPSYSTEMTIME lpSystemTime)

{
                    // WARNING: Could not recover jumptable at 0x00427040. Too many branches
                    // WARNING: Treating indirect jump as call
  GetLocalTime(lpSystemTime);
  return;
}



int __stdcall GetLocaleInfoA(LCID Locale,LCTYPE LCType,LPSTR lpLCData,int cchData)

{
  int iVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427046. Too many branches
                    // WARNING: Treating indirect jump as call
  iVar1 = GetLocaleInfoA(Locale,LCType,lpLCData,cchData);
  return iVar1;
}



DWORD __stdcall GetModuleFileNameA(HMODULE hModule,LPSTR lpFilename,DWORD nSize)

{
  DWORD DVar1;
  
                    // WARNING: Could not recover jumptable at 0x0042704c. Too many branches
                    // WARNING: Treating indirect jump as call
  DVar1 = GetModuleFileNameA(hModule,lpFilename,nSize);
  return DVar1;
}



HMODULE __stdcall GetModuleHandleA(LPCSTR lpModuleName)

{
  HMODULE pHVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427052. Too many branches
                    // WARNING: Treating indirect jump as call
  pHVar1 = GetModuleHandleA(lpModuleName);
  return pHVar1;
}



UINT __stdcall GetOEMCP(void)

{
  UINT UVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427058. Too many branches
                    // WARNING: Treating indirect jump as call
  UVar1 = GetOEMCP();
  return UVar1;
}



FARPROC __stdcall GetProcAddress(HMODULE hModule,LPCSTR lpProcName)

{
  FARPROC pFVar1;
  
                    // WARNING: Could not recover jumptable at 0x0042705e. Too many branches
                    // WARNING: Treating indirect jump as call
  pFVar1 = GetProcAddress(hModule,lpProcName);
  return pFVar1;
}



HANDLE __stdcall GetProcessHeap(void)

{
  HANDLE pvVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427064. Too many branches
                    // WARNING: Treating indirect jump as call
  pvVar1 = GetProcessHeap();
  return pvVar1;
}



void __stdcall GetStartupInfoA(LPSTARTUPINFOA lpStartupInfo)

{
                    // WARNING: Could not recover jumptable at 0x0042706a. Too many branches
                    // WARNING: Treating indirect jump as call
  GetStartupInfoA(lpStartupInfo);
  return;
}



HANDLE __stdcall GetStdHandle(DWORD nStdHandle)

{
  HANDLE pvVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427070. Too many branches
                    // WARNING: Treating indirect jump as call
  pvVar1 = GetStdHandle(nStdHandle);
  return pvVar1;
}



BOOL __stdcall GetStringTypeW(DWORD dwInfoType,LPCWSTR lpSrcStr,int cchSrc,LPWORD lpCharType)

{
  BOOL BVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427076. Too many branches
                    // WARNING: Treating indirect jump as call
  BVar1 = GetStringTypeW(dwInfoType,lpSrcStr,cchSrc,lpCharType);
  return BVar1;
}



LCID __stdcall GetThreadLocale(void)

{
  LCID LVar1;
  
                    // WARNING: Could not recover jumptable at 0x0042707c. Too many branches
                    // WARNING: Treating indirect jump as call
  LVar1 = GetThreadLocale();
  return LVar1;
}



LCID __stdcall GetUserDefaultLCID(void)

{
  LCID LVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427082. Too many branches
                    // WARNING: Treating indirect jump as call
  LVar1 = GetUserDefaultLCID();
  return LVar1;
}



DWORD __stdcall GetVersion(void)

{
  DWORD DVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427088. Too many branches
                    // WARNING: Treating indirect jump as call
  DVar1 = GetVersion();
  return DVar1;
}



BOOL __stdcall GetVersionExA(LPOSVERSIONINFOA lpVersionInformation)

{
  BOOL BVar1;
  
                    // WARNING: Could not recover jumptable at 0x0042708e. Too many branches
                    // WARNING: Treating indirect jump as call
  BVar1 = GetVersionExA(lpVersionInformation);
  return BVar1;
}



void __stdcall GlobalMemoryStatus(LPMEMORYSTATUS lpBuffer)

{
                    // WARNING: Could not recover jumptable at 0x00427094. Too many branches
                    // WARNING: Treating indirect jump as call
  GlobalMemoryStatus(lpBuffer);
  return;
}



LPVOID __stdcall HeapAlloc(HANDLE hHeap,DWORD dwFlags,SIZE_T dwBytes)

{
  LPVOID pvVar1;
  
                    // WARNING: Could not recover jumptable at 0x0042709a. Too many branches
                    // WARNING: Treating indirect jump as call
  pvVar1 = HeapAlloc(hHeap,dwFlags,dwBytes);
  return pvVar1;
}



BOOL __stdcall HeapFree(HANDLE hHeap,DWORD dwFlags,LPVOID lpMem)

{
  BOOL BVar1;
  
                    // WARNING: Could not recover jumptable at 0x004270a0. Too many branches
                    // WARNING: Treating indirect jump as call
  BVar1 = HeapFree(hHeap,dwFlags,lpMem);
  return BVar1;
}



void __stdcall InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection)

{
                    // WARNING: Could not recover jumptable at 0x004270a6. Too many branches
                    // WARNING: Treating indirect jump as call
  InitializeCriticalSection(lpCriticalSection);
  return;
}



LONG __stdcall InterlockedDecrement(LONG *lpAddend)

{
  LONG LVar1;
  
                    // WARNING: Could not recover jumptable at 0x004270ac. Too many branches
                    // WARNING: Treating indirect jump as call
  LVar1 = InterlockedDecrement(lpAddend);
  return LVar1;
}



LONG __stdcall InterlockedIncrement(LONG *lpAddend)

{
  LONG LVar1;
  
                    // WARNING: Could not recover jumptable at 0x004270b2. Too many branches
                    // WARNING: Treating indirect jump as call
  LVar1 = InterlockedIncrement(lpAddend);
  return LVar1;
}



void __stdcall LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection)

{
                    // WARNING: Could not recover jumptable at 0x004270b8. Too many branches
                    // WARNING: Treating indirect jump as call
  LeaveCriticalSection(lpCriticalSection);
  return;
}



HLOCAL __stdcall LocalAlloc(UINT uFlags,SIZE_T uBytes)

{
  HLOCAL pvVar1;
  
                    // WARNING: Could not recover jumptable at 0x004270c4. Too many branches
                    // WARNING: Treating indirect jump as call
  pvVar1 = LocalAlloc(uFlags,uBytes);
  return pvVar1;
}



HLOCAL __stdcall LocalFree(HLOCAL hMem)

{
  HLOCAL pvVar1;
  
                    // WARNING: Could not recover jumptable at 0x004270ca. Too many branches
                    // WARNING: Treating indirect jump as call
  pvVar1 = LocalFree(hMem);
  return pvVar1;
}



int __stdcall
MultiByteToWideChar(UINT CodePage,DWORD dwFlags,LPCSTR lpMultiByteStr,int cbMultiByte,
                   LPWSTR lpWideCharStr,int cchWideChar)

{
  int iVar1;
  
                    // WARNING: Could not recover jumptable at 0x004270d0. Too many branches
                    // WARNING: Treating indirect jump as call
  iVar1 = MultiByteToWideChar(CodePage,dwFlags,lpMultiByteStr,cbMultiByte,lpWideCharStr,cchWideChar)
  ;
  return iVar1;
}



void __stdcall
RaiseException(DWORD dwExceptionCode,DWORD dwExceptionFlags,DWORD nNumberOfArguments,
              ULONG_PTR *lpArguments)

{
                    // WARNING: Could not recover jumptable at 0x004270d6. Too many branches
                    // WARNING: Treating indirect jump as call
  RaiseException(dwExceptionCode,dwExceptionFlags,nNumberOfArguments,lpArguments);
  return;
}



BOOL __stdcall
ReadFile(HANDLE hFile,LPVOID lpBuffer,DWORD nNumberOfBytesToRead,LPDWORD lpNumberOfBytesRead,
        LPOVERLAPPED lpOverlapped)

{
  BOOL BVar1;
  
                    // WARNING: Could not recover jumptable at 0x004270dc. Too many branches
                    // WARNING: Treating indirect jump as call
  BVar1 = ReadFile(hFile,lpBuffer,nNumberOfBytesToRead,lpNumberOfBytesRead,lpOverlapped);
  return BVar1;
}



void __stdcall
RtlUnwind(PVOID TargetFrame,PVOID TargetIp,PEXCEPTION_RECORD ExceptionRecord,PVOID ReturnValue)

{
                    // WARNING: Could not recover jumptable at 0x004270e2. Too many branches
                    // WARNING: Treating indirect jump as call
  RtlUnwind(TargetFrame,TargetIp,ExceptionRecord,ReturnValue);
  return;
}



BOOL __stdcall SetConsoleCtrlHandler(PHANDLER_ROUTINE HandlerRoutine,BOOL Add)

{
  BOOL BVar1;
  
                    // WARNING: Could not recover jumptable at 0x004270e8. Too many branches
                    // WARNING: Treating indirect jump as call
  BVar1 = SetConsoleCtrlHandler(HandlerRoutine,Add);
  return BVar1;
}



BOOL __stdcall SetEndOfFile(HANDLE hFile)

{
  BOOL BVar1;
  
                    // WARNING: Could not recover jumptable at 0x004270ee. Too many branches
                    // WARNING: Treating indirect jump as call
  BVar1 = SetEndOfFile(hFile);
  return BVar1;
}



DWORD __stdcall
SetFilePointer(HANDLE hFile,LONG lDistanceToMove,PLONG lpDistanceToMoveHigh,DWORD dwMoveMethod)

{
  DWORD DVar1;
  
                    // WARNING: Could not recover jumptable at 0x004270f4. Too many branches
                    // WARNING: Treating indirect jump as call
  DVar1 = SetFilePointer(hFile,lDistanceToMove,lpDistanceToMoveHigh,dwMoveMethod);
  return DVar1;
}



UINT __stdcall SetHandleCount(UINT uNumber)

{
  UINT UVar1;
  
                    // WARNING: Could not recover jumptable at 0x004270fa. Too many branches
                    // WARNING: Treating indirect jump as call
  UVar1 = SetHandleCount(uNumber);
  return UVar1;
}



DWORD __stdcall TlsAlloc(void)

{
  DWORD DVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427100. Too many branches
                    // WARNING: Treating indirect jump as call
  DVar1 = TlsAlloc();
  return DVar1;
}



BOOL __stdcall TlsFree(DWORD dwTlsIndex)

{
  BOOL BVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427106. Too many branches
                    // WARNING: Treating indirect jump as call
  BVar1 = TlsFree(dwTlsIndex);
  return BVar1;
}



LPVOID __stdcall TlsGetValue(DWORD dwTlsIndex)

{
  LPVOID pvVar1;
  
                    // WARNING: Could not recover jumptable at 0x0042710c. Too many branches
                    // WARNING: Treating indirect jump as call
  pvVar1 = TlsGetValue(dwTlsIndex);
  return pvVar1;
}



BOOL __stdcall TlsSetValue(DWORD dwTlsIndex,LPVOID lpTlsValue)

{
  BOOL BVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427112. Too many branches
                    // WARNING: Treating indirect jump as call
  BVar1 = TlsSetValue(dwTlsIndex,lpTlsValue);
  return BVar1;
}



LONG __stdcall UnhandledExceptionFilter(_EXCEPTION_POINTERS *ExceptionInfo)

{
  LONG LVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427118. Too many branches
                    // WARNING: Treating indirect jump as call
  LVar1 = UnhandledExceptionFilter(ExceptionInfo);
  return LVar1;
}



LPVOID __stdcall VirtualAlloc(LPVOID lpAddress,SIZE_T dwSize,DWORD flAllocationType,DWORD flProtect)

{
  LPVOID pvVar1;
  
                    // WARNING: Could not recover jumptable at 0x0042711e. Too many branches
                    // WARNING: Treating indirect jump as call
  pvVar1 = VirtualAlloc(lpAddress,dwSize,flAllocationType,flProtect);
  return pvVar1;
}



BOOL __stdcall VirtualFree(LPVOID lpAddress,SIZE_T dwSize,DWORD dwFreeType)

{
  BOOL BVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427124. Too many branches
                    // WARNING: Treating indirect jump as call
  BVar1 = VirtualFree(lpAddress,dwSize,dwFreeType);
  return BVar1;
}



SIZE_T __stdcall VirtualQuery(LPCVOID lpAddress,PMEMORY_BASIC_INFORMATION lpBuffer,SIZE_T dwLength)

{
  SIZE_T SVar1;
  
                    // WARNING: Could not recover jumptable at 0x0042712a. Too many branches
                    // WARNING: Treating indirect jump as call
  SVar1 = VirtualQuery(lpAddress,lpBuffer,dwLength);
  return SVar1;
}



DWORD __stdcall WaitForSingleObject(HANDLE hHandle,DWORD dwMilliseconds)

{
  DWORD DVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427130. Too many branches
                    // WARNING: Treating indirect jump as call
  DVar1 = WaitForSingleObject(hHandle,dwMilliseconds);
  return DVar1;
}



int __stdcall
WideCharToMultiByte(UINT CodePage,DWORD dwFlags,LPCWSTR lpWideCharStr,int cchWideChar,
                   LPSTR lpMultiByteStr,int cbMultiByte,LPCSTR lpDefaultChar,
                   LPBOOL lpUsedDefaultChar)

{
  int iVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427136. Too many branches
                    // WARNING: Treating indirect jump as call
  iVar1 = WideCharToMultiByte(CodePage,dwFlags,lpWideCharStr,cchWideChar,lpMultiByteStr,cbMultiByte,
                              lpDefaultChar,lpUsedDefaultChar);
  return iVar1;
}



BOOL __stdcall
WriteFile(HANDLE hFile,LPCVOID lpBuffer,DWORD nNumberOfBytesToWrite,LPDWORD lpNumberOfBytesWritten,
         LPOVERLAPPED lpOverlapped)

{
  BOOL BVar1;
  
                    // WARNING: Could not recover jumptable at 0x0042713c. Too many branches
                    // WARNING: Treating indirect jump as call
  BVar1 = WriteFile(hFile,lpBuffer,nNumberOfBytesToWrite,lpNumberOfBytesWritten,lpOverlapped);
  return BVar1;
}



BOOL __stdcall DestroyWindow(HWND hWnd)

{
  BOOL BVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427144. Too many branches
                    // WARNING: Treating indirect jump as call
  BVar1 = DestroyWindow(hWnd);
  return BVar1;
}



BOOL __stdcall EnumThreadWindows(DWORD dwThreadId,WNDENUMPROC lpfn,LPARAM lParam)

{
  BOOL BVar1;
  
                    // WARNING: Could not recover jumptable at 0x0042714a. Too many branches
                    // WARNING: Treating indirect jump as call
  BVar1 = EnumThreadWindows(dwThreadId,lpfn,lParam);
  return BVar1;
}



int __stdcall GetKeyboardType(int nTypeFlag)

{
  int iVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427150. Too many branches
                    // WARNING: Treating indirect jump as call
  iVar1 = GetKeyboardType(nTypeFlag);
  return iVar1;
}



int __stdcall GetSystemMetrics(int nIndex)

{
  int iVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427156. Too many branches
                    // WARNING: Treating indirect jump as call
  iVar1 = GetSystemMetrics(nIndex);
  return iVar1;
}



int __stdcall LoadStringA(HINSTANCE hInstance,UINT uID,LPSTR lpBuffer,int cchBufferMax)

{
  int iVar1;
  
                    // WARNING: Could not recover jumptable at 0x0042715c. Too many branches
                    // WARNING: Treating indirect jump as call
  iVar1 = LoadStringA(hInstance,uID,lpBuffer,cchBufferMax);
  return iVar1;
}



int __stdcall MessageBoxA(HWND hWnd,LPCSTR lpText,LPCSTR lpCaption,UINT uType)

{
  int iVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427162. Too many branches
                    // WARNING: Treating indirect jump as call
  iVar1 = MessageBoxA(hWnd,lpText,lpCaption,uType);
  return iVar1;
}



int __cdecl wsprintfA(LPSTR param_1,LPCSTR param_2,...)

{
  int iVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427168. Too many branches
                    // WARNING: Treating indirect jump as call
  iVar1 = wsprintfA(param_1,param_2);
  return iVar1;
}



HRESULT __stdcall
CoCreateInstance(IID *rclsid,LPUNKNOWN pUnkOuter,DWORD dwClsContext,IID *riid,LPVOID *ppv)

{
  HRESULT HVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427170. Too many branches
                    // WARNING: Treating indirect jump as call
  HVar1 = CoCreateInstance(rclsid,pUnkOuter,dwClsContext,riid,ppv);
  return HVar1;
}



void __stdcall CoUninitialize(void)

{
                    // WARNING: Could not recover jumptable at 0x0042717c. Too many branches
                    // WARNING: Treating indirect jump as call
  CoUninitialize();
  return;
}



HRESULT __stdcall OleInitialize(LPVOID pvReserved)

{
  HRESULT HVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427182. Too many branches
                    // WARNING: Treating indirect jump as call
  HVar1 = OleInitialize(pvReserved);
  return HVar1;
}



void __stdcall OleUninitialize(void)

{
                    // WARNING: Could not recover jumptable at 0x00427188. Too many branches
                    // WARNING: Treating indirect jump as call
  OleUninitialize();
  return;
}



HRESULT __stdcall GetActiveObject(IID *rclsid,void *pvReserved,IUnknown **ppunk)

{
  HRESULT HVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427190. Too many branches
                    // WARNING: Treating indirect jump as call
  HVar1 = GetActiveObject(rclsid,pvReserved,ppunk);
  return HVar1;
}



HRESULT __stdcall GetErrorInfo(ULONG dwReserved,IErrorInfo **pperrinfo)

{
  HRESULT HVar1;
  
                    // WARNING: Could not recover jumptable at 0x00427196. Too many branches
                    // WARNING: Treating indirect jump as call
  HVar1 = GetErrorInfo(dwReserved,pperrinfo);
  return HVar1;
}



HRESULT __stdcall SafeArrayAccessData(SAFEARRAY *psa,void **ppvData)

{
  HRESULT HVar1;
  
                    // WARNING: Could not recover jumptable at 0x0042719c. Too many branches
                    // WARNING: Treating indirect jump as call
  HVar1 = SafeArrayAccessData(psa,ppvData);
  return HVar1;
}



SAFEARRAY * __stdcall SafeArrayCreateVector(VARTYPE vt,LONG lLbound,ULONG cElements)

{
  SAFEARRAY *pSVar1;
  
                    // WARNING: Could not recover jumptable at 0x004271a2. Too many branches
                    // WARNING: Treating indirect jump as call
  pSVar1 = SafeArrayCreateVector(vt,lLbound,cElements);
  return pSVar1;
}



HRESULT __stdcall SafeArrayUnaccessData(SAFEARRAY *psa)

{
  HRESULT HVar1;
  
                    // WARNING: Could not recover jumptable at 0x004271a8. Too many branches
                    // WARNING: Treating indirect jump as call
  HVar1 = SafeArrayUnaccessData(psa);
  return HVar1;
}



BSTR __stdcall SysAllocStringLen(OLECHAR *strIn,UINT ui)

{
  BSTR pOVar1;
  
                    // WARNING: Could not recover jumptable at 0x004271ae. Too many branches
                    // WARNING: Treating indirect jump as call
  pOVar1 = SysAllocStringLen(strIn,ui);
  return pOVar1;
}



void __stdcall SysFreeString(BSTR bstrString)

{
                    // WARNING: Could not recover jumptable at 0x004271b4. Too many branches
                    // WARNING: Treating indirect jump as call
  SysFreeString(bstrString);
  return;
}



INT __stdcall SysReAllocStringLen(BSTR *pbstr,OLECHAR *psz,uint len)

{
  INT IVar1;
  
                    // WARNING: Could not recover jumptable at 0x004271ba. Too many branches
                    // WARNING: Treating indirect jump as call
  IVar1 = SysReAllocStringLen(pbstr,psz,len);
  return IVar1;
}



UINT __stdcall SysStringLen(BSTR param_1)

{
  UINT UVar1;
  
                    // WARNING: Could not recover jumptable at 0x004271c0. Too many branches
                    // WARNING: Treating indirect jump as call
  UVar1 = SysStringLen(param_1);
  return UVar1;
}



HRESULT __stdcall
VariantChangeTypeEx(VARIANTARG *pvargDest,VARIANTARG *pvarSrc,LCID lcid,USHORT wFlags,VARTYPE vt)

{
  HRESULT HVar1;
  
                    // WARNING: Could not recover jumptable at 0x004271c6. Too many branches
                    // WARNING: Treating indirect jump as call
  HVar1 = VariantChangeTypeEx(pvargDest,pvarSrc,lcid,wFlags,vt);
  return HVar1;
}



HRESULT __stdcall VariantClear(VARIANTARG *pvarg)

{
  HRESULT HVar1;
  
                    // WARNING: Could not recover jumptable at 0x004271cc. Too many branches
                    // WARNING: Treating indirect jump as call
  HVar1 = VariantClear(pvarg);
  return HVar1;
}



HRESULT __stdcall VariantCopyInd(VARIANT *pvarDest,VARIANTARG *pvargSrc)

{
  HRESULT HVar1;
  
                    // WARNING: Could not recover jumptable at 0x004271d2. Too many branches
                    // WARNING: Treating indirect jump as call
  HVar1 = VariantCopyInd(pvarDest,pvargSrc);
  return HVar1;
}



void __stdcall VariantInit(VARIANTARG *pvarg)

{
                    // WARNING: Could not recover jumptable at 0x004271d8. Too many branches
                    // WARNING: Treating indirect jump as call
  VariantInit(pvarg);
  return;
}


