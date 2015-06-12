#pragma once

class OpenGLRect;
class TimelineDataManager;
class TrackDataManager;
class TrackDataInfo;
class TrackDataRect;
class ClipDataManager;
class ClipDataRect;
class DragAndDropOperator;
class TransitionManager;

// TimelineDataOperator コマンド ターゲット

class TimelineDataOperator : public CObject
{
public:
	TimelineDataOperator();
	virtual ~TimelineDataOperator();

private:
	const double kSlideTrimAreaRate = 0.1;			// スライドトリムエリアとして使用する比率
	const double kTrimAreaRate = 0.2;				// トリムエリアとして使用する比率
	const int kClipHitCheckMinWidth = 15;			// クリップ当たり判定の最小保証幅（これよりクリップが短い場合に使用する幅）
	const int kTrimHitCheckMinWidth = 5;			// トリムチェックで使用する最小幅（これよりトリムチェック範囲が短い場合に使用する幅）
	const int kTrimHitCheckMaxWidth = 30;			// トリムチェックで使用する最大幅（これよりトリムチェック範囲が長い場合に使用する幅）


	UUID m_uiTimelineDataOperatorId;

	// 操作中クリップへのポインタ
	ClipDataRect* m_pOperatingClipData;

	// ドラッグ＆ドロップでドラッグ中のクリップ
	ClipDataRect* m_pDnDClipDataRect;
	PCTSTR m_pszDnDClipDataRectUUID;

	// TODO: 操作中トラックへのポインタ			複数選択時の考慮が必要
	TrackDataRect* m_pSelectedTrack;		// 選択したクリップのあったトラック
	TrackDataInfo* m_pSelectedTrackInfo;
	TrackDataRect* m_pOperateToTrack;		// 移動先トラック
	TrackDataInfo* m_pOperateToTrackInfo;

	OpenGLRect* m_pTimelineEditPanelRect;
	OpenGLRect* m_pTimelineEditHeaderRect;
	OpenGLRect* m_pTimelineControlPanelRect;
	OpenGLRect* m_pSeekBarRect;
	OpenGLRect* m_pTrackHeaderRect;
	OpenGLRect* m_pTimelineDataRect;
	OpenGLRect* m_pTimelineCursorHitArea;
	//TODO: これはいらないかも？
	OpenGLRect* m_pTransitionRect;

	CRect m_rcMousePointRect;			// 移動中のイメージ（重なり中のイメージ）
	CPoint m_poMousePointerLocation;	// マウスボタンが押されたときの位置
	BOOL m_fLButtonDown;				// マウスボタンが押されているかどうかを記録
	BOOL m_fMove;						// Move操作中
	BOOL m_fSingleInTrim;				// In側SingleTrim操作中
	BOOL m_fSingleOutTrim;				// Out側SingleTrim操作中
	BOOL m_fScrub;						// Scrub操作中
	BOOL m_fDragShuttle;				// DragShuttle操作中
	BOOL m_fDragAndDrop;				// Drag&Drop操作中
	BOOL m_fAllowFile;					// Drag&Drop操作物のドロップ可否（形式チェック結果）
	BOOL m_fAllowDrop;					// Drag&Drop操作可否
	BOOL m_fPreviewWork;				// プレビュー動作中
	BOOL m_fPlay;						// 再生中
	BOOL m_fJumpFrame;					// ジャンプフレーム時のアニメーション動作要

	// タイムラインデータ管理
	TimelineDataManager* m_pTimelineDataManager;
	UUID m_uiTimelineDataManagerId;
	TrackDataManager* m_pTrackDataVideoManager;
	UUID m_uiTrackDataVideoManagerId;
	TrackDataManager* m_pTrackDataAudioManager;
	UUID m_uiTrackDataAudioManagerId;
	TrackDataManager* m_pTrackDataInfoManager;
	UUID m_uiTrackDataInfoManagerId;
	TrackDataManager* m_pTrackDataMasterManager;
	UUID m_uiTrackDataMasterManagerId;
	ClipDataManager* m_pClipDataManager;
	UUID m_uiClipDataManagerUUID;
	TransitionManager* m_pTransitionManager;
	CString m_strTransitionManagerUUID;

	// オペレータ
	DragAndDropOperator* m_pDropAndDragOperator;
	UUID m_uiDropAndDragOperatorId;

