	// 共通カラー
#define BLACKCOLOR_BRUSH_FLOAT 0.0f, 0.0f, 0.0f, 1.0f
#define WHITECOLOR_BRUSH_FLOAT 1.0f, 1.0f, 1.0f, 1.0f
#define REDCOLOR_BRUSH_FLOAT 1.0f, 0.0f, 0.0f, 1.0f
#define BLUECOLOR_BRUSH_FLOAT 0.0f, 0.0f, 1.0f, 1.0f
#define GREENCOLOR_BRUSH_FLOAT 0.0f, 1.0f, 0.0f, 1.0f
#define DARKGRAYCOLOR_BRUSH_FLOAT 0.3f, 0.3f, 0.3f, 1.0f
#define DARKGRAYCOLOR2_BRUSH_FLOAT 0.2f, 0.2f, 0.2f, 1.0f
#define DARKGRAYCOLOR3_BRUSH_FLOAT 0.1f, 0.1f, 0.1f, 1.0f
#define GRAYCOLOR_BRUSH_FLOAT 0.5f, 0.5f, 0.5f, 1.0f
#define LIGHTGRAYCOLOR_BRUSH_FLOAT 0.7f, 0.7f, 0.7f, 1.0f
#define LIGHTGRAYCOLOR2_BRUSH_FLOAT 0.8f, 0.8f, 0.8f, 1.0f
#define LIGHTGRAYCOLOR3_BRUSH_FLOAT 0.9f, 0.9f, 0.9f, 1.0f

#define ACCENTCOLOR_BRUSH_FLOAT 0.3f, 0.7f, 1.0f, 1.0f
#define ACCENTCOLOR_HALFALPHA_BRUSH_FLOAT 0.3f, 0.7f, 1.0f, 0.5f
#define ACCENTCOLOR2_BRUSH_FLOAT 0.3f,0.4f,1.0f,1.0f
#define ACCENTCOLOR2_HALFALPHA_BRUSH_FLOAT 0.3f,0.4f,1.0f,0.5f
#define ACCENTCOLOR2_ALPHA1_BRUSH_FLOAT 0.3f,0.4f,1.0f,0.7f
#define ACCENTCOLOR3_BRUSH_FLOAT 0.8f,0.9f,1.0f,1.0f
#define ACCENTCOLOR4_BRUSH_FLOAT 0.5f,0.8f,1.0f,1.0f

#define BASECOLOR_BRUSH_FLOAT WHITECOLOR_BRUSH_FLOAT
#define CAUTIONCOLOR_BRUSH_FLOAT REDCOLOR_BRUSH_FLOAT

	// 固有カラー
#define PREVIEWPANELBORDER_BRUSH_FLOAT LIGHTGRAYCOLOR_BRUSH_FLOAT

#define TIMELINEBASECOLOR_BRUSH_FLOAT BASECOLOR_BRUSH_FLOAT

#define TIMELINECONTROLPANELBACKGROUNDCOLOR_BRUSH_FLOAT ACCENTCOLOR2_BRUSH_FLOAT
#define SEEKBARBACKGROUNDCOLOR_BRUSH_FLOAT ACCENTCOLOR2_BRUSH_FLOAT
#define TIMELINETRACKHEADERBORDERCOLOR_BRUSH_FLOAT LIGHTGRAYCOLOR_BRUSH_FLOAT
#define TIMELINETRACKCOLOR_BRUSH_FLOAT WHITECOLOR_BRUSH_FLOAT
#define TIMELINETRACKCOLOR_BRUSH2_FLOAT LIGHTGRAYCOLOR3_BRUSH_FLOAT
#define TIMELINETRACKBORDERCOLOR_BRUSH_FLOAT ACCENTCOLOR2_ALPHA1_BRUSH_FLOAT
#define TIMELINETRACKBORDERCOLOR_BRUSH2_FLOAT ACCENTCOLOR2_ALPHA1_BRUSH_FLOAT
#define TIMELINEDATAAREABORDERCOLOR_BRUSH_FLOAT LIGHTGRAYCOLOR_BRUSH_FLOAT

#define SEEKBARBIGSCALECOLOR_BRUSH_FLOAT LIGHTGRAYCOLOR2_BRUSH_FLOAT
#define SEEKBARMIDDLESCALECOLOR_BRUSH_FLOAT LIGHTGRAYCOLOR2_BRUSH_FLOAT
#define SEEKBARSMALLSCALECOLOR_BRUSH_FLOAT LIGHTGRAYCOLOR_BRUSH_FLOAT

