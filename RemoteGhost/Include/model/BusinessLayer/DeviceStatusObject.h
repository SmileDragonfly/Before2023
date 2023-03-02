#pragma once

#include "DataObject.h"
#include <vector>

using namespace std;

//////////////////////////////////////////////////////////////////////////
// Device Status Object
//////////////////////////////////////////////////////////////////////////
class DeviceStatusObj
{
public:

	template<class T> void orm(T& t)
	{
		t & ORM_COL(EventClass); 
		t & ORM_COL(EventID);
		t & ORM_COL(Description);
		t & ORM_COL(DeviceType);
		t & ORM_COL(EventParam);
	}

	property<std::string> EventClass;
	property<std::string> EventID;
	property<std::string> Description;
	property<std::string> DeviceType;
	property<std::string> EventParam;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "DeviceStatusObj";
	}	

	DeviceStatusObj()
	{
		std::string sTemp("");
		EventClass=EventID=Description=DeviceType=EventParam=sTemp;
	} 

	CStringA GenerateData()
	{
		CStringA strData("");

		if (CStringA(EventParam.get().c_str()).CompareNoCase("") == 0)
		{
			EventParam = std::string("{}");
		}

		strData.Format("{\"EventClass\":\"%s\",\"EventID\":\"%s\",\"Description\":\"%s\",\"DeviceType\":\"%s\",\"EventParam\":%s}",
			CStringA(EventClass.get().c_str()), 
			CStringA(EventID.get().c_str()),
			CStringA(Description.get().c_str()),
			CStringA(DeviceType.get().c_str()),
			CStringA(EventParam.get().c_str()));
		return strData;
	}

};


//////////////////////////////////////////////////////////////////////////
// HWError Object
//////////////////////////////////////////////////////////////////////////
struct HWError
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(LogicalName); 
		t & ORM_COL(PhysicalName);
		t & ORM_COL(WorkstationName);
		t & ORM_COL(AppID);
		t & ORM_COL(Action);
		t & ORM_COL(Description);
	}

	property<std::string> LogicalName;
	property<std::string> PhysicalName;
	property<std::string> WorkstationName;
	property<std::string> AppID;
	property<std::string> Action;
	property<std::string> Description;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "HWError";
	}

	HWError()
	{
		std::string sTemp("");
		LogicalName=PhysicalName=WorkstationName=AppID=Action=Description=sTemp;
	} 
};


//////////////////////////////////////////////////////////////////////////
// Dev status Object
//////////////////////////////////////////////////////////////////////////
struct DevStatus
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(PhysicalName);
		t & ORM_COL(WorkstationName);
		t & ORM_COL(State);
	}

	property<std::string> PhysicalName;
	property<std::string> WorkstationName;
	property<std::string> State;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "DevStatus";
	}

	DevStatus()
	{
		std::string sTemp("");
		PhysicalName=WorkstationName=State=sTemp;
	} 
};



/************************************************************************/
/* Cash Dispenser Device                                                */
/************************************************************************/
struct CDMPhysicalCU
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(PhysicalPositionName);
		t & ORM_COL(UnitID);
// 		t & ORM_COL(ulInitialCount);
// 		t & ORM_COL(ulCount);
// 		t & ORM_COL(ulRejectCount);
// 		t & ORM_COL(ulMaximum);
		t & ORM_COL(Status);
		t & ORM_COL(bHardwareSensor);
	}

	property<std::string> PhysicalPositionName;
	property<std::string> UnitID;
// 	property<long>		  ulInitialCount;
// 	property<long>		  ulCount;
// 	property<long>		  ulRejectCount;
// 	property<long>		  ulMaximum;
	property<std::string> Status;
	property<BOOL>		  bHardwareSensor;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CDMPhysicalCU";
	}

	CDMPhysicalCU()
	{
		std::string sTemp("");
		PhysicalPositionName=UnitID=Status=sTemp;

		BOOL bTemp=FALSE;
		bHardwareSensor = bTemp;
	} 
};

struct CDMCashUnit
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(Type);
		t & ORM_COL(Number);
		t & ORM_COL(Values);
		t & ORM_COL(InitialCount);
		t & ORM_COL(Count);
		t & ORM_COL(RejectCount);
		t & ORM_COL(Status);
		t & ORM_COL(lsPhysicalCU);
	}

	property<std::string> Type;
	property<int> Number;
	property<int> Values;
	property<int> InitialCount;
	property<int> Count;
	property<int> RejectCount;
	property<std::string> Status;
	property<std::vector<CDMPhysicalCU> > lsPhysicalCU;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CDMCashUnit";
	}

	CDMCashUnit()
	{
		std::string sTemp="";
		Type = Status = sTemp;

		int iTemp=0;
		Number = Values = InitialCount = Count = RejectCount = iTemp;
		
		std::vector<CDMPhysicalCU> vTemp;
		vTemp.clear();
		lsPhysicalCU = vTemp;
	}
};	

