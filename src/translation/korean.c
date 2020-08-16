#include "translation/common.h"
#include "translation/translation.h"

static translation_string all_strings[] = {
    {TR_NO_PATCH_TITLE, "1.0.1.0 패치가 설치되지 않음"},
    {TR_NO_PATCH_MESSAGE,
        "현재 설치된 시저 3는 1.0.1.0 패치를 포함하고 있지 않습니다. "
        "패치는 이곳에서 다운로드하실 수 있습니다. \n"
        URL_PATCHES "\n"
        "패치 없이 진행하는 데 따른 문제는 본인 책임입니다."},
    // Do not translate the following message since it cannot be shown in Korean:
    {TR_MISSING_FONTS_TITLE, "Missing fonts"},
    {TR_MISSING_FONTS_MESSAGE,
        "Your Caesar 3 installation requires extra font files. "
        "You can download them for your language from:\n"
        URL_PATCHES},
    {TR_NO_EDITOR_TITLE, "편집기가 설치되지 않음"},
    {TR_NO_EDITOR_MESSAGE,
        "현재 설치된 시저 3는 편집기 파일을 포함하고 있지 않습니다. "
        "편집기는 이곳에서 다운로드하실 수 있습니다. \n"
        URL_EDITOR},
    {TR_INVALID_LANGUAGE_TITLE, "잘못된 언어 디렉터리"},
    {TR_INVALID_LANGUAGE_MESSAGE,
        "현재 선택된 디렉터리는 사용 가능한 언어 팩을 포함하고 있지 않습니다. "
        "기록에서 오류를 확인해 주세요."},
    {TR_BUTTON_OK, "확인"},
    {TR_BUTTON_CANCEL, "취소"},
    {TR_BUTTON_RESET_DEFAULTS, "기본값으로 초기화"},
    {TR_BUTTON_CONFIGURE_HOTKEYS, "단축키 설정"},
    {TR_CONFIG_TITLE, "Julius 설정 옵션"},
    {TR_CONFIG_LANGUAGE_LABEL, "언어:"},
    {TR_CONFIG_LANGUAGE_DEFAULT, "(기본)"},
    {TR_CONFIG_HEADER_UI_CHANGES, "사용자 인터페이스 변경"},
    {TR_CONFIG_HEADER_GAMEPLAY_CHANGES, "게임플레이 변경"},
    {TR_CONFIG_SHOW_INTRO_VIDEO, "시작 영상 재생"},
    {TR_CONFIG_SIDEBAR_INFO, "게임 제어판에 추가 정보 표시"},
    {TR_CONFIG_SMOOTH_SCROLLING, "부드러운 화면이동 가능"},
    {TR_CONFIG_VISUAL_FEEDBACK_ON_DELETE, "지형 평탄화 시 시각적 표현 향상"},
    {TR_CONFIG_ALLOW_CYCLING_TEMPLES, "각각의 신전을 연속으로 설치 가능"},
    {TR_CONFIG_SHOW_WATER_STRUCTURE_RANGE, "저수조, 분수와 우물 설치 시 범위 표시"},
    {TR_CONFIG_SHOW_CONSTRUCTION_SIZE, "드래그 건설 시 전체 크기 표시"},
    {TR_CONFIG_HIGHLIGHT_LEGIONS, "커서가 올라간 군단 강조"},
    {TR_CONFIG_FIX_IMMIGRATION_BUG, "매우 어려움 난이도 이주 문제 수정"},
    {TR_CONFIG_FIX_100_YEAR_GHOSTS, "100세 이상 고령 주민 문제 수정"},
    {TR_CONFIG_FIX_EDITOR_EVENTS, "맞춤형 임무에서 황제 교체와 생존 시간 문제 수정"},
    {TR_HOTKEY_TITLE, "Julius 단축키 설정"},
    {TR_HOTKEY_LABEL, "단축키"},
    {TR_HOTKEY_ALTERNATIVE_LABEL, "대체"},
    {TR_HOTKEY_HEADER_ARROWS, "화살표키"},
    {TR_HOTKEY_HEADER_GLOBAL, "전역 단축키"},
    {TR_HOTKEY_HEADER_CITY, "도시 단축키"},
    {TR_HOTKEY_HEADER_ADVISORS, "자문관"},
    {TR_HOTKEY_HEADER_OVERLAYS, "맵보기"},
    {TR_HOTKEY_HEADER_BOOKMARKS, "도시 맵 즉시 이동"},
    {TR_HOTKEY_HEADER_EDITOR, "편집기"},
    {TR_HOTKEY_HEADER_BUILD, "건설 단축키"}, // TODO: Google Translate
    {TR_HOTKEY_ARROW_UP, "위"},
    {TR_HOTKEY_ARROW_DOWN, "아래"},
    {TR_HOTKEY_ARROW_LEFT, "왼쪽"},
    {TR_HOTKEY_ARROW_RIGHT, "오른쪽"},
    {TR_HOTKEY_TOGGLE_FULLSCREEN, "전체화면 전환"},
    {TR_HOTKEY_CENTER_WINDOW, "창을 화면 가운데로"},
    {TR_HOTKEY_RESIZE_TO_640, "창 크기 640x480으로 변경"},
    {TR_HOTKEY_RESIZE_TO_800, "창 크기 800x600으로 변경"},
    {TR_HOTKEY_RESIZE_TO_1024, "창 크기 1024x768로 변경"},
    {TR_HOTKEY_SAVE_SCREENSHOT, "스크린샷 저장"},
    {TR_HOTKEY_SAVE_CITY_SCREENSHOT, "도시 전체 스크린샷 저장"},
    {TR_HOTKEY_BUILD_CLONE, "커서 아래에 건물 복제"}, // TODO: Google Translate
    {TR_HOTKEY_LOAD_FILE, "파일 불러오기"},
    {TR_HOTKEY_SAVE_FILE, "파일 저장하기"},
    {TR_HOTKEY_INCREASE_GAME_SPEED, "게임 속도 증가"},
    {TR_HOTKEY_DECREASE_GAME_SPEED, "게임 속도 감소"},
    {TR_HOTKEY_TOGGLE_PAUSE, "일시 정지 전환"},
    {TR_HOTKEY_CYCLE_LEGION, "군단 돌아가며 보기"},
    {TR_HOTKEY_ROTATE_MAP_LEFT, "시계 반대 방향으로 맵 회전"},
    {TR_HOTKEY_ROTATE_MAP_RIGHT, "시계 방향으로 맵 회전"},
    {TR_HOTKEY_SHOW_ADVISOR_LABOR, "노동 자문관"},
    {TR_HOTKEY_SHOW_ADVISOR_MILITARY, "군단 자문관"},
    {TR_HOTKEY_SHOW_ADVISOR_IMPERIAL, "황제 자문관"},
    {TR_HOTKEY_SHOW_ADVISOR_RATINGS, "등급 자문관"},
    {TR_HOTKEY_SHOW_ADVISOR_TRADE, "무역 자문관"},
    {TR_HOTKEY_SHOW_ADVISOR_POPULATION, "인구 자문관"},
    {TR_HOTKEY_SHOW_ADVISOR_HEALTH, "보건 자문관"},
    {TR_HOTKEY_SHOW_ADVISOR_EDUCATION, "교육 자문관"},
    {TR_HOTKEY_SHOW_ADVISOR_ENTERTAINMENT, "오락 자문관"},
    {TR_HOTKEY_SHOW_ADVISOR_RELIGION, "종교 자문관"},
    {TR_HOTKEY_SHOW_ADVISOR_FINANCIAL, "재정 자문관"},
    {TR_HOTKEY_SHOW_ADVISOR_CHIEF, "수석 자문관"},
    {TR_HOTKEY_TOGGLE_OVERLAY, "현재 맵보기 전환"},
    {TR_HOTKEY_SHOW_OVERLAY_WATER, "물공급 맵보기 전환"},
    {TR_HOTKEY_SHOW_OVERLAY_FIRE, "화재 맵보기 전환"},
    {TR_HOTKEY_SHOW_OVERLAY_DAMAGE, "손상 맵보기 전환"},
    {TR_HOTKEY_SHOW_OVERLAY_CRIME, "범죄 맵보기 전환"},
    {TR_HOTKEY_SHOW_OVERLAY_PROBLEMS, "문제 맵보기 전환"},
    {TR_HOTKEY_GO_TO_BOOKMARK_1, "1지점으로 이동"},
    {TR_HOTKEY_GO_TO_BOOKMARK_2, "2지점으로 이동"},
    {TR_HOTKEY_GO_TO_BOOKMARK_3, "3지점으로 이동"},
    {TR_HOTKEY_GO_TO_BOOKMARK_4, "4지점으로 이동"},
    {TR_HOTKEY_SET_BOOKMARK_1, "1지점 설정"},
    {TR_HOTKEY_SET_BOOKMARK_2, "2지점 설정"},
    {TR_HOTKEY_SET_BOOKMARK_3, "3지점 설정"},
    {TR_HOTKEY_SET_BOOKMARK_4, "4지점 설정"},
    {TR_HOTKEY_EDITOR_TOGGLE_BATTLE_INFO, "전투 정보 전환"},
    {TR_HOTKEY_EDIT_TITLE, "새 단축키 입력"}
};

void translation_korean(const translation_string **strings, int *num_strings)
{
    *strings = all_strings;
    *num_strings = sizeof(all_strings) / sizeof(translation_string);
}
