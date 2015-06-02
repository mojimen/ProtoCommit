#pragma once

class OpenGLRect;
class TimelineDataManager;
class TrackDataManager;
class TrackDataInfo;
class TrackDataRect;
class ClipDataManager;
class ClipDataRect;

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


	// 操作中クリップへのポインタ
	ClipDataRect* m_clMovingClipData;

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
	OpenGLRect* m_pTransisionRect;

	CRect m_rcMousePointRect;			// 移動中のイメージ（重なり中のイメージ）
	CPoint m_poMousePointerLocation;	// マウスボタンが押されたときの位置
	BOOL m_fLButtonClicking;			// マウスボタンが押されているかどうかを記録
	BOOL m_fMoving;						// Move操作中
	BOOL m_fSingleInTriming;			// In側SingleTrim操作中
	BOOL m_fSingleOutTriming;			// Out側SingleTrim操作中
	BOOL m_fScrubing;					// Scrub操作中
	BOOL m_fDragShuttling;				// DragShuttle操作中

	// タイムラインデータ管理
	TimelineDataManager* m_pTimelineDataManager;
	UUID m_uiTimelineDataManager;
	TrackDataManager* m_pTrackDataVideoManager;
	UUID m_uiTrackDataVideoManager;
	TrackDataManager* m_pTrackDataAudioManager;
	UUID m_uiTrackDataAudioManager;
	TrackDataManager* m_pTrackDataInfoManager;
	UUID m_uiTrackDataInfoManager;
	TrackDataManager* m_pTrackDataMasterManager;
	UUID m_uiTrackDataMasterManager;
	ClipDataManager* m_pClipDataManager;
	UUID m_uiClipDataManager;

	int m_iTimelineCursorFramePosition;	// タイムラインカーソル位置のフレーム番号
	int m_iLeftFrameNumber;				// タイムラインデータ表示範囲の先頭フレーム
	int m_iRightFrameNumber;			// タイムラインデータ表示範囲の最終フレーム
	int m_iOperatingFrameCount;			// 操作中の移動フレーム数
	int m_iOperatingTimelineCursorFramePosition;	// タイムラインカーソル位置のフレーム番号
	int m_iOperatingLeftFrameNumber;	// タイムラインデータ表示範囲の先頭フレーム
	int m_iOperatingRightFrameNumber;	// タイムラインデータ表示範囲の最終フレーム
	int m_iOperatingClipFrameCount;		// 操作中クリップの移動／伸縮フレーム数
	int m_fSuttleSpeed;					// シャトル操作中の移動スピード（倍）
	int m_iEnableMovingFrameCount;		// Move操作中に移動が可能であった直近の移動フレーム数
	TrackDataRect* m_pEnableMovingTrack;// Move操作中に移動が可能であった直近のトラック位置

	int m_iFramePerPoint;				// １ポイントあたりのフレーム数
	int m_iPointPerFrame;				// １フレームあたりのポイント数
	int m_iSmallScaleDrawInterval;		// 小目盛りあたりのフレーム数
	int m_iMiddleScaleDrawInterval;		// 中目盛りあたりの小目盛り数
	int m_iBigScaleDrawInterval;		// 大目盛りあたりの中目盛り数
	int m_iTimelineCursorPoint;			// タイムラインカーソルの描画位置

	void InitializeTimelineDataOperator(void);
	void DeleteTimelineDataOperator(void);
	BOOL OnLButtonDown(UINT nFlags, CPoint point);	//L押下時の処理
	
	BOOL ChangeDisplayScale(void);
	void CalcTimelineDisplayRange(void);

	// ヒットテスト
	TrackDataRect* IsPointInAnyTrack(const CPoint& point);
	BOOL IsPointInAnyClipRect(const CPoint& point);
	BOOL IsPointInClipRect(const CPoint& point, const CRect& rcClipRect);
	BOOL IsPointInTrimRange(const CPoint& point, const CRect& rcClipRect);
	//BOOL CheckInTrim(void);
	//BOOL CheckOutTrim(void);
	//BOOL CheckMove(CPoint& point);
	BOOL IsPointInSeekBar(const CPoint& point);
	BOOL IsPointInTimelineControlPanel(const CPoint& point);

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
	OpenGLRect* GetTransisionRect(void){ return m_pTransisionRect; }

};