/************************************************************************/
/* Card Device                                                          */
/************************************************************************/
struct IDCRetainBin
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(RetainBin);
	}

	property<std::string> RetainBin;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "IDCRetainBin";
	}

	IDCRetainBin()
	{
		std::string sTemp="";
		RetainBin = sTemp;
	}
};

struct IDCCardAction
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(Action);
		t & ORM_COL(Position);
	}

	property<std::string> Action;
	property<std::string> Position;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "IDCCardAction";
	}

	IDCCardAction()
	{
		std::string sTemp="";
		Action = Position = sTemp;
	}
};

struct IDCMediaDetected
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(ResetOut);
	}

	property<std::string> ResetOut;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "IDCMediaDetected";
	}

	IDCMediaDetected()
	{
		std::string sTemp="";
		ResetOut = sTemp;
	}
};

struct IDCTrackEvent
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(Status);
		t & ORM_COL(Track);
		t & ORM_COL(Data);
	}

	property<std::string> Status;
	property<std::string> Track;
	property<std::string> Data;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "IDCTrackEvent";
	}

	IDCTrackEvent()
	{
		std::string sTemp="";
		Status = Track = Data = sTemp;
	}
};


/************************************************************************/
/* Printer Device                                                       */
/************************************************************************/
struct PTRMediaDetected
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(Position);
		t & ORM_COL(RetractBinNumber);
	}

	property<std::string> Position;
	property<int> RetractBinNumber;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "PTRMediaDetected";
	}

	PTRMediaDetected()
	{
		std::string sTemp="";
		Position = sTemp;

		int iTemp = 0;
		RetractBinNumber = iTemp;
	}
};	

struct PTRBinThreshold
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(RetractBin);
		t & ORM_COL(BinNumber);
	}

	property<std::string> RetractBin;
	property<int> BinNumber;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "PTRBinThreshold";
	}

	PTRBinThreshold()
	{
		std::string sTemp="";
		RetractBin = sTemp;

		int iTemp = 0;
		BinNumber = iTemp;
	}
};

struct PTRPaperThreshold
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(PaperSource);
		t & ORM_COL(PaperThreshold);
	}

	property<std::string> PaperSource;
	property<std::string> PaperThreshold;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "PTRPaperThreshold";
	}

	PTRPaperThreshold()
	{
		std::string sTemp="";
		PaperSource = PaperThreshold = sTemp;
	}
};

struct PTRTonerThreshold
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(TonerThreshold);
	}

	property<std::string> TonerThreshold;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "PTRTonerThreshold";
	}

	PTRTonerThreshold()
	{
		std::string sTemp="";
		TonerThreshold = sTemp;
	}
};

struct PTRLampThreshold
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(LampThreshold);
	}

	property<std::string> LampThreshold;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "PTRLampThreshold";
	}

	PTRLampThreshold()
	{
		std::string sTemp="";
		LampThreshold = sTemp;
	}
};

struct PTRInkThreshold
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(InkThreshold);
	}

	property<std::string> InkThreshold;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "PTRInkThreshold";
	}

	PTRInkThreshold()
	{
		std::string sTemp="";
		InkThreshold = sTemp;
	}
};

struct PTRNoMedia
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(UserPrompt);
	}

	property<std::string> UserPrompt;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "PTRNoMedia";
	}

	PTRNoMedia()
	{
		std::string sTemp="";
		UserPrompt = sTemp;
	}
};

struct PTRFieldError
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(FormName);
		t & ORM_COL(FieldName);
		t & ORM_COL(Failure);
	}

	property<std::string> FormName;
	property<std::string> FieldName;
	property<std::string> Failure;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "PTRFieldError";
	}

	PTRFieldError()
	{
		std::string sTemp="";
		FormName = FieldName = Failure = sTemp;
	}
};



/************************************************************************/
/* Cash-In module device                                                */
/************************************************************************/
struct CIMNoteNumber
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(NoteID);
		t & ORM_COL(Count);
	}

	property<int> NoteID;
	property<DWORD> Count;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CIMNoteNumber";
	}

	CIMNoteNumber()
	{
		int iTemp=0;
		NoteID = iTemp;

		DWORD dwTemp=0;
		Count = dwTemp;
	}
};

struct CIMPhysicalcu
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(PhysicalPositionName);
		t & ORM_COL(UnitID);
		t & ORM_COL(CashInCount);
		t & ORM_COL(Count);
		t & ORM_COL(Maximum);
		t & ORM_COL(UnitID);
		t & ORM_COL(bHardwareSensors);
		t & ORM_COL(Extra);
	}

	property<std::string> PhysicalPositionName;
	property<std::string> UnitID;
	property<DWORD>		  CashInCount;
	property<DWORD>       Count;
	property<DWORD>		  Maximum;
	property<std::string> PStatus;
	property<BOOL>		  bHardwareSensors;
	property<std::string> Extra;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CIMPhysicalcu";
	}

	CIMPhysicalcu()
	{
		std::string sTemp = "";
		PhysicalPositionName = UnitID = PStatus = Extra = sTemp;

		DWORD dwTemp=0;
		CashInCount = Count = Maximum = dwTemp;

		BOOL bTemp = FALSE;
		bHardwareSensors = bTemp;
	}
};

