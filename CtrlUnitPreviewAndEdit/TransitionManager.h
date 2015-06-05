#pragma once
#include "DataObjectBase.h"
#include "TransitionData.h"

#include <map>

// TransitionManager コマンド ターゲット

typedef std::map<CString, TransitionData *> TransitionDataMap;


class TransitionManager : public DataObjectBase
{
public:
	TransitionManager();
	virtual ~TransitionManager();

private:
	TransitionDataMap m_mpTransitionDataMap;

public:
	BOOL InitializeTransitionManager(CString& strTransitionManagerUUID);
	void DeleteTransitionManager(void);
	TransitionData* GetTransitionData(CString& strTransitionUUID);
	BOOL CreateTransitionData(CString& strTransitionUUID, int iDuration, BOOL fIn = TRUE);
	BOOL SetTransitionData(TransitionData& cTransitionData);
	BOOL DeleteTransitionData(CString& strTransitionUUID);
	void DeleteAllTransitionData(void);
};


