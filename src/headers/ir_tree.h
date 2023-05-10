#include <string>
#include <vector>
#include <iostream>
#include <ctype.h>
typedef enum
{
    Sequence,
    Lable,
    Jump,
    Cjump,
    Move,
    Exp
} StmKind;
typedef enum
{
    plus,
    minus,
    mul,
    _div,
    rem,
    LeftShift,
    RightShift,
    LogicAnd,
    LogicOr,
    IsEqual,
    IsNe,
    IsLt,
    IsGt,
    IsLe,
    IsGe
} BinOpKind;
typedef enum
{
    INT32,
    VOID
} ValueType;
typedef enum
{
    BinOp,
    Mem,
    Temp,
    ESeq,
    Name,
    Const,
    Call,
    Allocate 
} ExpKind;
class TempIdAllocater
{
private:
    static int counter;

public:
    static int GetId() { return counter++; }
};
int TempIdAllocater::counter = 0;
class ExpIRT;
class BinOpIRT;
class TempIRT;
class MemIRT;
class ConstIRT;
class CallIRT;
class NameIRT;
class ESeqIRT;
class AllocateIRT;
class BaseIRT
{
public:
    virtual ~BaseIRT() = default;
    virtual void Dump() const = 0;
};
bool isDigit(const std::string &str){
    for(int i =0;i<str.length();++i){
        if(str[i]>'9' || str[i]<0){
            return false;
        }
    }
    return true;
}
void CheckAndConvertExpToTemp(std::string &str)
{
    if (str[0] != '%' && str[0] != '@' && !isDigit(str))
    {
        int TempId = TempIdAllocater::GetId();
        std::cout << "%" << TempId << " = " << str << "\n";
        str = "%" + std::to_string(TempId);
    }
}

class StatementIRT : public BaseIRT
{
public:
    StmKind ContentKind;
    BaseIRT *StmContent;
    StatementIRT(StmKind kind, BaseIRT *content) : ContentKind(kind), StmContent(content) {}
};
class SequenceIRT : public BaseIRT
{
public:
    BaseIRT *LeftChild, *RightChild;
    SequenceIRT(StatementIRT *LeftStm, StatementIRT *RightStm)
        : LeftChild(LeftStm), RightChild(RightStm)
    {
    }
    void Dump() const override
    {
        LeftChild->Dump();
        RightChild->Dump();
    }
};
class LableIRT : public BaseIRT
{
public:
    std::string LableName;
    LableIRT(std::string LableNameStr) { LableName = LableNameStr; }
    LableIRT():LableName(std::string("temp_lable_"+std::to_string(TempIdAllocater::GetId()))){}
    void Dump() const override
    {
        std::cout << LableName << ":\n";
    }
};
class JumpIRT : public BaseIRT
{
public:
    LableIRT *JumpLable;
    // BaseIRT* lable;
    // BaseIRT* AddressExp;
    // std::vector<BaseIRT*> LableList;
    JumpIRT(LableIRT *ArgLableIRT) : JumpLable(ArgLableIRT) {}
    void Dump() const override
    {
        std::cout << "br lable %" << JumpLable->LableName << "\n";
    }
};
class CjumpIRT : public BaseIRT
{
public:
    BinOpKind RelationOp;
    ExpIRT *LeftExp, *RightExp;
    LableIRT *LabelTrue, *LableFalse;
    CjumpIRT(BinOpKind kind, ExpIRT *left, ExpIRT *right, LableIRT *TrueLable, LableIRT *FalseLable)
        : RelationOp(kind), LeftExp(left), RightExp(right), LabelTrue(TrueLable), LableFalse(FalseLable)
    {
    }
    void Dump() const override;
};

class MoveIRT : public BaseIRT
{
public:
    enum
    {
        ToTemp,
        ToMem
    } MoveKind;
    TempIRT* DstTemp;
    MemIRT* DstMem;
    ExpIRT* SrcExp;
    MoveIRT(TempIRT *temp, ExpIRT *exp) : MoveKind(ToTemp),DstTemp(temp),DstMem(nullptr),SrcExp(exp) {}
    MoveIRT(MemIRT *mem, ExpIRT *exp) : MoveKind(ToMem), DstTemp(nullptr),DstMem(mem),SrcExp(exp) {}
    void Dump() const override;
};
class ExpIRT : public BaseIRT
{
public:

    ExpKind ContentKind;
    BaseIRT *ExpContent;
    ExpIRT(ExpKind kind, BaseIRT *content) : ContentKind(kind), ExpContent(reinterpret_cast<BaseIRT*>(content)) {}
    ExpIRT(ConstIRT* constant):ContentKind(ExpKind::Const),ExpContent(reinterpret_cast<BaseIRT*>(constant)){}
    ExpIRT(CallIRT* call):ContentKind(ExpKind::Call),ExpContent(reinterpret_cast<BaseIRT*>(call)){}
    ExpIRT(NameIRT* name):ContentKind(ExpKind::Name),ExpContent(reinterpret_cast<BaseIRT*>(name)){}
    ExpIRT(ESeqIRT* eseq):ContentKind(ExpKind::ESeq),ExpContent(reinterpret_cast<BaseIRT*>(eseq)){}
    ExpIRT(TempIRT* temp):ContentKind(ExpKind::Temp),ExpContent(reinterpret_cast<BaseIRT*>(temp)){}
    ExpIRT(MemIRT* mem):ContentKind(ExpKind::Mem),ExpContent(reinterpret_cast<BaseIRT*>(mem)){}
    ExpIRT(BinOpIRT* binop ):ContentKind(ExpKind::BinOp),ExpContent(reinterpret_cast<BaseIRT*>(binop)){} 
    ExpIRT(AllocateIRT* alloc):ContentKind(ExpKind::Allocate),ExpContent(reinterpret_cast<BaseIRT*>(alloc)) {}
    void Dump() const override
    {
    }
    std::string ExpDump() const;
};
class BinOpIRT : public BaseIRT
{
public:
    BinOpKind OpKind;
    ExpIRT *LeftExp, *RightExp;
    BinOpIRT(BinOpKind kind, ExpIRT *left, ExpIRT *right) : OpKind(kind), LeftExp(left), RightExp(right) {}
    void Dump() const override
    {
    }
    std::string ExpDump() const;
};
class MemIRT : public BaseIRT
{
public:
    ExpIRT *AddressExp;
    MemIRT(ExpIRT *addr) : AddressExp(addr) {}
    void Dump() const override
    {
    }
    std::string ExpDump() const;
};
class TempIRT : public BaseIRT
{
public:
    int TempVarId;
    TempIRT()
    {
        TempVarId = TempIdAllocater::GetId();
    }
    void Dump() const override
    {
    }
    std::string ExpDump() const;
};
class ESeqIRT : public BaseIRT
{
public:
    StatementIRT *SideEffectStm;
    ExpIRT *EstimatedExp;
    ESeqIRT(StatementIRT *stm, ExpIRT *exp) : SideEffectStm(stm), EstimatedExp(exp) {}
    void Dump() const override
    {
    }
    std::string ExpDump() const;
};
class NameIRT : public BaseIRT
{
public:
    std::string AsmLableName;
    NameIRT(std::string name) : AsmLableName(name) {}
    void Dump() const override
    {
    }
    std::string ExpDump() const;
};
class ConstIRT : public BaseIRT
{
public:
    int ConstVal;
    ConstIRT(int val):ConstVal(val){}
    void Dump() const override
    {
    }
    std::string ExpDump() const;
};
class AllocateIRT: public BaseAST{
public:
    int NumOfInt;
    int AlignSize;
    AllocateIRT(int num=1,int align=1):NumOfInt(num),AlignSize(align){}
    void Dump() const override{}
    std::string ExpDump() const;
};
class CallIRT : public BaseIRT
{
public:
    ValueType RetValType;
    LableIRT *FuncLable, *RetLable, *ExceptionLable;
    std::vector<ExpIRT *> ArgsExpList;
    CallIRT(ValueType type, LableIRT *call, LableIRT *ret, LableIRT *exclable, std::vector<ExpIRT *> args) : RetValType(type), FuncLable(call), RetLable(ret), ExceptionLable(exclable), ArgsExpList(args) {}
    void Dump() const override
    {
    }
    std::string ExpDump() const;
};