struct CIMCashIn
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(Number);
		t & ORM_COL(Type);
		t & ORM_COL(ItemType);
		t & ORM_COL(UnitID);
		t & ORM_COL(CurrencyID);
		t & ORM_COL(Values);
		t & ORM_COL(CashInCount);
		t & ORM_COL(Count);
		t & ORM_COL(Maximum);
		t & ORM_COL(Status);
		t & ORM_COL(AppLock);
		t & ORM_COL(lsNoteNumberList);
		t & ORM_COL(lsPhysical);
		t & ORM_COL(Extra);
	}

	property<int>		  Number;
	property<std::string> Type;
	property<std::string> ItemType;
	property<std::string> UnitID;
	property<std::string> CurrencyID;
	property<DWORD>		  Values;
	property<DWORD>		  CashInCount;
	property<DWORD>		  Count;
	property<DWORD>		  Maximum;
	property<std::string> Status;
	property<BOOL>		  AppLock;
	property<std::vector<CIMNoteNumber> > lsNoteNumberList;
	property<std::vector<CIMPhysicalcu> > lsPhysical;
	property<std::string> Extra;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CIMCashIn";
	}

	CIMCashIn()
	{
		std::string sTemp = "";
		Type = ItemType = UnitID = CurrencyID = Status = Extra = sTemp;

		int iTemp=0;
		Number = iTemp;

		DWORD dwTemp=0;
		Values = CashInCount = Count = Maximum = dwTemp;

		BOOL bTemp = FALSE;
		AppLock = bTemp;
	}
};
// 
// class CIMCashIn
// {
// public:
// 	USHORT						usNumber;
// 	DWORD						fwType;
// 	DWORD						fwItemType;
// 	std::string					sUnitID;
// 	std::string					sCurrencyID;
// 	ULONG						ulValues;
// 	ULONG						ulCashInCount;
// 	ULONG						ulCount;
// 	ULONG						ulMaximum;
// 	USHORT						usStatus;
// 	BOOL						bAppLock;
// 	std::vector<CIMNoteNumber>	lsNoteNumberList;
// 	std::vector<CIMPhysicalcu>	lsPhysical;
// 	std::string					lpszExtra;
// 	//LPUSHORT lpusNoteIDs;
// 	// 	WORD						usCDMType;
// 	// 	std::string					lpszCashUnitName;
// 	// 	long						ulInitialCount;
// 	// 	long						ulDispensedCount;
// 	// 	long						ulPresentedCount;
// 	// 	long						ulRetractedCount;
// 	// 	long						ulRejectCount;
// 	// 	long						ulMinimum;
// 
// 	Json to_json() const {
// 		std::string sType = CUtil::CIMCashInTypeToString(fwType);
// 		std::string sItemType = CUtil::CIMCashInItemTypeToString(fwType);
// 		std::string sStatus = CUtil::CIMCashInStatusToString(usStatus);
// 		return Json::object{
// 			{ "Number", usNumber },
// 			{ "Type", sType },
// 			{ "ItemType", sItemType },
// 			{ "UnitID", sUnitID },
// 			{ "CurrencyID", sCurrencyID },
// 			{ "Values", ulValues },
// 			{ "CashInCount", ulCashInCount },
// 			{ "Count", ulCount },
// 			{ "Maximum", ulMaximum },
// 			{ "Status", sStatus },
// 			{ "AppLock", bAppLock },
// 			{ "lsNoteNumberList", lsNoteNumberList },
// 			{ "lsPhysical", lsPhysical },
// 			{ "Extra", lpszExtra },
// 			// 			{ "CDMType", usCDMType },
// 			// 			{ "CashUnitName", lpszCashUnitName },
// 			// 			{ "InitialCount", ulInitialCount },
// 			// 			{ "DispensedCount", ulDispensedCount },
// 			// 			{ "PresentedCount", ulPresentedCount },
// 			// 			{ "RetractedCount", ulRetractedCount },
// 			// 			{ "RejectCount", ulRejectCount },
// 			// 			{ "Minimum", ulMinimum }
// 		};
// 	}
// };
// 
struct CUNumList
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(Number);
	}

	property<int> Number;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CUNumList";
	}

	CUNumList()
	{
		int iTemp=0;
		Number = iTemp;
	}
};
struct CIMCountsChanged
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(Count);
		t & ORM_COL(lsCUNumList);
	}

	property<int> Count;
	property<std::vector<CUNumList> > lsCUNumList;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CIMCountsChanged";
	}

	CIMCountsChanged()
	{
		int iTemp=0;
		Count = iTemp;
	}
};

