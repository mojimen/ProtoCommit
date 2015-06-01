#pragma once

#include <list>
#include <map>
#include <rpc.h>
#pragma comment(lib ,"rpcrt4.lib")

#include "TrackDataManager.h"

class ClipDataRect;

// TrackDataInfo �R�}���h �^�[�Q�b�g

typedef std::map<int, ClipDataRect*> ClipDataPositionMap;	//InPoint,ClipDataPointer 
//typedef std::map<int, UUID> ClipDataPositionMap;	//InPoint,ClipId 

class TrackDataInfo : public CObject
{
public:
	TrackDataInfo();
	virtual ~TrackDataInfo();

	enum TrackKind {
		VIDEO,
		AUDIO,
		TITLE,
		INFO,
		MASTER_VIDEO,
		MASTER_AUDIO
	};

private:
	TrackDataTag m_eTrackDataInfoTag;
	UUID m_uiTrackId;
	TrackKind m_eTrackKind;
	UUID m_uiInputChannel;
	UUID m_uiOutputChannel;
	ClipDataPositionMap m_mpClipDataInfoMap;

	// �Đ����Q�ƍ���
	BOOL m_fSolo;
	BOOL m_fAudioRecordable;
	BOOL m_fMuted;
	BOOL m_fLevelPointRecordable;
	
	// �r���[�Ƃ̃����N����
	//UUID m_uiTrackRectId;
	//TrackDataRect* m_pTrackDataRect;

public:
	BOOL InitTrackData(void);
	BOOL InitializeTrackId(UUID& uiClipId);


	void AddClip(const int iInPoint, ClipDataRect* pClipData);
	void DeleteClip(const int iInPoint);
	void ChangeClip(const int iOldInPoint, const int iNewInPoint, ClipDataRect* pClipData);

	// Getter
	ClipDataRect* GetClipDataInfo(const int iFrame, int& iInPoint);
	int GetClipDataAtFrame(const int iFrame, ClipDataPositionMap& mpClipMap);
	int GetClipDataInRange(const int iStartFrame, const int iEndFrame, ClipDataPositionMap& mpClipMap);
	//UUID GetTrackRectId(void) { return m_uiTrackRectId; }
	//TrackDataRect* GetTrackDataRect(void) { return m_pTrackDataRect; }

	// Setter
	//void SetTrackRectId(UUID uiTrackRectId) { m_uiTrackRectId = uiTrackRectId; }
	//void SetTrackDataRect(TrackDataRect* pTrackDataRect) { m_pTrackDataRect = pTrackDataRect; }



	// TODO: ����̓R���g���[���[�ɁH
	int CheckClipInSingleOutTrimRange(int iStartFrame, int iEndFrame);
	int CheckClipInSingleInTrimRange(int iStartFrame, int iEndFrame);
	ClipDataRect* CheckMove(ClipDataRect* pCheckClipData, const int iInPoint, const int iOutPoint);

};


