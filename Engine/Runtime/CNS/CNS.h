#ifndef CNS_H
#define CNS_H

    #include <stdint.h>
    #include <stddef.h>

    #define CNS_INVALID_ID UINT32_MAX

    typedef uint32_t CNSID;

    typedef enum CNSType {
        CNS_NULL = 0,
        CNS_BOOL,
        CNS_I32,
        CNS_U32,
        CNS_I64,
        CNS_U64,
        CNS_F64,
        CNS_STRING,
        CNS_BLOB,
        CNS_OBJECT,
        CNS_ARRAY
    } CNSType;

    typedef struct CNSNode {
        CNSType Type;
        uint32_t Flags;

        uint32_t NodeID;
        uint32_t KeyID;
        uint32_t KeyString;

        uint32_t Parent;
        uint32_t FirstChild;
        uint32_t LastChild;
        uint32_t PreviousSibling;
        uint32_t NextSibling;

        union {
            uint64_t u64;

            struct {
                uint32_t Offset;
                uint32_t Size;
            } Blob;
        } Value;
    } CNSNode;

    typedef struct CNSStore CNSStore;

    CNSStore *CNSCreate(void);
    void CNSDestroy(CNSStore *DataStore);
    void CNSClear(CNSStore *DataStore);

    int CNSReserve(CNSStore *DataStore, uint32_t NodeCapacity, uint32_t StringCapacity, uint32_t BlobCapacity);

    uint32_t CNSNodeCount(const CNSStore *DataStore);
    uint32_t CNSStringCount(const CNSStore *DataStore);

    const CNSNode *CNSGetNode(const CNSStore *DataStore, CNSID NodeID);

    CNSID CNSAddNode(CNSStore *DataStore, CNSID Parent, const char *KeyString, uint32_t KeyID, CNSType Type, const void *Value, uint32_t ValueSize);
    CNSID CNSAddNull (CNSStore *DataStore, CNSID Parent, const char *KeyString, uint32_t KeyID);
    CNSID CNSAddBool (CNSStore *DataStore, CNSID Parent, const char *KeyString, uint32_t KeyID, int Value);
    CNSID CNSAddI32 (CNSStore *DataStore, CNSID Parent, const char *KeyString, uint32_t KeyID, int32_t Value);
    CNSID CNSAddU32 (CNSStore *DataStore, CNSID Parent, const char *KeyString, uint32_t KeyID, uint32_t Value);
    CNSID CNSAddI64 (CNSStore *DataStore, CNSID Parent, const char *KeyString, uint32_t KeyID, int64_t Value);
    CNSID CNSAddU64 (CNSStore *DataStore, CNSID Parent, const char *KeyString, uint32_t KeyID, uint64_t Value);
    CNSID CNSAddF64 (CNSStore *DataStore, CNSID Parent, const char *KeyString, uint32_t KeyID, double Value);
    CNSID CNSAddString (CNSStore *DataStore, CNSID Parent, const char *KeyString, uint32_t KeyID, const char *Value);
    CNSID CNSAddBlob (CNSStore *DataStore, CNSID Parent, const char *KeyString, uint32_t KeyID, const void *Data, uint32_t Size);
    CNSID CNSAddObject (CNSStore *DataStore, CNSID Parent, const char *KeyString, uint32_t KeyID);
    CNSID CNSAddArray (CNSStore *DataStore, CNSID Parent, const char *KeyString, uint32_t KeyID);

    CNSID CNSFindChildByName(const CNSStore *DataStore, CNSID Parent, const char *KeyString);
    CNSID CNSFindChildByKeyID(const CNSStore *DataStore, CNSID Parent, uint32_t KeyID);
    CNSID CNSFindChildByIndex(const CNSStore *DataStore, CNSID Parent, uint32_t index);

    CNSID CNSParent(const CNSStore *DataStore, CNSID NodeID);
    uint32_t CNSChildCount(const CNSStore *DataStore, CNSID Parent);

    CNSType CNSGetType(const CNSStore *DataStore, CNSID NodeID);

    int         CNSAsBool  (const CNSStore *DataStore, CNSID NodeID, int Fallback);
    int32_t     CNSAsI32   (const CNSStore *DataStore, CNSID NodeID, int32_t Fallback);
    uint32_t    CNSAsU32   (const CNSStore *DataStore, CNSID NodeID, uint32_t Fallback);
    int64_t     CNSAsI64   (const CNSStore *DataStore, CNSID NodeID, int64_t Fallback);
    uint64_t    CNSAsU64   (const CNSStore *DataStore, CNSID NodeID, uint64_t Fallback);
    double      CNSAsF64   (const CNSStore *DataStore, CNSID NodeID, double Fallback);
    const char *CNSAsString (const CNSStore *DataStore, CNSID NodeID);
    const void *CNSAsBlob   (const CNSStore *DataStore, CNSID NodeID, uint32_t *SizeOut);

    int CNSSetNull (CNSStore *DataStore, CNSID NodeID);
    int CNSSetBool (CNSStore *DataStore, CNSID NodeID, int Value);
    int CNSSetI32 (CNSStore *DataStore, CNSID NodeID, int32_t Value);
    int CNSSetU32 (CNSStore *DataStore, CNSID NodeID, uint32_t Value);
    int CNSSetI64 (CNSStore *DataStore, CNSID NodeID, int64_t Value);
    int CNSSetU64 (CNSStore *DataStore, CNSID NodeID, uint64_t Value);
    int CNSSetF64 (CNSStore *DataStore, CNSID NodeID, double Value);
    int CNSSetString (CNSStore *DataStore, CNSID NodeID, const char *Value);
    int CNSSetBlob (CNSStore *DataStore, CNSID NodeID, const void *Data, uint32_t Size);
    int CNSSetObject (CNSStore *DataStore, CNSID NodeID);
    int CNSSetArray (CNSStore *DataStore, CNSID NodeID);

    int CNSRename(CNSStore *DataStore, CNSID NodeID, const char *KeyString, uint32_t KeyID);
    int CNSRemove(CNSStore *DataStore, CNSID NodeID);

    int CNSSaveBinary(const CNSStore *DataStore, const char *Path);
    CNSStore *CNSLoadBinary(const char *Path);

#endif