struct CIMRetract
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(OutputPosition);
		t & ORM_COL(RetractArea);
		t & ORM_COL(Index);
	}

	property<int> OutputPosition;
	property<int> RetractArea;
	property<int> Index;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CIMRetract";
	}

	CIMRetract()
	{
		int iTemp=0;
		OutputPosition = RetractArea = Index = iTemp;
	}
};

struct CIMItemposition
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(Number);
		t & ORM_COL(RetractArea);
		t & ORM_COL(OutputPosition);
	}

	property<int> Number;
	property<CIMRetract> RetractArea;
	property<int> OutputPosition;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CIMItemposition";
	}

	CIMItemposition()
	{
		int iTemp=0;
		Number = OutputPosition = iTemp;
	}
};

struct CIMCashUnitError
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(Failure);
		t & ORM_COL(CashUnit);
	}

	property<std::string> Failure;
	property<CIMCashIn> CashUnit;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CIMCashUnitError";
	}

	CIMCashUnitError()
	{
		std::string sTemp="";
		Failure = sTemp;
	}
};

struct CIMP6Info
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(Level);
		t & ORM_COL(lpNoteNumberList);
		t & ORM_COL(NumOfSignatures);
	}

	property<int> Level;
	property<std::vector<CIMNoteNumber> > lpNoteNumberList;
	property<int> NumOfSignatures;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CIMP6Info";
	}

	CIMP6Info()
	{
		int iTemp=0;
		Level = NumOfSignatures = iTemp;
	}
};

/************************************************************************/
/* BARCODE module device                                                */
/************************************************************************/


struct BCRDevicePosition
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(Position);
		
	}

	property<std::string> Position;


	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "BCRDevicePosition";
	}

	BCRDevicePosition()
	{
		Position = "";
		
	}
};

struct BCRPowerSaveChange
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(PowerSaveRecoveryTime);

	}

	property<int> PowerSaveRecoveryTime;


	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "BCRPowerSaveChange";
	}

	BCRPowerSaveChange()
	{
		int iTemp = 0;
		PowerSaveRecoveryTime = iTemp;

	}
};


/************************************************************************/
/* CAMERADEVICE module device                                           */
/************************************************************************/

struct CAMMediaThreshold
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(MediaThreshold);
	}

	property<std::string> MediaThreshold;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CAMMediaThreshold";
	}

	CAMMediaThreshold()
	{
		std::string sTemp = "";
		MediaThreshold = sTemp;
	}
};


/************************************************************************/
/* CARDEMBOSSING module device                                          */
/************************************************************************/


struct CEUOutputBin
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(OutputBin);
	}

	property<std::string> OutputBin;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CEUOutputBin";
	}

	CEUOutputBin()
	{
		std::string sTemp = "";
		OutputBin = sTemp;
	}
};


struct CEURetainBin
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(RetainBin);
	}

	property<std::string> RetainBin;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CEURetainBin";
	}

	CEURetainBin()
	{
		std::string sTemp = "";
		RetainBin = sTemp;
	}
};


struct CEUPosition
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(Position);
	}

	property<std::string> Position;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CEUPosition";
	}

	CEUPosition()
	{
		std::string sTemp = "";
		Position = sTemp;
	}
};


struct CEUDevicePosition
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(Position);
	}

	property<std::string> Position;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CEUDevicePosition";
	}

	CEUDevicePosition()
	{
		std::string sTemp = "";
		Position = sTemp;
	}
};


struct CEUPowerSaveChange
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(PowerSaveRecoveryTime);
	}

	property<int> PowerSaveRecoveryTime;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CEUPowerSaveChange";
	}

	CEUPowerSaveChange()
	{
		int sTemp = 0;
		PowerSaveRecoveryTime = sTemp;
	}
};



struct CEUFieldFail
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(FormName);       //LPSTR
		t & ORM_COL(FieldName);		 //LPSTR
		t & ORM_COL(Failure);			 //WORD
	}

	property<std::string> FormName;
	property<std::string> FieldName;
	property<std::string> Failure;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CEUFieldFail";
	}

	CEUFieldFail()
	{
		std::string sTemp = "";
		FormName = sTemp;
		FieldName = sTemp;
		Failure = sTemp;

	}
};


/************************************************************************/
/* CARD_IN_MODULE module device                                          */
/************************************************************************/

struct CIMCashUnitInfoChanged
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(CashUnit);  
	}

	property<std::string> CashUnit;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CIMCashUnitInfoChanged";
	}

	CIMCashUnitInfoChanged()
	{
		std::string sTemp = "";
		CashUnit = sTemp;

	}
};



struct CIMTellerInfoChanged
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(TellerID);
	}

	property<int> TellerID;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CIMTELLERINFOCHANGED";
	}

	CIMTellerInfoChanged()
	{
		int sTemp = 0;
		TellerID = sTemp;

	}
};