#define SEEKBARTIMECODETEXTCOLOR_BRUSH_FLOAT LIGHTGRAYCOLOR3_BRUSH_FLOAT

#define SEEKBARBIGSCALELINECOLOR_BRUSH_FLOAT SEEKBARBIGSCALECOLOR_BRUSH_FLOAT
#define SEEKBARMIDDLESCALELINECOLOR_BRUSH_FLOAT 1.0f, 0.8f, 1.0f, 1.0f
#define SEEKBARSMALLSCALELINECOLOR_BRUSH_FLOAT 1.0f, 1.0f, 0.9f, 1.0f

#define TIMELINECURSORCOLOR_BRUSH_FLOAT 1.0f, 0.0f, 1.0f, 0.6f
#define TIMELINECURSORDRAGGUIDELINECOLOR_BRUSH ACCENTCOLOR_BRUSH

#define CLIPCOLOR_BRUSH_FLOAT ACCENTCOLOR_BRUSH_FLOAT
#define CLIPBORDERCOLOR_BRUSH_FLOAT LIGHTGRAYCOLOR2_BRUSH_FLOAT
#define CLIPCOLOR_OPERATING_BRUSH_FLOAT LIGHTGRAYCOLOR3_BRUSH_FLOAT
#define CLIPCOLOR_MOVING_BRUSH_FLOAT ACCENTCOLOR_HALFALPHA_BRUSH_FLOAT
#define CLIPCOLOR_SINGLEINTRIMING_BRUSH_FLOAT ACCENTCOLOR_HALFALPHA_BRUSH_FLOAT
#define CLIPCOLOR_SINGLEOUTTRIMING_BRUSH_FLOAT ACCENTCOLOR_HALFALPHA_BRUSH_FLOAT
#define CLIPCOLOR_OVERLAPPING_BRUSH_FLOAT 1.0f, 0.0f, 0.0f, 0.2f
#define CLIPCOLOR_SINGLETRIMINGBORDER_BRUSH_FLOAT 1.0f, 0.7f, 0.3f, 0.7f


// 動作モード
#define PROTOTYPEMODE
#define SEEKBAR_MIDDLESCALELINE_DRAW
#define SEEKBAR_SMALLSCALELINE_DRAW

// 文字種別／サイズ
#define FONTFACE_MSMINCHOU _T("ＭＳ 明朝")
#define FONTFACE_MSGOTHIC _T("ＭＳ ゴシック")
#define FONTFACE_MEIRIO _T("メイリオ")
#define DEFAULT_FONTFACE FONTFACE_MSGOTHIC
#define SEEKBARTIMECODE_FONTSIZE 15
#define SEEKBARTIMECODE_MIDDLE_FONTSIZE 12

// 線
#define PREVIEWPANELBORDER_LINEWIDTH 1.0f
#define SEEKBARBIGSCALE_LINEWIDTH 1.0f
#define SEEKBARMIDDLESCALE_LINEWIDTH 1.0f
#define SEEKBARSMALLSCALE_LINEWIDTH 1.0f
#define SEEKBARBIGSCALELINE_LINEWIDTH SEEKBARBIGSCALE_LINEWIDTH
#define SEEKBARMIDDLESCALELINE_LINEWIDTH SEEKBARMIDDLESCALE_LINEWIDTH
#define SEEKBARSMALLSCALELINE_LINEWIDTH SEEKBARSMALLSCALE_LINEWIDTH
#define TIMELINECURSOR_LINEWIDTH 1.0f


//マージン、位置補正
#define SEEKBARTIMECODE_MARGINX 2
#define SEEKBARTIMECODE_MARGINY 16
#define SEEKBARTIMECODE_MIDDLE_MARGINY 17
#define SEEKBARBIGSCALE_TOPMARGIN 10
#define SEEKBARMIDDLESCALE_TOPMARGIN 20
#define SEEKBARSMALLSCALE_TOPMARGIN 25
#define TIMELINE_DEFAULTZ 0.0f
#define CLIPHIGHT 0.9f
#define TRACK_NAME_MARGINTOP 20
#define TRACK_NAME_MARGINLEFT 10


#define TRACK_HIEGHT_DEFAULT 50
#define TRACK_HIEGHT_MIN 10
#define TRACK_HIEGHT_MAX 300

#define SINGLETRIMINGBORDER_WIDTH 10



// トランジション
#define TRANSITION_DEFAULT_LENGTH 120




// エラーメッセージ
#define TRANSITION_GAP_NOT_EXIST _T("余分な映像が存在しないためトランジションの設定ができません")
