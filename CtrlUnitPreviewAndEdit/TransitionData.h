#pragma once
#include "DataObjectBase.h"

// TransitionData コマンド ターゲット

class TransitionData : public DataObjectBase
{
public:
	TransitionData();
	virtual ~TransitionData();

	enum TransitionType
	{
		DISSOLVE
	};

private:
	TransitionType m_eTransitionType;
	int m_iTransitionDuration;
	BOOL m_fReverse;



public:

	BOOL InitializeInTransition(CString& strTransitionDataUUID);
	BOOL InitializeOutTransition(CString& strTransitionDataUUID);

	int GetTransitionDuration(void){ return m_iTransitionDuration; }
	void SetTransitionDuration(int iTransitionDuration){ m_iTransitionDuration = iTransitionDuration; }
	void AddTransitionDuration(int iNum){ m_iTransitionDuration += iNum; }	
};