struct CIMItemsTaken
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(Position);
		t & ORM_COL(AdditionalBunches);
		t & ORM_COL(BunchesRemaining);
	}

	property<std::string> Position;
	property<std::string> AdditionalBunches;
	property<int> BunchesRemaining;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CIMITEMSTAKEN";
	}

	CIMItemsTaken()
	{
		std::string sTemp = "";
		Position = sTemp;
		AdditionalBunches = sTemp;

		int iTemp = 0;
		BunchesRemaining = iTemp;
	}
};



struct CIMItemsPresented
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(Position);
		t & ORM_COL(AdditionalBunches);
		t & ORM_COL(BunchesRemaining);
	}

	property<std::string> Position;
	property<std::string> AdditionalBunches;
	property<int> BunchesRemaining;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CIMITEMSPRESENTED";
	}

	CIMItemsPresented()
	{
		std::string sTemp = "";
		Position = sTemp;
		AdditionalBunches = sTemp;

		int iTemp = 0;
		BunchesRemaining = iTemp;
	}
};



/************************************************************************/
/* CHK-DEVICE module device                                             */
/************************************************************************/


struct CHKMediaDetected
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(ResetOut);
	}

	property<std::string> ResetOut;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CHKMEDIADETECTED";
	}

	CHKMediaDetected()
	{
		std::string sTemp = "";
		ResetOut = sTemp;
	}
};


struct CHKDevicePosition
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(Position);
	}

	property<std::string> Position;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CHKDEVICEPOSITION";
	}

	CHKDevicePosition()
	{
		std::string sTemp = "";
		Position = sTemp;
	}
};



struct CHKPowerSaveChange
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(PowerSaveRecoveryTime);
	}

	property<int> PowerSaveRecoveryTime;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CHKPOWERSAVECHANGE";
	}

	CHKPowerSaveChange()
	{
		int sTemp = 0;
		PowerSaveRecoveryTime = sTemp;
	}
};


struct CHKInkThreshold
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(InkThreshold);
	}

	property<std::string> InkThreshold;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CHKINKTHRESHOLD";
	}

	CHKInkThreshold()
	{
		std::string sTemp = "";
		InkThreshold = sTemp;
	}
};



struct CHKNoMedia
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(UserPrompt);
	}

	property<std::string> UserPrompt;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CHKNOMEDIA";
	}

	CHKNoMedia()
	{
		std::string sTemp = "";
		UserPrompt = sTemp;
	}
};


struct CHKFieldError
{
	template<class T> void orm(T& t)
	{
		//			LPSTR lpszFormName;
		// 			LPSTR lpszFieldName;
		// 			WORD wFailure;
		t & ORM_COL(FormName);
		t & ORM_COL(FieldName);
		t & ORM_COL(Failure);
	}

	property<std::string> FormName;
	property<std::string> FieldName;
	property<std::string> Failure;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CHKFIELDERROR";
	}

	CHKFieldError()
	{
		std::string sTemp = "";
		FormName = sTemp;
		FieldName = sTemp;
		Failure = sTemp;
	}
};


/************************************************************************/
/* DEPOSITORY-DEVICE device                                             */
/************************************************************************/


struct DEPMediaDetected
{
	template<class T> void orm(T& t)
	{

		t & ORM_COL(DispenseMedia);
		t & ORM_COL(DepositMedia);

	}

	property<std::string> DispenseMedia;
	property<std::string> DepositMedia;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "DEPMediaDetected";
	}

	DEPMediaDetected()
	{
		std::string sTemp = "";
		DispenseMedia = sTemp;
		DepositMedia = sTemp;
	}
};



struct DEPDevicePosition
{
	template<class T> void orm(T& t)
	{

		t & ORM_COL(Position);

	}

	property<std::string> Position;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "DEPDEVICEPOSITION";
	}

	DEPDevicePosition()
	{
		std::string sTemp = "";
		Position = sTemp;
	}
};



struct DEPPowerSaveChange
{
	template<class T> void orm(T& t)
	{

		t & ORM_COL(PowerSaveRecoveryTime);

	}

	property<int> PowerSaveRecoveryTime;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "DEPPOWERSAVECHANGE";
	}

	DEPPowerSaveChange()
	{
		int iTemp = 0;
		PowerSaveRecoveryTime = iTemp;
	}
};



struct DEPDepThreshold
{
	template<class T> void orm(T& t)
	{

		t & ORM_COL(DepositThreshold);

	}

	property<std::string> DepositThreshold;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "DEPDEPTHRESHOLD";
	}

	DEPDepThreshold()
	{
		std::string sTemp = "";
		DepositThreshold = sTemp;
	}
};



struct DEPTonerThreshold
{
	template<class T> void orm(T& t)
	{

		t & ORM_COL(TonerThreshold);

	}