void CjumpIRT::Dump() const
{
    BinOpIRT CondExp(RelationOp, LeftExp, RightExp);
    std::string ConString = CondExp.ExpDump();
    CheckAndConvertExpToTemp(ConString);
    std::cout << "\n";
    std::cout << "br i1 " << ConString << ", lable %" << LabelTrue->LableName << ", lable %" << LableFalse->LableName << "\n";
}
void MoveIRT::Dump() const
{
    std::string SrcString = SrcExp->ExpDump();
    if (MoveKind == ToTemp)
    {
        std::cout << "%" << DstTemp->TempVarId << " = " << SrcString;
        std::cout << "\n";
    }
    else
    {
        std::string DstAddrString = DstMem->ExpDump();
        CheckAndConvertExpToTemp(SrcString);
        CheckAndConvertExpToTemp(DstAddrString);
        std::cout << "store i32 " << SrcString << ", ptr " << DstAddrString << "\n";
    }
}
std::string ExpIRT::ExpDump() const
{
    BinOpIRT *BinOpContent = reinterpret_cast<BinOpIRT *>(ExpContent);
    MemIRT *MemContent = reinterpret_cast<MemIRT *>(ExpContent);
    TempIRT *TempContent = reinterpret_cast<TempIRT *>(ExpContent);
    ESeqIRT *EseqContent = reinterpret_cast<ESeqIRT *>(ExpContent);
    NameIRT *NameContent = reinterpret_cast<NameIRT *>(ExpContent);
    ConstIRT *ConstContent = reinterpret_cast<ConstIRT *>(ExpContent);
    CallIRT *CallContent = reinterpret_cast<CallIRT *>(ExpContent);
    AllocateIRT* AllocateContent = reinterpret_cast<AllocateIRT *>(ExpContent);
    switch (ContentKind)
    {
    case ExpKind::BinOp:
        
        return BinOpContent->ExpDump();
    case ExpKind::Mem:
        
        return MemContent->ExpDump();
    case ExpKind::Temp:
        
        return TempContent->ExpDump();
    case ExpKind::ESeq:
        
        return EseqContent->ExpDump();
    case ExpKind::Name:
        
        return NameContent->ExpDump();
    case ExpKind::Const:
        
        return ConstContent->ExpDump();
    case ExpKind::Call:
        
        return CallContent->ExpDump();
    case ExpKind::Allocate:
        return AllocateContent->ExpDump();
    }
}
std::string BinOpIRT::ExpDump() const
{
    std::string ResString("");
    std::string LeftTempString = LeftExp->ExpDump(), RightTempString = RightExp->ExpDump();
    CheckAndConvertExpToTemp(LeftTempString);
    CheckAndConvertExpToTemp(RightTempString);
    switch (OpKind)
    {
    case BinOpKind::plus:
        ResString += "add i32 " + LeftTempString + ", " + RightTempString + "\n";
        break;
    case BinOpKind::minus:
        ResString += "sub i32 " + LeftTempString + ", " + RightTempString + "\n";
        break;
    case BinOpKind::mul:
        ResString += "mul i32 " + LeftTempString + ", " + RightTempString + "\n";
        break;
    case BinOpKind::_div:
        ResString += "sdiv i32 " + LeftTempString + ", " + RightTempString + "\n";
        break;
    case BinOpKind::IsEqual:
        ResString += "icmp eq i32 " + LeftTempString + ", " + RightTempString + "\n";
        break;
    case BinOpKind::IsNe:
        ResString += "icmp ne i32 " + LeftTempString + ", " + RightTempString + "\n";
        break;
    case BinOpKind::IsLt:
        ResString += "icmp slt i32 " + LeftTempString + ", " + RightTempString + "\n";
        break;
    case BinOpKind::IsLe:
        ResString += "icmp sle i32 " + LeftTempString + ", " + RightTempString + "\n";
        break;
    case BinOpKind::IsGt:
        ResString += "icmp sgt i32 " + LeftTempString + ", " + RightTempString + "\n";
        break;
    case BinOpKind::IsGe:
        ResString += "icmp sge i32 " + LeftTempString + ", " + RightTempString + "\n";
        break;
    case BinOpKind::rem:
        ResString += "srem i32 " + LeftTempString + ", " + RightTempString + "\n";
        break;
    default:
        break;
    }
    return ResString;
}

std::string MemIRT::ExpDump() const
{
    return AddressExp->ExpDump();
}
std::string TempIRT::ExpDump() const
{
    return std::to_string(TempVarId);
}
std::string ESeqIRT::ExpDump() const
{
    SideEffectStm->Dump();
    return EstimatedExp->ExpDump();
}
std::string NameIRT::ExpDump() const
{
    return AsmLableName;
}
std::string ConstIRT::ExpDump() const
{
    return std::to_string(ConstVal);
}
std::string CallIRT::ExpDump() const
{
    std::string ResString("");
    std::string RetTypeString;
    if (RetValType == ValueType::INT32)
        RetTypeString = "i32";
    else
        RetTypeString = "void";
    ResString += "invoke signext " + RetTypeString + " @" + FuncLable->LableName + "(";
    for (int i = 0; i < ArgsExpList.size(); ++i)
    {
        ResString += "i32 %" + ArgsExpList[i]->ExpDump();
        if (i != ArgsExpList.size() - 1)
        {
            ResString += ", ";
        }
    }
    ResString += ") to lable %" + RetLable->LableName + " unwind lable " + ExceptionLable->LableName + "\n";
    return ResString;
}
std::string AllocateIRT::ExpDump() const{
    std::string ResString("");
    ResString += "alloca i32 ";
    if(NumOfInt>1){
        ResString += ", i32 " + std::to_string(NumOfInt);
    }
    if(AlignSize>1){
        ResString += ", align " + std::to_string(AlignSize);
    }
    ResString += "\n";
    return ResString;
}