	int m_iTimelineCursorFramePosition;	// タイムラインカーソル位置のフレーム番号
	int m_iLeftFrameNumber;				// タイムラインデータ表示範囲の先頭フレーム
	int m_iRightFrameNumber;			// タイムラインデータ表示範囲の最終フレーム
	int m_iOperatingFrameCount;			// 操作中の移動フレーム数
	int m_iOperatingTimelineCursorFramePosition;	// タイムラインカーソル位置のフレーム番号
	int m_iOperatingLeftFrameNumber;	// タイムラインデータ表示範囲の先頭フレーム
	int m_iOperatingRightFrameNumber;	// タイムラインデータ表示範囲の最終フレーム
	int m_iOperatingClipFrameCount;		// 操作中クリップの移動／伸縮フレーム数
	float m_fSuttleSpeed;				// シャトル操作中の移動スピード（倍）
	int m_iEnableMovingFrameCount;		// Move操作中に移動が可能であった直近の移動フレーム数
	TrackDataRect* m_pEnableMovingTrack;// Move操作中に移動が可能であった直近のトラック位置
	int m_iPlayFrameCount;				// 再生中の再生済みフレーム数
	int m_iJumpFrameCount;				// ジャンプフレーム時の移動フレーム数
	int m_iJumpFrameCursorPosition;		// ジャンプフレーム時の移動フレーム数
	int m_iShuttleSpeedNumerator;		// シャトル再生時のスピード（分子）
	int m_iShuttleSpeedDenominator;		// シャトル再生時のスピード（分母）