	property<std::string> TonerThreshold;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "DEPTONERTHRESHOLD";
	}

	DEPTonerThreshold()
	{
		std::string sTemp = "";
		TonerThreshold = sTemp;
	}
};



struct DEPEnvThreshold
{
	template<class T> void orm(T& t)
	{

		t & ORM_COL(EnvelopeThreshold);

	}

	property<std::string> EnvelopeThreshold;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "DEPENVTHRESHOLD";
	}

	DEPEnvThreshold()
	{
		std::string sTemp = "";
		EnvelopeThreshold = sTemp;
	}
};


struct DEPDepositError
{
	template<class T> void orm(T& t)
	{

		t & ORM_COL(Error);

	}

	property<std::string> Error;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "DEPDEPOSITERROR";
	}

	DEPDepositError()
	{
		std::string sTemp = "";
		Error = sTemp;
	}
};



/************************************************************************/
/* ITEM-PROCESSING device                                               */
/************************************************************************/

struct IMPMediaBin
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(BinNumber);
		t & ORM_COL(PositionName);
		t & ORM_COL(Type);
		t & ORM_COL(MediaType);
		t & ORM_COL(BinID);
		t & ORM_COL(MediaInCount);
		t & ORM_COL(Count);
		t & ORM_COL(RetractOperations);
		t & ORM_COL(HardwareSensors);
		t & ORM_COL(MaximumItems);
		t & ORM_COL(MaximumRetractOperations);
		t & ORM_COL(Status);
		t & ORM_COL(Extra);
	}

	property<int> BinNumber;
	property<std::string> PositionName;
	property<std::string> Type;
	property<std::string> MediaType;
	property<std::string> BinID;
	property<int> MediaInCount;
	property<int> Count;
	property<int> RetractOperations;
	property<int> HardwareSensors;
	property<int> MaximumItems;
	property<int> MaximumRetractOperations;
	property<std::string> Status;
	property<std::string> Extra;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "IPMMEDIABIN";
	}

	IMPMediaBin()
	{
		std::string sTemp = "";
		PositionName = Type = MediaType = BinID = Status = Extra = sTemp;
		int nTemp = 0;
		BinNumber = MediaInCount = Count = RetractOperations = HardwareSensors = MaximumItems = MaximumRetractOperations = nTemp;
	}
};



struct IPMMediaTaken
{
	template<class T> void orm(T& t)
	{

		t & ORM_COL(Position);

	}

	property<std::string> Position;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "IPMMEDIATAKEN";
	}

	IPMMediaTaken()
	{
		std::string sTemp = "";
		Position = sTemp;
	}
};


struct IPMMediaDetected
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(Position);
		t & ORM_COL(RetractBinNumber);

	}

	property<std::string> Position;
	property<int> RetractBinNumber;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "IPMMediaDetected";
	}

	IPMMediaDetected()
	{
		std::string sTemp = "";
		Position = sTemp;

		int iTemp = 0;
		RetractBinNumber = iTemp;
	}
};



struct IPMDevicePosition
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(Position);

	}

	property<std::string> Position;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "IPMDEVICEPOSITION";
	}

	IPMDevicePosition()
	{
		std::string sTemp = "";
		Position = sTemp;
	}
};



struct IPMPowerSaveChange
{
	template<class T> void orm(T& t)
	{

		t & ORM_COL(PowerSaveRecoveryTime);

	}

	property<int> PowerSaveRecoveryTime;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "IPMPOWERSAVECHANGE";
	}

	IPMPowerSaveChange()
	{
		int iTemp = 0;
		PowerSaveRecoveryTime = iTemp;
	}
};



struct IPMShutterStatusChanged
{
	template<class T> void orm(T& t)
	{

		t & ORM_COL(Position);
		t & ORM_COL(Shutter);

	}

	property<std::string> Position;
	property<std::string> Shutter;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "IPMSHUTTERSTATUSCHANGED";
	}

	IPMShutterStatusChanged()
	{
		std::string sTemp = "";
		Position = sTemp;
		Shutter = sTemp;
	}
};


struct IPMTonerThreshold
{
	template<class T> void orm(T& t)
	{

		t & ORM_COL(Threshold);

	}

	property<std::string> Threshold;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "IPMTONERTHRESHOLD";
	}

	IPMTonerThreshold()
	{
		std::string sTemp = "";
		Threshold = sTemp;
	}
};



struct IPMScannerThreshold
{
	template<class T> void orm(T& t)
	{

		t & ORM_COL(Scanner);
		t & ORM_COL(Threshold);

	}

	property<std::string> Scanner;
	property<std::string> Threshold;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "IPMSCANNERTHRESHOLD";
	}

	IPMScannerThreshold()
	{
		std::string sTemp = "";
		Scanner = sTemp;
		Threshold = sTemp;
	}
};



struct IPMINKThreshold
{
	template<class T> void orm(T& t)
	{

		t & ORM_COL(Threshold);

	}

	property<std::string> Threshold;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "IPMINKTHRESHOLD";
	}

	IPMINKThreshold()
	{
		std::string sTemp = "";
		Threshold = sTemp;
	}
};


struct IPMMicrThreshold
{
	template<class T> void orm(T& t)
	{

		t & ORM_COL(Threshold);

	}

	property<std::string> Threshold;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "IPMMICRTHRESHOLD";
	}

	IPMMicrThreshold()
	{
		std::string sTemp = "";
		Threshold = sTemp;
	}
};



struct IPMMediaBinerror
{
	template<class T> void orm(T& t)
	{

		t & ORM_COL(Failure);

	}

	property<std::string> Failure;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "IPMMEDIABINERROR";
	}

	IPMMediaBinerror()
	{
		std::string sTemp = "";
		Failure = sTemp;
	}
};


struct IPMMediaPresented
{
	template<class T> void orm(T& t)
	{

		t & ORM_COL(Position);
		t & ORM_COL(BunchIndex);
		t & ORM_COL(TotalBunches);

	}

	property<std::string> Position;
	property<int> BunchIndex;
	property<int> TotalBunches;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "IPMMEDIAPRESENTED";
	}

	IPMMediaPresented()
	{
		std::string sTemp = "";
		Position = sTemp;

		BunchIndex = 0;
		TotalBunches = 0;

	}
};


struct IPMMediasize
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(SizeX);
		t & ORM_COL(SizeY);
	}

	property<int> SizeX;
	property<int> SizeY;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "IPMMEDIASIZE";
	}

	IPMMediasize()
	{
		int iTemp = 0;
		SizeX = iTemp;
		SizeY = iTemp;
	}
};


struct IPMMediaRefused
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(Reason);
		t & ORM_COL(MediaLocation);
		t & ORM_COL(PresentRequired);
		t & ORM_COL(MediaSize);
	}

	property<std::string> Reason;
	property<std::string> MediaLocation;
	property<BOOL> PresentRequired;
	property<IPMMediasize> MediaSize;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "IPMMEDIAREFUSED";
	}

	IPMMediaRefused()
	{
		std::string sTemp = "";
		Reason = sTemp;

		MediaLocation = sTemp;
		PresentRequired = false;

	}
};



struct IPMMediaRejected
{
	template<class T> void orm(T& t)
	{

		t & ORM_COL(Reason);

	}

	property<std::string> Reason;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "IPMMEDIAREJECTED";
	}

	IPMMediaRejected()
	{
		std::string sTemp = "";
		Reason = sTemp;
	}
};


struct IPMImageData
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(ImageSource);
		t & ORM_COL(ImageType);
		t & ORM_COL(ImageColorFormat);
		t & ORM_COL(ImageScanColor);
		t & ORM_COL(ImageStatus);
		t & ORM_COL(ImageFile);

	}

	property<std::string> ImageSource;
	property<std::string> ImageType;
	property<std::string> ImageColorFormat;
	property<std::string> ImageScanColor;
	property<std::string> ImageStatus;
	property<std::string> ImageFile;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "IPMIMAGEDATA";
	}

	IPMImageData()
	{
		std::string sTemp = "";
		ImageSource = sTemp;
		ImageType = sTemp;
		ImageColorFormat = sTemp;
		ImageScanColor = sTemp;
		ImageStatus = sTemp;
		ImageFile = sTemp;
	}
};


struct IPMMediaData
{
	template<class T> void orm(T& t)
	{

		t & ORM_COL(MediaID);
		t & ORM_COL(CodelineDataLength);
		t & ORM_COL(MagneticReadIndicator);
		t & ORM_COL(Image);
		t & ORM_COL(InsertOrientation);
		t & ORM_COL(MediaSize);
		t & ORM_COL(MediaValidity);

	}

	property<int> MediaID;
	property<int> CodelineDataLength;
	property<std::string> MagneticReadIndicator;
	property<std::vector<IPMImageData> > Image;
	property<std::string> InsertOrientation;
	property<IPMMediasize> MediaSize;
	property<std::string> MediaValidity;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "IPMMEDIADATA";
	}

	IPMMediaData()
	{
		std::string sTemp = "";
		int iTemp = 0;
		MediaID = 1;
		CodelineDataLength = iTemp;
		MagneticReadIndicator = sTemp;
		InsertOrientation = sTemp;
		MediaValidity = sTemp;
	}
};



///////////////////////////////////////////////////////
/*
 SENSOR-INDICATOR-UNIT-DEVICE
*/


struct SIUPortEvent
{
	template<class T> void orm(T& t)
	{

		t & ORM_COL(PortType);
		t & ORM_COL(PortIndex);
		t & ORM_COL(PortStatus);
		t & ORM_COL(Extra);

	}