	int m_iFramePerPoint;				// １ポイントあたりのフレーム数
	int m_iPointPerFrame;				// １フレームあたりのポイント数
	int m_iSmallScaleDrawInterval;		// 小目盛りあたりのフレーム数
	int m_iMiddleScaleDrawInterval;		// 中目盛りあたりの小目盛り数
	int m_iBigScaleDrawInterval;		// 大目盛りあたりの中目盛り数
	int m_iTimelineCursorPoint;			// タイムラインカーソルの描画位置


public:
	BOOL InitializeTimelineDataOperator(UUID& uiTimelineDataOperatorId);
	void DeleteTimelineDataOperator(void);
	BOOL OnLButtonDown(UINT nFlags, CPoint point);	//Lボタンの処理
	BOOL OnLButtonUp(UINT nFlags, CPoint point, BOOL& fStopPlay);	//Lボタンの処理
	BOOL OnRButtonUp(UINT nFlags, CPoint point);	//Lボタンの処理
	BOOL OnMouseMove(UINT nFlags, CPoint point, BOOL& fStartPlay);	//マウス移動時の処理
	BOOL OnDropFiles(const HDROP& hDropInfo, CString& strFileName);
	BOOL OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, const CPoint& point);
	void OnDragLeave(void);
	DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, const CPoint& point);
	//BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	BOOL OnContextMenu(const CPoint& poClientPoint, CMenu& mContextMenu);
	BOOL OnTransitionSetIn(PCTSTR pszMessage);
	BOOL OnTransitionSetOut(PCTSTR pszMessage);
	BOOL OnTransitionResetInCenter(void);
	BOOL OnTransitionResetInStart(void);
	BOOL OnTransitionResetInEnd(void);
	BOOL OnTransitionResetOutCenter(void);
	BOOL OnTransitionResetOutStart(void);
	BOOL OnTransitionResetOutEnd(void);
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint poPoint, BOOL& fCursorChange);

	// プレビュー連動
	BOOL OnPlay(int iSpeed);
	void OnStop(void);


	BOOL ChangeDisplayScale(void);
	void CalcTimelineDisplayRange(void);

	// ヒットテスト
	TrackDataRect* IsPointInAnyTrack(const CPoint& point);
	BOOL IsPointInAnyClipRect(const CPoint& point, TrackDataInfo& cTrackInfo);
	BOOL IsPointInClip(const CPoint& poPoint);
	BOOL IsPointInClipRect(const CPoint& point, const CRect& rcClipRect);
	BOOL IsPointInTrimRange(const CPoint& point, const CRect& rcClipRect);
	BOOL IsPointInSeekBar(const CPoint& point);
	BOOL IsPointInTimelineControlPanel(const CPoint& point);
	ClipDataRect* IsClipAtIn(TrackDataInfo& cTrackInfo, const int& iIn);
	ClipDataRect* IsClipAtOut(TrackDataInfo& cTrackInfo, const int& iOut);

	// 操作チェック
	BOOL CheckInTrim(void);
	BOOL CheckOutTrim(void);
	BOOL CheckMove(CPoint& point);

	// 座標計算・変換
	BOOL CalcClipRectDisplayPoint(CRect& rcClipRect, const ClipDataRect* clClipData, const CRect& rcTrackRect,
		const int& iMoveFrame = 0, const int& iIntrimFrames = 0, const int& iOuttrimFrames = 0);
	BOOL CalcClipRect(CRect& rcClipRect, const int& iInPoint, const int& iDuration, const CRect& rcTrackRect,
		const int& iMoveFrame = 0, const int& iIntrimFrames = 0, const int& iOuttrimFrames = 0);
	int ChangeTimelineFramePositionToDisplayPoint(const int iFrame);
	int ChangeDisplayPointToTimelineFramePosition(const CPoint& point, int& iActualFrame);
	int ChangeOperatingDistanceToTimelineFrames(const CSize& szMoveSize, const int iStartFrame = 0);


	// Getter
	OpenGLRect* GetTimelineEditPanelRect(void){ return m_pTimelineEditPanelRect; }
	OpenGLRect* GetTimelineEditHeaderRect(void){ return m_pTimelineEditHeaderRect; }
	OpenGLRect* GetTimelineControlPanelRect(void){ return m_pTimelineControlPanelRect; }
	OpenGLRect* GetSeekBarRect(void){ return m_pSeekBarRect; }
	OpenGLRect* GetTrackHeaderRect(void){ return m_pTrackHeaderRect; }
	OpenGLRect* GetTimelineDataRect(void){ return m_pTimelineDataRect; }
	OpenGLRect* GetTimelineCursorHitArea(void){ return m_pTimelineCursorHitArea; }
	//TODO: これはいらないかも？
	OpenGLRect* GetTransitionRect(void){ return m_pTransitionRect; }

	TimelineDataManager* GetTimelineDataManager(void){ return m_pTimelineDataManager; }
	UUID GetTimelineDataManagerId(void){ return m_uiTimelineDataManagerId; }
	BOOL IsLButtonDown(void){ return m_fLButtonDown; }
	BOOL IsSingleInTrim(void){ return m_fSingleInTrim; }
	BOOL IsSingleOutTrim(void){ return m_fSingleOutTrim; }
	BOOL IsMove(void){ return m_fMove; }
	BOOL IsScrub(void){ return m_fScrub; }
	BOOL IsDragShuttle(void){ return m_fDragShuttle; }
	BOOL IsDragAndDrop(void){ return m_fDragAndDrop; }
	BOOL IsDropFileCrrect(void){ return m_fDragAndDrop && m_fAllowFile; }
	BOOL EnableDrawDragRect(void){ return m_fDragAndDrop && m_fAllowFile && m_fAllowDrop; }
	BOOL IsPlaying(void) { return m_fPlay; }
	BOOL IsPreviewWorking(void) { return m_fPreviewWork; }
	BOOL IsJumpFrame(void) { return m_fJumpFrame; }
	void ResetJumpFrame(void) { m_fJumpFrame = FALSE; }

	int GetTimelineCursorFramePosition(void){ return m_iTimelineCursorFramePosition; }
	int GetLeftFrameNumber(void){ return m_iLeftFrameNumber; }
	int GetRightFrameNumber(void){ return m_iRightFrameNumber; }
	int GetOperatingFrameCount(void){ return m_iOperatingFrameCount; }
	int GetOperatingTimelineCursorFramePosition(void){ return m_iOperatingTimelineCursorFramePosition; }
	int GetOperatingLeftFrameNumber(void){ return m_iOperatingLeftFrameNumber; }
	int GetOperatingRightFrameNumber(void){ return m_iOperatingRightFrameNumber; }
	int GetOperatingClipFrameCount(void){ return m_iOperatingClipFrameCount; }
	float GetSuttleSpeed(void){ return m_fSuttleSpeed; }
	int GetEnableMovingFrameCount(void){ return m_iEnableMovingFrameCount; }
	TrackDataRect* GetEnableMovingTrack(void){ return m_pEnableMovingTrack; }

	int GetFramePerPoint(void){ return m_iFramePerPoint; }
	int GetPointPerFrame(void){ return m_iPointPerFrame; }
	int GetSmallScaleDrawInterval(void){ return m_iSmallScaleDrawInterval; }
	int GetMiddleScaleDrawInterval(void){ return m_iMiddleScaleDrawInterval; }
	int GetBigScaleDrawInterval(void){ return m_iBigScaleDrawInterval; }
	int GetTimelineCursorPoint(void){ return m_iTimelineCursorPoint; }
	int GetJumpFrameCount(void){ return m_iJumpFrameCount; }
	int GetJumpFrameCursorPosition(void){ return m_iJumpFrameCursorPosition; }
	int GetShuttleSpeedNumerator(void){ return m_iShuttleSpeedNumerator; }
	int GetShuttleSpeedDenominator(void){ return m_iShuttleSpeedDenominator; }

	ClipDataRect* GetOperatingClipData(void){ return m_pOperatingClipData; }
	ClipDataRect* GetDragAndDropClipDataRect(void){ return m_pDnDClipDataRect; }
	CRect GetMousePointRect(void){ return m_rcMousePointRect; }
	CPoint GetMousePointerLocation(void){ return m_poMousePointerLocation; }

	TrackDataRect* GetSelectedTrack(void){ return m_pSelectedTrack; }
	TrackDataInfo* GetSelectedTrackInfo(void){ return m_pSelectedTrackInfo; }
	TrackDataRect* GetOperateToTrack(void){ return m_pOperateToTrack; }
	TrackDataInfo* GetOperateToTrackInfo(void){ return m_pOperateToTrackInfo; }

	void SetRectAroundPoint(ClipDataRect& pClipRect, const POINT point, const int& iInputHeight);
	void ClearJumpFramecount(void) { m_iJumpFrameCount = 0; }
	void MoveTimelineCursor(int iLength);
	BOOL CalcShuttleSpeed(const CPoint& point, const CSize& szMoveSize, int& iNumerator, int& iDenominator);

	BOOL DeleteClip(void);
	CMenu* FindSubMenuFromString(CMenu* pMenu, LPCTSTR pszSearchString, int& nPos);

};