	property<std::string> PortType;
	property<std::string> PortIndex;
	property<int> PortStatus;
	property<std::string> Extra;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "SIUPORTEVENT";
	}

	SIUPortEvent()
	{
		std::string sTemp = "";
		PortType = sTemp;
		PortIndex = sTemp;		
		Extra = sTemp;
		int nTemp = 0;
		PortStatus = nTemp;
	}
};



struct SIUPowerSaveChange
{
	template<class T> void orm(T& t)
	{

		t & ORM_COL(PowerSaveRecoveryTime);

	}

	property<int> PowerSaveRecoveryTime;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "SIUPOWERSAVECHANGE";
	}

	SIUPowerSaveChange()
	{
		int iTemp = 0;
		PowerSaveRecoveryTime = iTemp;
	}
};



struct SIUPortError
{
	template<class T> void orm(T& t)
	{

		t & ORM_COL(PortType);
		t & ORM_COL(PortIndex);
		t & ORM_COL(PortError);
		t & ORM_COL(PortStatus);
		t & ORM_COL(Extra);
	}

	property<std::string> PortType;
	property<std::string> PortIndex;
	property<std::string> PortError;
	property<int> PortStatus;
	property<std::string> Extra;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "SIUPORTERROR";
	}

	SIUPortError()
	{
		std::string sTemp = "";
		PortType = sTemp;
		PortIndex = sTemp;
		PortError = sTemp;
		Extra = sTemp;
		int nTemp = 0;
		PortStatus = nTemp;
	}
};


/************************************************************************/
/* CRD - DEVICE                                                         */
/************************************************************************/


struct CRDMediaDetected
{
	template<class T> void orm(T& t)
	{

		t & ORM_COL(Position);
		t & ORM_COL(Number);

	}

	property<std::string> Position;
	property<int> Number;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CRDMEDIADETECTED";
	}

	CRDMediaDetected()
	{
		Number = 0;
		Position = "";
	}
};


struct CRDCardUnitThreshold
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(Number);
		t & ORM_COL(CardName);
		t & ORM_COL(Type);
		t & ORM_COL(InitialCount);
		t & ORM_COL(Count);
		t & ORM_COL(RetainCount);
		t & ORM_COL(Threshold);
		t & ORM_COL(Status);
		t & ORM_COL(HardwareSensor);

	}

	property<int> Number;
	property<std::string> CardName;
	property<int> Type;
	property<int> InitialCount;
	property<int> Count;
	property<int> RetainCount;
	property<int> Threshold;
	property<int> Status;
	property<BOOL> HardwareSensor;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CRDCARDUNITTHRESHOLD";
	}

	CRDCardUnitThreshold()
	{
	}
};


struct CRDCardUnitError
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(Failure);
		t & ORM_COL(CardUnit);
	}

	property<std::string> Failure;
	property<CRDCardUnitThreshold> CardUnit;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CRDCARDUNITERROR";
	}

	CRDCardUnitError()
	{
		Failure = "";
	}
};


struct CRDDevicePosition
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(Position);
	}

	property<std::string> Position;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CRDDEVICEPOSITION";
	}

	CRDDevicePosition()
	{
		Position = "";
	}
};


struct CRDPowerSaveChange
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(PowerSaveRecoveryTime);
	}

	property<int> PowerSaveRecoveryTime;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "CRDPOWERSAVECHANGE";
	}

	CRDPowerSaveChange()
	{
		PowerSaveRecoveryTime = 0;
	}
};



/************************************************************************/
/* TextTerminalUnitDeviceMgr                                            */
/************************************************************************/

struct TTUDevicePosition
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(Position);
	}

	property<std::string> Position;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "TTUDEVICEPOSITION";
	}

	TTUDevicePosition()
	{
		Position = "";
	}
};

struct TTUPowerSaveChange
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(PowerSaveRecoveryTime);
	}

	property<int> PowerSaveRecoveryTime;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "TTUPOWERSAVECHANGE";
	}

	TTUPowerSaveChange()
	{
		PowerSaveRecoveryTime = 0;
	}
};

struct TTUFieldError
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(FormName);
		t & ORM_COL(FieldName);
		t & ORM_COL(Failure);
	}

	property<std::string> FormName;
	property<std::string> FieldName;
	property<std::string> Failure;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "TTUFIELDERROR";
	}

	TTUFieldError()
	{
		FormName = "";
		FieldName = "";
		Failure = "";
	}
};


struct TTUKey
{
	template<class T> void orm(T& t)
	{
		t & ORM_COL(cKey);
		t & ORM_COL(UNICODEKey);
		t & ORM_COL(CommandKey);
	}

	property<std::string> cKey;
	property<std::string> UNICODEKey;
	property<std::string> CommandKey;

	template<class A> void mapobjects(A* t)	{}

	static CStringA getClassName()
	{
		return "TTUKEY";
	}

	TTUKey()
	{
		cKey = "";
		UNICODEKey = "";
		CommandKey = "";
	}
};