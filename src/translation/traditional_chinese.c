#include "translation/common.h"
#include "translation/translation.h"

static translation_string all_strings[] = {
    {TR_NO_PATCH_TITLE, "未安裝1.0.1.0補丁"},
    {TR_NO_PATCH_MESSAGE,
        "《凱撒大帝3》未安裝1.0.1.0補丁。"
        "補丁下載鏈接：\n"
        URL_PATCHES "\n"
        "繼續游戲風險自擔。"},
    {TR_MISSING_FONTS_TITLE, "缺失字體"},
    {TR_MISSING_FONTS_MESSAGE,
        "《凱撒大帝3》需要額外字體文件。"
        "語言包下載鏈接：\n"
        URL_PATCHES},
    {TR_NO_EDITOR_TITLE, "未安裝地圖編輯器"},
    {TR_NO_EDITOR_MESSAGE,
        "《凱撒大帝3》未檢測到地圖編輯器文件。"
        "地圖編輯器下載鏈接：\n"
        URL_EDITOR},
    {TR_INVALID_LANGUAGE_TITLE, "語言包路徑無效"},
    {TR_INVALID_LANGUAGE_MESSAGE,
        "指定路徑未檢測到有效語言包。"
        "請檢視日志查看錯誤。"},
    {TR_BUILD_ALL_TEMPLES, "全   部"},
    {TR_BUTTON_OK, "確定"},
    {TR_BUTTON_CANCEL, "取消"},
    {TR_BUTTON_RESET_DEFAULTS, "重置默認"},
    {TR_BUTTON_CONFIGURE_HOTKEYS, "熱鍵綁定"},
    {TR_CONFIG_ARE_YOU_SURE, "重置為默認?"},
    {TR_CONFIG_YOU_WILL_LOSE_YOUR_CONFIGS, "你將丟失所有配置訊息。"},
    {TR_CONFIG_YOU_WILL_LOSE_YOUR_HOTKEYS, "你將丟失所有熱鍵訊息。"},
    {TR_CONFIG_TITLE, "Augustus 配置"},
    {TR_CONFIG_HEADER_GENERAL, "通用設置"},
    {TR_CONFIG_HEADER_UI_CHANGES, "用戶界面更變"},
    {TR_CONFIG_HEADER_GAMEPLAY_CHANGES, "游戲難度"},
    {TR_CONFIG_HEADER_CITY_MANAGEMENT_CHANGES, "城市管理"},
    {TR_CONFIG_LANGUAGE_LABEL, "語言包："},
    {TR_CONFIG_LANGUAGE_DEFAULT, "默認"},
    {TR_CONFIG_GAME_SPEED, "游戲速度："},
    {TR_CONFIG_VIDEO, "顯示設定"},
    {TR_CONFIG_FULLSCREEN, "全屏"},
    {TR_CONFIG_WINDOWED_RESOLUTION, "窗口化分辨率："},
    {TR_CONFIG_DISPLAY_SCALE, "顯示比例："},
    {TR_CONFIG_CURSOR_SCALE, "滑鼠比例："},
    {TR_CONFIG_USE_COLOR_CURSORS, "使用彩色滑鼠"},
    {TR_CONFIG_AUDIO, "聲音選項"},
    {TR_CONFIG_VOLUME, "音量"},
    {TR_CONFIG_ENABLE_AUDIO, "開啟聲音"},
    {TR_CONFIG_MUSIC, "開啟音樂"},
    {TR_CONFIG_SPEECH, "開啟語音"},
    {TR_CONFIG_EFFECTS, "開啟音效"},
    {TR_CONFIG_CITY_SOUNDS, "開啟城市音效"},
    {TR_CONFIG_VIDEO_SOUND, "開啟視頻音效"},
    {TR_CONFIG_SCROLL_SPEED, "滾動速度："},
    {TR_CONFIG_SHOW_INTRO_VIDEO, "播放開場動畫"},
    {TR_CONFIG_SIDEBAR_INFO, "控制面板更多資訊"},
    {TR_CONFIG_SMOOTH_SCROLLING, "平滑視角滾動"},
    {TR_CONFIG_DISABLE_MOUSE_EDGE_SCROLLING, "禁用屏幕邊緣滾動地圖"},
    {TR_CONFIG_DISABLE_RIGHT_CLICK_MAP_DRAG, "禁用滑鼠右鍵拖動視角"},
    {TR_CONFIG_VISUAL_FEEDBACK_ON_DELETE, "優化清理土地視覺效果"},
    {TR_CONFIG_ALLOW_CYCLING_TEMPLES, "允許連續建造每種神廟"},
    {TR_CONFIG_SHOW_WATER_STRUCTURE_RANGE, "建造時顯示貯水池水泉及水井覆蓋范圍"},
    {TR_CONFIG_SHOW_CONSTRUCTION_SIZE, "顯示拖動建設大小"},
    {TR_CONFIG_HIGHLIGHT_LEGIONS, "滑鼠懸停時高亮軍團"},
    {TR_CONFIG_ROTATE_MANUALLY, "熱鍵可旋轉門房與勝利拱門建造方向"},
    {TR_CONFIG_SHOW_MILITARY_SIDEBAR, "顯示軍隊資訊側欄"},
    {TR_CONFIG_FIX_IMMIGRATION_BUG, "修復非常困難不來人BUG"},
    {TR_CONFIG_FIX_100_YEAR_GHOSTS, "修復人口百歲仍占房BUG"},
    {TR_CONFIG_FIX_EDITOR_EVENTS, "修復自定義地圖中皇帝更位和生存時間BUG"},
    {TR_CONFIG_DRAW_WALKER_WAYPOINTS, "點擊建筑后覆層上勾畫出行人路徑點"},
    {TR_CONFIG_ENABLE_ZOOM, "開啟城市縮放功能（游戲速度可能減慢，需要更多運行內存和顯存）"},
    {TR_CONFIG_COMPLETE_RATING_COLUMNS, "修復低目標下無法完成評比任務BUG"},
    {TR_CONFIG_GRANDFESTIVAL, "舉辦豪華節慶獲取神靈更多祝福"},
    {TR_CONFIG_DIFFICULTY, "游戲難度："},
    {TR_CONFIG_GODS_EFFECTS, "開啟神靈詛咒/祝福效果"},
    {TR_CONFIG_JEALOUS_GODS, "關閉神靈嫉妒效果"},
    {TR_CONFIG_GLOBAL_LABOUR, "開啟全圖招工"},
    {TR_CONFIG_SCHOOL_WALKERS, "擴大學校行人路徑范圍"},
    {TR_CONFIG_RETIRE_AT_60, "市民退休年齡從50延長至60歲"},
    {TR_CONFIG_FIXED_WORKERS, "固定38%的勞工人口"},
    {TR_CONFIG_EXTRA_FORTS, "允許建造4個額外要塞"},
    {TR_CONFIG_WOLVES_BLOCK, "禁止在狼群周邊建造"},
    {TR_CONFIG_DYNAMIC_GRANARIES, "禁止未連接主干的谷倉道路"},
    {TR_CONFIG_MORE_STOCKPILE, "住宅可存放更多市場物資"},
    {TR_CONFIG_NO_SUPPLIER_DISTRIBUTION, "取貨的市場大媽不分發物資"},
    {TR_CONFIG_IMMEDIATELY_DELETE_BUILDINGS, "秒刪建筑"},
    {TR_CONFIG_GETTING_GRANARIES_GO_OFFROAD, "取得貨物的谷倉推車工可不走道路運輸"},
    {TR_CONFIG_GRANARIES_GET_DOUBLE, "取得貨物的谷倉推車工可攜運容量翻倍"},
    {TR_CONFIG_ALLOW_EXPORTING_FROM_GRANARIES, "允許商人從谷倉中出口食物"},
    {TR_CONFIG_TOWER_SENTRIES_GO_OFFROAD, "軍營無需道路連接至高塔即可提供守衛"},
    {TR_CONFIG_FARMS_DELIVER_CLOSE, "農場和埠頭僅向臨近谷倉輸送食物"},
    {TR_CONFIG_DELIVER_ONLY_TO_ACCEPTING_GRANARIES, "新產食物不會運至取得貨物的谷倉"},
    {TR_CONFIG_ALL_HOUSES_MERGE, "所有住宅都會合并"},
    {TR_CONFIG_WINE_COUNTS_IF_OPEN_TRADE_ROUTE, "開啟貿易路線算作提供不同種類葡萄酒"},
    {TR_CONFIG_RANDOM_COLLAPSES_TAKE_MONEY, "隨機倒塌的黏土坑和鐵礦只需耗費金錢自動重建"},
    {TR_CONFIG_MULTIPLE_BARRACKS, "允許建造多個軍營" },
    {TR_CONFIG_NOT_ACCEPTING_WAREHOUSES, "倉庫和谷倉建造后不接受任何物資"},
    {TR_CONFIG_HOUSES_DONT_EXPAND_INTO_GARDENS, "住宅擴建不會覆蓋到花園"},
    {TR_CONFIG_ROAMERS_DONT_SKIP_CORNERS, "間接目標行人不會跳過角落"},
    {TR_CONFIG_ZOOM_COULD_NOT_BE_ENABLED_TITLE, "無法開啟城市縮放功能"},
    {TR_CONFIG_ZOOM_COULD_NOT_BE_ENABLED_MESSAGE,
        "你的系統沒有足夠的顯存來開啟城市縮放功能。\n"
        "未開啟城市縮放功能。"},
    {TR_CONFIG_SHOW_MAX_POSSIBLE_PROSPERITY, "顯示當前住房可獲得的最高繁榮評比"},
    {TR_HOTKEY_TITLE, "Augustus 熱鍵配置"},
    {TR_HOTKEY_LABEL, "熱鍵"},
    {TR_HOTKEY_ALTERNATIVE_LABEL, "可替代鍵"},
    {TR_HOTKEY_HEADER_ARROWS, "方向鍵"},
    {TR_HOTKEY_HEADER_GLOBAL, "全局熱鍵"},
    {TR_HOTKEY_HEADER_CITY, "城市熱鍵"},
    {TR_HOTKEY_HEADER_ADVISORS, "顧問"},
    {TR_HOTKEY_HEADER_OVERLAYS, "覆層"},
    {TR_HOTKEY_HEADER_BOOKMARKS, "城市地圖視角標簽"},
    {TR_HOTKEY_HEADER_EDITOR, "編輯器"},
    {TR_HOTKEY_HEADER_BUILD, "建造熱鍵"},
    {TR_HOTKEY_ARROW_UP, "上"},
    {TR_HOTKEY_ARROW_DOWN, "下"},
    {TR_HOTKEY_ARROW_LEFT, "左"},
    {TR_HOTKEY_ARROW_RIGHT, "右"},
    {TR_HOTKEY_TOGGLE_FULLSCREEN, "全屏切換"},
    {TR_HOTKEY_CENTER_WINDOW, "中心窗口化"},
    {TR_HOTKEY_RESIZE_TO_640, "重置分辨率至640x480"},
    {TR_HOTKEY_RESIZE_TO_800, "重置分辨率至800x600"},
    {TR_HOTKEY_RESIZE_TO_1024, "重置分辨率至1024x768"},
    {TR_HOTKEY_SAVE_SCREENSHOT, "保存截圖"},
    {TR_HOTKEY_SAVE_CITY_SCREENSHOT, "保存城市全景截圖"},
    {TR_HOTKEY_BUILD_CLONE, "復制滑鼠下的建筑"},
    {TR_HOTKEY_LOAD_FILE, "載入文件"},
    {TR_HOTKEY_SAVE_FILE, "保存文件"},
    {TR_HOTKEY_INCREASE_GAME_SPEED, "加快游戲速度"},
    {TR_HOTKEY_DECREASE_GAME_SPEED, "減慢游戲速度"},
    {TR_HOTKEY_TOGGLE_PAUSE, "暫停切換"},
    {TR_HOTKEY_CYCLE_LEGION, "切換各軍團所在視角"},
    {TR_HOTKEY_ROTATE_MAP_LEFT, "順時針旋轉地圖視角"},
    {TR_HOTKEY_ROTATE_MAP_RIGHT, "逆時針旋轉地圖視角"},
    {TR_HOTKEY_SHOW_ADVISOR_LABOR, "勞工顧問"},
    {TR_HOTKEY_SHOW_ADVISOR_MILITARY, "軍事顧問"},
    {TR_HOTKEY_SHOW_ADVISOR_IMPERIAL, "皇帝顧問"},
    {TR_HOTKEY_SHOW_ADVISOR_RATINGS, "評比顧問"},
    {TR_HOTKEY_SHOW_ADVISOR_TRADE, "貿易顧問"},
    {TR_HOTKEY_SHOW_ADVISOR_POPULATION, "人口顧問"},
    {TR_HOTKEY_SHOW_ADVISOR_HEALTH, "健康顧問"},
    {TR_HOTKEY_SHOW_ADVISOR_EDUCATION, "教育顧問"},
    {TR_HOTKEY_SHOW_ADVISOR_ENTERTAINMENT, "娛樂顧問"},
    {TR_HOTKEY_SHOW_ADVISOR_RELIGION, "宗教顧問"},
    {TR_HOTKEY_SHOW_ADVISOR_FINANCIAL, "財政顧問"},
    {TR_HOTKEY_SHOW_ADVISOR_CHIEF, "首席顧問"},
    {TR_HOTKEY_SHOW_ADVISOR_HOUSING, "住房顧問"},
    {TR_HOTKEY_TOGGLE_OVERLAY, "當前覆層視角切換"},
    {TR_HOTKEY_SHOW_OVERLAY_RELATIVE, "顯示相關覆層"},
    {TR_HOTKEY_SHOW_OVERLAY_WATER, "顯示供水覆層"},
    {TR_HOTKEY_SHOW_OVERLAY_FIRE, "顯示火災覆層"},
    {TR_HOTKEY_SHOW_OVERLAY_DAMAGE, "顯示損壞覆層"},
    {TR_HOTKEY_SHOW_OVERLAY_CRIME, "顯示犯罪覆層"},
    {TR_HOTKEY_ROTATE_BUILDING, "建造正轉向"},
    {TR_HOTKEY_ROTATE_BUILDING_BACK, "建造逆轉向"},
    {TR_HOTKEY_SHOW_OVERLAY_PROBLEMS, "顯示問題覆層"},
    {TR_HOTKEY_GO_TO_BOOKMARK_1, "前往視角標簽 1"},
    {TR_HOTKEY_GO_TO_BOOKMARK_2, "前往視角標簽 2"},
    {TR_HOTKEY_GO_TO_BOOKMARK_3, "前往視角標簽 3"},
    {TR_HOTKEY_GO_TO_BOOKMARK_4, "前往視角標簽 4"},
    {TR_HOTKEY_SET_BOOKMARK_1, "設定視角標簽 1"},
    {TR_HOTKEY_SET_BOOKMARK_2, "設定視角標簽 2"},
    {TR_HOTKEY_SET_BOOKMARK_3, "設定視角標簽 3"},
    {TR_HOTKEY_SET_BOOKMARK_4, "設定視角標簽 4"},
    {TR_HOTKEY_EDITOR_TOGGLE_BATTLE_INFO, "戰斗資訊切換"},
    {TR_HOTKEY_EDIT_TITLE, "按下新熱鍵"},
    {TR_BUILDING_ROADBLOCK, "路   障"},
    {TR_BUILDING_ROADBLOCK_DESC, "路障可攔住四處游走的行人。"},
    {TR_BUILDING_ARCHITECT_GUILD, "建筑師協會" },
    {TR_BUILDING_ARCHITECT_GUILD_DESC, "建筑師們在此不辭辛勞地為羅馬的榮耀建造紀念碑。" },
    {TR_BUILDING_GRAND_TEMPLE_ADD_MODULE, "授予神廟名號" },
    {TR_BUILDING_GRAND_TEMPLE_CERES, "塞麗絲（農業之神）" },
    {TR_BUILDING_GRAND_TEMPLE_NEPTUNE, "納普敦（海洋之神）" },
    {TR_BUILDING_GRAND_TEMPLE_MERCURY, "莫丘里（商業之神）" },
    {TR_BUILDING_GRAND_TEMPLE_MARS, "馬爾斯（戰爭之神）" },
    {TR_BUILDING_GRAND_TEMPLE_VENUS, "維納斯（愛情之神）" },
    {TR_BUILDING_PANTHEON, "萬 神 殿" },
    {TR_BUILDING_GRAND_TEMPLE_CERES_DESC, "塞麗絲宏偉神廟（農業之神）" },
    {TR_BUILDING_GRAND_TEMPLE_NEPTUNE_DESC, "納普敦宏偉神廟（海洋之神）" },
    {TR_BUILDING_GRAND_TEMPLE_MERCURY_DESC, "莫丘里宏偉神廟（商業之神）" },
    {TR_BUILDING_GRAND_TEMPLE_MARS_DESC, "馬爾斯宏偉神廟（戰爭之神）" },
    {TR_BUILDING_GRAND_TEMPLE_VENUS_DESC, "維納斯宏偉神廟（愛情之神）" },
    {TR_BUILDING_PANTHEON_DESC, "萬神殿" },
    {TR_BUILDING_GRAND_TEMPLE_CERES_BONUS_DESC, "農場推車工移動速度提高50%。" },
    {TR_BUILDING_GRAND_TEMPLE_NEPTUNE_BONUS_DESC, "水泉和水井范圍+1，貯水池范圍+2，供水服務所需勞工降低50%，商船只行駛速度提高25%。" },
    {TR_BUILDING_GRAND_TEMPLE_MERCURY_BONUS_DESC, "陸地商人和海上商船攜貨量提高50%，陸地商人移動速度提高25%。" },
    {TR_BUILDING_GRAND_TEMPLE_MARS_BONUS_DESC, "可像軍營一樣訓練士兵，可建造額外四座要塞。" },
    {TR_BUILDING_GRAND_TEMPLE_VENUS_BONUS_DESC, "城市花園、雕像及神廟隨著美化程度的提高而發光，并賦予更高地區滿意度。住宅可存放更多物資且退化速度更慢。" },
    {TR_BUILDING_PANTHEON_BONUS_DESC, "可供奉所有神靈，且能舉行年度節慶，宗教和文化建筑維護費降低25%。" }, // Note to translators: This one changed from the previous version, please update.
    {TR_BUILDING_GRAND_TEMPLE_CERES_MODULE_1_DESC, "祭司能降低食物消耗20%。" },
    {TR_BUILDING_GRAND_TEMPLE_NEPTUNE_MODULE_1_DESC, "神廟可為馬術場生產戰車。" },
    {TR_BUILDING_GRAND_TEMPLE_MERCURY_MODULE_1_DESC, "祭司能降低陶器和家具消耗20%。" },
    {TR_BUILDING_GRAND_TEMPLE_MARS_MODULE_1_DESC, "神廟可根據自己覆蓋的住宅數量生產相應數量的食物，并運送至軍糧站。要塞維護費降低25%。" }, // Note to translators: This one changed from the previous version, please update.
    {TR_BUILDING_GRAND_TEMPLE_VENUS_MODULE_1_DESC, "宏偉神廟可根據接觸到維納斯神廟的城市人口量生產出相應數量的葡萄酒，神廟中生產存放好的葡萄酒經祭司分發至住宅和酒館。" },
    {TR_BUILDING_GRAND_TEMPLE_CERES_MODULE_2_DESC, "祭司能從城市儲備中取貨一種當地自產食物和橄欖油并分發至住宅。" },
    {TR_BUILDING_GRAND_TEMPLE_NEPTUNE_MODULE_2_DESC, "祭司能擴大住房容量5%，宏偉神廟可像注滿水的貯水池一樣供水。" },
    {TR_BUILDING_GRAND_TEMPLE_MERCURY_MODULE_2_DESC, "祭司能降低橄欖油和葡萄酒消耗20%。" },
    {TR_BUILDING_GRAND_TEMPLE_MARS_MODULE_2_DESC, "祭司能降低所有物資消耗10%。" },
    {TR_BUILDING_GRAND_TEMPLE_VENUS_MODULE_2_DESC, "祭司能美化住房并提供娛樂，且無需裝飾即可提高周邊滿意度。" },
    {TR_BUILDING_PANTHEON_MODULE_1_DESC, "城市中所有神廟為萬神殿派出祭司，傳播信仰和神靈的祝福。" },
    {TR_BUILDING_PANTHEON_MODULE_2_DESC, "接觸到萬神殿的住宅可升一級。" },
    {TR_BUILDING_GRAND_TEMPLE_CERES_DESC_MODULE_1, "塞麗絲神廟-沃土" }, // Note to translators: This one changed from the previous version, please update.
    {TR_BUILDING_GRAND_TEMPLE_CERES_DESC_MODULE_2, "塞麗絲神廟-雄市" }, // Note to translators: This one changed from the previous version, please update.
    {TR_BUILDING_GRAND_TEMPLE_NEPTUNE_DESC_MODULE_1, "納普敦神廟-戰車" },
    {TR_BUILDING_GRAND_TEMPLE_NEPTUNE_DESC_MODULE_2, "納普敦神廟-安居" },
    {TR_BUILDING_GRAND_TEMPLE_MERCURY_DESC_MODULE_1, "莫丘里神廟-天享" },
    {TR_BUILDING_GRAND_TEMPLE_MERCURY_DESC_MODULE_2, "莫丘里神廟-地用" }, // Note to translators: This one changed from the previous versions, please update.
    {TR_BUILDING_GRAND_TEMPLE_MARS_DESC_MODULE_1, "馬爾斯神廟-糧草" },
    {TR_BUILDING_GRAND_TEMPLE_MARS_DESC_MODULE_2, "馬爾斯神廟-節用" },
    {TR_BUILDING_GRAND_TEMPLE_VENUS_DESC_MODULE_1, "維納斯神廟-暢飲" },
    {TR_BUILDING_GRAND_TEMPLE_VENUS_DESC_MODULE_2, "維納斯神廟-宜居" },
    {TR_BUILDING_PANTHEON_DESC_MODULE_1, "萬神殿-朝宗" },
    {TR_BUILDING_PANTHEON_DESC_MODULE_2, "萬神殿-不朽" },
    {TR_BUILDING_GRAND_TEMPLE_MENU, "宏偉神廟" },
    {TR_BUILDING_WORK_CAMP, "勞 工 營" },
    {TR_BUILDING_WORK_CAMP_DESC, "勞工在此聚集并將建材運至建筑工地。" },
    {TR_HEADER_HOUSING, "住房"},
    {TR_ADVISOR_HOUSING_ROOM, "城市空房可供"},
    {TR_ADVISOR_HOUSING_NO_ROOM, "城市已無空房。"},
    {TR_ADVISOR_RESIDENCES_DEMANDING_POTTERY, "需要陶器的住宅："},
    {TR_ADVISOR_RESIDENCES_DEMANDING_FURNITURE, "需要家具的住宅："},
    {TR_ADVISOR_RESIDENCES_DEMANDING_OIL, "需要橄欖油的住宅："},
    {TR_ADVISOR_RESIDENCES_DEMANDING_WINE, "需要葡萄酒的住宅："},
    {TR_ADVISOR_TOTAL_NUM_HOUSES, "住宅總數："},
    {TR_ADVISOR_AVAILABLE_HOUSING_CAPACITY, "空房容量："},
    {TR_ADVISOR_TOTAL_HOUSING_CAPACITY, "總容量："},
    {TR_ADVISOR_ADVISOR_HEADER_HOUSING, "人口-住房"},
    {TR_ADVISOR_BUTTON_GRAPHS, "曲線圖"},
    {TR_ADVISOR_HOUSING_PROSPERITY_RATING, "住房繁榮評比："},
    {TR_ADVISOR_PERCENTAGE_IN_VILLAS_PALACES, "居住在華宅和宮殿的人口比例："},
    {TR_ADVISOR_PERCENTAGE_IN_TENTS_SHACKS, "居住在帳篷和木屋的人口比例："},
    {TR_ADVISOR_AVERAGE_TAX, "每戶平均繳稅："},
    {TR_ADVISOR_AVERAGE_AGE, "人口平均年齡："},
    {TR_ADVISOR_PERCENT_IN_WORKFORCE, "勞動力占總人口比例："},
    {TR_ADVISOR_BIRTHS_LAST_YEAR, "去年新生人口："},
    {TR_ADVISOR_DEATHS_LAST_YEAR, "去年死亡人口："},
    {TR_ADVISOR_TOTAL_POPULATION, "位居民（總人口）"},
    {TR_ADVISOR_IN_STORAGE, "在倉庫"},
    {TR_ADVISOR_FROM_GRANARIES, "在谷倉"},
    {TR_ADVISOR_DISPATCHING_FOOD_FROM_GRANARIES_TITLE, "從谷倉中分發食物"},
    {TR_ADVISOR_DISPATCHING_FOOD_FROM_GRANARIES_TEXT, "你確定嗎?"},
    {TR_ADVISOR_KEEP_STOCKPILING, "保持囤積資源"},
    {TR_ADVISOR_MAX_ATTAINABLE_PROSPERITY_IS, "當前住房水平的最高繁榮值："},
    {TR_REQUIRED_RESOURCES, "當前階段所需資源："},
    {TR_BUILDING_GRAND_TEMPLE_CONSTRUCTION_DESC, "建造宏偉神廟需要倉庫提供建材、勞工營提供勞動力、建筑師協會提供師傅。"},
    {TR_CONSTRUCTION_PHASE, "建造階段："},
    {TR_ADD_MODULE, "開光授名神廟?"},
    {TR_BUILDING_TEMPLE_MODULE_CONSTRUCTED, "名號已授予。"},
    {TR_BUILDING_CERES_TEMPLE_QUOTE, "起初塞麗絲教會了勤勞的母鹿如何犁地\n懷孕的大地胎動著即將落地的種子。\n她首先為人提供了健康的食物\n然后為這罪惡的世界帶來了法律。\n-奧維德-《變形記》-第五卷" },
    {TR_BUILDING_NEPTUNE_TEMPLE_QUOTE, "隨著納普敦放下他的權杖，巨浪滾滾落下\n在波濤洶涌的海面上，浪花撫平他那布滿皺紋的臉。\n特里同已在他的召喚下出現在海面上;\n他身著一件提爾長袍;\n同時手里拿著一把彎曲的小號。\n-奧維德-《變形記》-第一卷"},
    {TR_BUILDING_MERCURY_TEMPLE_QUOTE, "牛和肥羊都可以劫走,\n三角桌可以出掉，茶色頭的駿馬也能賣個好價錢。\n但人的生命和呼吸是回不來的-\n強盜不會太多，貿易買不回性命，\n一旦從那咬緊牙關的人嘴里流出，什么都沒了。\n-荷馬-《伊利亞特》-第九卷"},
    {TR_BUILDING_MARS_TEMPLE_QUOTE, "馬爾斯喜歡的是，修好他受損的車輪和愚鈍的戰爭，\n然后手握擦亮的武器再次奔赴前線，\n用響亮的角聲喚醒這散漫的戰爭。\n-維吉爾-《埃涅阿斯紀》-第八卷" },
    {TR_BUILDING_VENUS_TEMPLE_QUOTE, "長著翅膀的丘比特和他溫柔的母親維納斯，\n我可真幸福！\n激情的歷歷在目是那么地動人心弦！\n我的心在我躺著的地方徹底融化了！\n-奧維德-《變形記》-第九卷" },
    {TR_BUILDING_PANTHEON_QUOTE, "天堂之門打開了：朱庇特召喚眾神\n眾神在大廳里商議。\n他端坐著，遙遠地俯視著\n下面的土地、營帳與戰爭的走向，\n俯視著低等世界的一切。從始至終，\n那等級森嚴至高無上的元老院巍峨地屹立著。\n-維吉爾-《埃涅阿斯紀》-第十卷" },
    {TR_BUILDING_GRAND_TEMPLE_PHASE_1, "（基臺）"},
    {TR_BUILDING_GRAND_TEMPLE_PHASE_2, "（門廊）"},
    {TR_BUILDING_GRAND_TEMPLE_PHASE_3, "（內殿）"},
    {TR_BUILDING_GRAND_TEMPLE_PHASE_4, "（禁地）"},
    {TR_BUILDING_GRAND_TEMPLE_PHASE_5, "（開廟）"},
    {TR_BUILDING_GRAND_TEMPLE_PHASE_1_TEXT, "工人們正在建造抬高神廟的基臺，打牢堅實的地基，以經得起時間的風吹雨打。" },
    {TR_BUILDING_GRAND_TEMPLE_PHASE_2_TEXT, "工匠們正在制作門廊，它承載著神廟的祭壇。" },
    {TR_BUILDING_GRAND_TEMPLE_PHASE_3_TEXT, "工程師們正在建造內殿，它是神廟內部存放圣像的地方。" },
    {TR_BUILDING_GRAND_TEMPLE_PHASE_4_TEXT, "禁地的建造工作已開始，朝拜者們在戶外聚集，然后開展公共儀式。" },
    {TR_BUILDING_GRAND_TEMPLE_PHASE_5_TEXT, "隨著神廟的完工，祭司們聚集在一起進行開廟儀式。" },
    {TR_BUILDING_MENU_TREES, "樹   木" },
    {TR_BUILDING_MENU_PATHS, "林 蔭 道" },
    {TR_BUILDING_MENU_PARKS, "公   園"},
    {TR_BUILDING_SMALL_POND, "小 池 塘"},
    {TR_BUILDING_LARGE_POND, "大 池 塘"},
    {TR_BUILDING_PINE_TREE, "松   樹"},
    {TR_BUILDING_FIR_TREE, "冷 杉 樹" },
    {TR_BUILDING_OAK_TREE, "橡   樹" },
    {TR_BUILDING_ELM_TREE, "榆   樹" },
    {TR_BUILDING_FIG_TREE, "無花果樹" },
    {TR_BUILDING_PLUM_TREE, "李   樹" },
    {TR_BUILDING_PALM_TREE, "棕 櫚 樹" },
    {TR_BUILDING_DATE_TREE, "棗   樹" },
    {TR_BUILDING_PINE_PATH, "松樹小徑" },
    {TR_BUILDING_FIR_PATH, "冷杉小徑" },
    {TR_BUILDING_OAK_PATH, "橡樹小徑" },
    {TR_BUILDING_ELM_PATH, "榆樹小徑" },
    {TR_BUILDING_FIG_PATH, "無花果小徑" },
    {TR_BUILDING_PLUM_PATH, "李樹小徑" },
    {TR_BUILDING_PALM_PATH, "棕櫚小徑" },
    {TR_BUILDING_DATE_PATH, "棗樹小徑" },
    {TR_BUILDING_BLUE_PAVILION, "亭   臺" },
    {TR_BUILDING_RED_PAVILION, "紅色亭臺"},
    {TR_BUILDING_ORANGE_PAVILION, "橙色亭臺"},
    {TR_BUILDING_YELLOW_PAVILION, "黃色亭臺"},
    {TR_BUILDING_GREEN_PAVILION, "綠色亭臺" },
    {TR_BUILDING_SMALL_STATUE_ALT, "女神雕像" },
    {TR_BUILDING_SMALL_STATUE_ALT_B, "議員雕像"},
    {TR_BUILDING_OBELISK, "方 尖 碑" },
    {TR_BUILDING_POND_DESC, "池塘由城市的貯水池注滿水，為動植物提供水源，同時也是供市民乘涼休息的綠洲。人人都喜歡住在池塘邊上。" },
    {TR_BUILDING_WINDOW_POND, "池   塘"},
    {TR_BUILDING_OBELISK_DESC, "一座久負盛名的紀念碑，是一位古代尼羅河國王的作品，并非出自埃及人之手。"},
    {TR_ADVISOR_FINANCE_LEVIES, "建筑維護費"},
    {TR_CONFIRM_DELETE_MONUMENT, "拆除該奇觀"},
    {TR_SELECT_EPITHET_PROMPT_HEADER, "選擇授予名號"},
    {TR_SELECT_EPITHET_PROMPT_TEXT, "授予名號將使神廟永久投身于敬拜其神靈的某一特性，同時賦予祭司特定的神力。" },
    {TR_BUILDING_INFO_MONTHLY_LEVY, "/月"},
    {TR_BUILDING_MESS_HALL, "軍 糧 站"},
    {TR_BUILDING_MESS_HALL_DESC, "軍糧站從谷倉中取得食物，供養駐扎在要塞里的將士。軍糧不足將導致招兵變慢、士氣受損。"},
    {TR_BUILDING_MESS_HALL_FULFILLMENT, "上個月軍糧供應："},
    {TR_BUILDING_MESS_HALL_TROOP_HUNGER, "士兵狀態：" },
    {TR_BUILDING_MESS_HALL_TROOP_HUNGER_1, "吃好喝足" },
    {TR_BUILDING_MESS_HALL_TROOP_HUNGER_2, "勉強溫飽"},
    {TR_BUILDING_MESS_HALL_TROOP_HUNGER_3, "食不果腹"},
    {TR_BUILDING_MESS_HALL_TROOP_HUNGER_4, "饑火燒腸"},
    {TR_BUILDING_MESS_HALL_TROOP_HUNGER_5, "忍痛挨餓"},
    {TR_BUILDING_MESS_HALL_FOOD_TYPES_BONUS_1, "飲食多樣化大大提高了部隊的士氣。"},
    {TR_BUILDING_MESS_HALL_FOOD_TYPES_BONUS_2, "種類多樣、營養豐富的飲食大大提高了部隊的士氣。"},
    {TR_BUILDING_MESS_HALL_NO_SOLDIERS, "無軍隊需供養。" },
    {TR_BUILDING_MESS_HALL_MONTHS_FOOD_STORED, "已備軍糧可供時日（月）：" },
    {TR_MESS_HALL_SPECIAL_ORDERS_HEADER, "軍糧站指令"},
    {TR_BUILDING_BARRACKS_FOOD_WARNING, "食物短缺導致士兵招募速度下降。"},
    {TR_BUILDING_BARRACKS_FOOD_WARNING_2, "食物嚴重短缺大大拖慢了招兵速度。" },
    {TR_BUILDING_LEGION_FOOD_WARNING_1, "近期的食物短缺削弱了部隊士氣"},
    {TR_BUILDING_LEGION_FOOD_WARNING_2, "近期的食物短缺嚴重削弱了部隊士氣"},
    {TR_BUILDING_LEGION_STARVING, "食物短缺" },
    {TR_ADVISOR_TRADE_MAX, "最大量"},
    {TR_ADVISOR_TRADE_NO_LIMIT, "最大量"},
    {TR_ADVISOR_TRADE_IMPORTABLE, "可進口"},
    {TR_ADVISOR_TRADE_EXPORTABLE, "可出口"},
    {TR_ADVISOR_TRADE_NO_BUYERS, "該資源無買家"},
    {TR_ADVISOR_TRADE_NO_SELLERS, "該資源無賣家"},
    {TR_ADVISOR_TRADE_NOT_IMPORTING, "未進口"},
    {TR_ADVISOR_TRADE_NOT_EXPORTING, "未出口"},
    {TR_ADVISOR_OPEN_TO_IMPORT, "開啟貿易路線以進口"},
    {TR_ADVISOR_OPEN_TO_EXPORT, "開啟貿易路線以出口"},
    {TR_ADVISOR_LEGION_FOOD_SATISFIED, "士兵所需食物充足。"},
    {TR_ADVISOR_LEGION_FOOD_NEEDED, "士兵需要更多食物。" },
    {TR_ADVISOR_LEGION_FOOD_CRITICAL, "你的士兵正在挨餓！"},
    {TR_ADVISOR_LEGION_MONTHS_FOOD_STORED, "軍糧站已備軍糧可供時日（月）：" },
    {TR_CITY_MESSAGE_TITLE_MESS_HALL_NEEDS_FOOD, "忍饑受餓的士兵" },
    {TR_CITY_MESSAGE_TEXT_MESS_HALL_NEEDS_FOOD, "你的軍糧站食物嚴重短缺，軍隊士氣正在衰弱。請確保你的谷倉儲備良好，軍糧站能及時取糧。"},
    {TR_CITY_MESSAGE_TEXT_MESS_HALL_MISSING, "城市軍糧站被毀，軍隊正在挨餓，請立刻建造一處軍糧站。"},
    {TR_MARKET_SPECIAL_ORDERS_HEADER, "市場指令"},
    {TR_DOCK_SPECIAL_ORDERS_HEADER, "碼頭指令"},
    {TR_TEMPLE_SPECIAL_ORDERS_HEADER, "神廟指令"},
    {TR_TAVERN_SPECIAL_ORDERS_HEADER, "酒館指令"},
    {TR_WARNING_NO_MESS_HALL, "你必須先建造軍糧站供養士兵。" },
    {TR_WARNING_MAX_GRAND_TEMPLES, "只能建造兩座宏偉神廟。" },
    {TR_CITY_MESSAGE_TITLE_GRAND_TEMPLE_COMPLETE, "宏偉神廟建造完成"},
    {TR_CITY_MESSAGE_TEXT_GRAND_TEMPLE_COMPLETE, "祭司和朝拜者們聚眾在宏偉神廟舉行獻祭儀式，你沐浴在神的眷顧下，市民對你的豐功偉績深感敬畏。"},
    {TR_CITY_MESSAGE_TITLE_MERCURY_BLESSING, "莫丘里的祝福" },
    {TR_CITY_MESSAGE_TEXT_MERCURY_BLESSING, "莫丘里對你的虔誠感到欣慰，他祝福了你的工業鏈，為你的工匠挖出了生產原料。"},
    {TR_FIGURE_TYPE_WORK_CAMP_WORKER, "工頭"},
    {TR_FIGURE_TYPE_WORK_CAMP_SLAVE, "搬運工"},
    {TR_FIGURE_TYPE_WORK_CAMP_ARCHITECT, "建筑師"},
    {TR_FIGURE_TYPE_MESS_HALL_SUPPLIER, "軍需官"},
    {TR_FIGURE_TYPE_MESS_HALL_COLLECTOR, "炊事員"},
    {TR_BUILDING_CERES_TEMPLE_MODULE_DESC, "宏偉神廟將塞麗絲神廟的祭司組織起來向饑餓的市民分發食物和橄欖油。"},
    {TR_BUILDING_VENUS_TEMPLE_MODULE_DESC, "宏偉神廟可讓維納斯神廟的祭司向市民供應神圣的葡萄酒。" },
    {TR_BUILDING_MARS_TEMPLE_MODULE_DESC, "宏偉神廟將馬爾斯神廟的祭司組織起來為軍糧站提供食物。"},
    {TR_BUILDING_SMALL_TEMPLE_CERES_NAME, "塞麗絲神廟"},
    {TR_BUILDING_SMALL_TEMPLE_NEPTUNE_NAME, "納普敦神廟"},
    {TR_BUILDING_SMALL_TEMPLE_MERCURY_NAME, "莫丘里神廟"},
    {TR_BUILDING_SMALL_TEMPLE_MARS_NAME, "馬爾斯神廟"},
    {TR_BUILDING_SMALL_TEMPLE_VENUS_NAME, "維納斯神廟"},
    {TR_FIGURE_TYPE_PRIEST_SUPPLIER, "祭司" },
    {TR_BUILDING_DOCK_CITIES_CONFIG_DESC, "海上貿易城市：" },
    {TR_BUILDING_DOCK_CITIES_NO_ROUTES, "無海上貿易路線。" },
    // Transcription of mission_exact4.wav
    {TR_PHRASE_FIGURE_MISSIONARY_EXACT_4, "\"我將盡我所能讓這些野蠻人冷靜下來，我相信和他們交談后，他們就不會再來侵擾城市了。\"" },
    {TR_CITY_MESSAGE_TITLE_PANTHEON_FESTIVAL, "年度節慶"},
    {TR_CITY_MESSAGE_TEXT_PANTHEON_FESTIVAL_CERES, "虔誠的信徒們在神廟前匯聚一堂，慶祝谷神節以表達對塞麗絲女神的敬意，農民們紛紛舉行祭祀活動以求得來年的豐收。"},
    {TR_CITY_MESSAGE_TEXT_PANTHEON_FESTIVAL_NEPTUNE, "今天是紀念納普敦的海神節，忠實的市民們用樹枝和樹葉建造小屋，在炎炎的烈日下盡情狂歡。"},
    {TR_CITY_MESSAGE_TEXT_PANTHEON_FESTIVAL_MERCURY, "商人們成群結隊地前來慶祝商神節。人們將圣水灑向商船和倉庫，希望獲得莫丘里的保佑。"},
    {TR_CITY_MESSAGE_TEXT_PANTHEON_FESTIVAL_MARS, "市民們聚集在城外慶祝賽馬節，以尋求馬爾斯的青睞，賽場的空氣中充滿著雷鳴般的馬蹄聲和戰車的嘎吱聲，這是對戰神忠誠的致敬。"},
    {TR_CITY_MESSAGE_TEXT_PANTHEON_FESTIVAL_VENUS, "忠實的市民們聚集在一起慶祝愛神節，向神圣的維納斯致敬，祈求在愛情上獲得好運。"},
    {TR_TOOLTIP_BUTTON_DELETE_READ_MESSAGES, "刪除已讀資訊"},
    {TR_TOOLTIP_BUTTON_MOTHBALL_ON, "關停該建筑"},
    {TR_TOOLTIP_BUTTON_MOTHBALL_OFF, "開啟該建筑"},
    {TR_TOOLTIP_BUTTON_ACCEPT_MARKET_LADIES, "允許市場大媽在此收購物資"},
    {TR_TOOLTIP_BUTTON_ACCEPT_TRADE_CARAVAN, "允許商隊在此貿易"},
    {TR_TOOLTIP_BUTTON_ACCEPT_TRADE_SHIPS, "允許商船在此貿易"},
    {TR_BUILDING_LIGHTHOUSE, "燈   塔"},
    {TR_BUILDING_CARAVANSERAI, "商隊驛站"},
    {TR_BUILDING_CARAVANSERAI_DESC, "商隊在此歇腳休息、喂食坐騎。陸地貿易歇腳時間占總時間的一半。"},
    {TR_CARAVANSERAI_SPECIAL_ORDERS_HEADER, "商隊驛站指令"},
    {TR_BUILDING_CARAVANSERAI_PHASE_1, "（地基）"},
    {TR_BUILDING_CARAVANSERAI_PHASE_1_TEXT, "工程師為商站打下堅實的地基。"},
    {TR_FIGURE_TYPE_CARAVANSERAI_SUPPLIER, "商隊驛站供應人" },
    {TR_FIGURE_TYPE_CARAVANSERAI_COLLECTOR, "商隊驛站收購工" },
    {TR_BUILDING_CARAVANSERAI_POLICY_TITLE, "陸地貿易政策" },
    {TR_BUILDING_CARAVANSERAI_NO_POLICY, "無陸地貿易政策。" },
    {TR_BUILDING_CARAVANSERAI_POLICY_TEXT, "為城市選擇一項陸地貿易政策，你也可以隨時切換成其他政策。" },
    {TR_BUILDING_CARAVANSERAI_POLICY_1_TITLE, "陸地出口導向型" },
    {TR_BUILDING_CARAVANSERAI_POLICY_1, "陸地出口導向型政策：陸地出口收入提高20%，但陸地進口價格上漲10%。" },
    {TR_BUILDING_CARAVANSERAI_POLICY_2_TITLE, "陸地進口導向型" },
    {TR_BUILDING_CARAVANSERAI_POLICY_2, "陸地進口導向型政策：陸地進口價格降低20%，但陸地出口收入下降10%。" },
    {TR_BUILDING_CARAVANSERAI_POLICY_3_TITLE, "陸地種類導向型" },
    {TR_BUILDING_CARAVANSERAI_POLICY_3, "陸地種類導向型政策：商隊攜帶額外4種貿易物資，但商隊驛站的食物消耗量增加20%。" },
    {TR_BUILDING_LIGHTHOUSE_PHASE_1, "（地基）" },
    {TR_BUILDING_LIGHTHOUSE_PHASE_2, "（基座）" },
    {TR_BUILDING_LIGHTHOUSE_PHASE_3, "（塔樓）" },
    {TR_BUILDING_LIGHTHOUSE_PHASE_4, "（頂冠）" },
    {TR_BUILDING_LIGHTHOUSE_PHASE_1_TEXT, "工程師們正在打下堅實的地基，以承得起巨型石塔的重量。" },
    {TR_BUILDING_LIGHTHOUSE_PHASE_2_TEXT, "石匠們正在建造基座，將燈塔的火焰抬高至地平線之上。" },
    {TR_BUILDING_LIGHTHOUSE_PHASE_3_TEXT, "石匠們正持續為這座高塔鋪設階梯，燈塔的高度與日俱增。" },
    {TR_BUILDING_LIGHTHOUSE_PHASE_4_TEXT, "工程師們正在對燈塔進行最后的修飾，不久后其光芒將照耀千里，指引遠方歸家的船只。" },
    {TR_BUILDING_LIGHTHOUSE_CONSTRUCTION_DESC, "建造燈塔需要倉庫提供建材、勞工營提供勞動力、建筑師協會提供師傅。" },
    {TR_BUILDING_LIGHTHOUSE_BONUS_DESC, "這座高塔頂部的光芒指引著來自世界各地的船只，以平安抵達您的城市。漁船移速提高10%，海上風暴持續時間減半。" },
    {TR_BUILDING_LIGHTHOUSE_POLICY_TITLE, "海上貿易政策" },
    {TR_BUILDING_LIGHTHOUSE_NO_POLICY, "無海上貿易政策。" },
    {TR_BUILDING_LIGHTHOUSE_POLICY_TEXT, "為城市選擇一項海上貿易政策，你也可以隨時切換成其他政策。" },
    {TR_BUILDING_LIGHTHOUSE_POLICY_1_TITLE, "海上出口導向型" },
    {TR_BUILDING_LIGHTHOUSE_POLICY_1, "海上出口導向型政策：海上出口收入提高20%，但海上進口價格上漲10%。" },
    {TR_BUILDING_LIGHTHOUSE_POLICY_2_TITLE, "海上進口導向型" },
    {TR_BUILDING_LIGHTHOUSE_POLICY_2, "海上進口導向型政策：海上進口價格降低20%，但海上出口收入下降10%。" },
    {TR_BUILDING_LIGHTHOUSE_POLICY_3_TITLE, "海上種類導向型" },
    {TR_BUILDING_LIGHTHOUSE_POLICY_3, "海上種類導向型政策：商船攜帶額外4種貿易物資，但燈塔的木材消耗量增加20%。" },
    {TR_EDITOR_ALLOWED_BUILDINGS_MONUMENTS, "奇觀"},
    {TR_CITY_MESSAGE_TEXT_LIGHTHOUSE_COMPLETE, "建成的燈塔；聳立在地平線上的一座巨型石塔，希望它的燈焰能指引海上的船只平安歸家，直到永遠。" },
    {TR_CITY_MESSAGE_TEXT_PANTHEON_COMPLETE, "萬神殿建造完成。這座奇觀建筑神圣不可侵犯，展現出羅馬眾神和其人民的偉大力量。" },
    {TR_CITY_MESSAGE_TITLE_MONUMENT_COMPLETE, "奇觀已建成" },
    {TR_CITY_MESSAGE_TITLE_NEPTUNE_BLESSING, "來自納普敦的祝福"},
    {TR_CITY_MESSAGE_TEXT_NEPTUNE_BLESSING, "鑒于你的虔誠，納普敦讓城市的商人在接下來的12個月里暢通無阻，在此期間出口收入提高50%。"},
    {TR_CITY_MESSAGE_TITLE_VENUS_BLESSING, "來自維納斯的祝福" },
    {TR_CITY_MESSAGE_TEXT_VENUS_BLESSING, "取悅于你的崇敬，維納斯祝福你的人民永葆青春、健康且幸福，提高了勞動力人口占比。在維納斯的微笑下城市的情緒得到改善。" },
    {TR_BUILDING_MENU_STATUES, "雕   像" },
    {TR_BUILDING_MENU_GOV_RES, "政府官的宅邸" },
    {TR_OVERLAY_ROADS, "道    路" },
    {TR_NO_EXTRA_ASSETS_TITLE, "無法加載額外資源" },
    {TR_NO_EXTRA_ASSETS_MESSAGE,
        "無法加載Augustus額外資源。\n"
        "原因可能是‘assets’目錄缺失或版本老舊。\n"
        "請確保‘assets’目錄下的文件為最新版，可從Github上下載。" },
    {TR_WARNING_WATER_NEEDED_FOR_LIGHTHOUSE, "燈塔必須臨水建立"},
    {TR_TOOLTIP_OVERLAY_PANTHEON_ACCESS, "該住宅可接觸到萬神殿的所有5位神靈"},
    {TR_BUILDING_LEGION_FOOD_BONUS, "充足的食物正在提高士氣" },
    {TR_BUILDING_LEGION_FOOD_STATUS, "食物狀態"},
    {TR_TOOLTIP_BUTTON_ACCEPT_QUARTERMASTER, "允許軍需官在此取得食物"},
    {TR_WARNING_RESOURCES_NOT_AVAILABLE, "你無法獲得所需材料"},
    {TR_CONFIG_GP_CH_MONUMENTS_BOOST_CULTURE_RATING, "已建成的宏偉神廟+6點文化評比"},
    {TR_CONFIG_GP_CH_DISABLE_INFINITE_WOLVES_SPAWNING, "禁止無限產生狼群"},
    {TR_BUTTON_BACK_TO_MAIN_MENU, "退出至主菜單" },
    {TR_LABEL_PAUSE_MENU, "暫停" },
    {TR_OVERLAY_LEVY, "維 護 費" },
    {TR_TOOLTIP_OVERLAY_LEVY, " Dn用于每月建筑維護。" },
    {TR_MAP_EDITOR_OPTIONS, "場景設定" },
    {TR_BUILDING_TAVERN, "酒   館" },
    {TR_BUILDING_GRAND_GARDEN, "宏偉花園" },
    {TR_BUILDING_TAVERN_DESC_1, "該酒館無工人前來運營，無法開張。" },
    {TR_BUILDING_TAVERN_DESC_2, "該酒館無葡萄酒供給，無法開張。" },
    {TR_BUILDING_TAVERN_DESC_3, "只要葡萄酒的供應源源不斷，該酒館就能持續為人民提供娛樂，再提供些肉類可進一步提高其吸引力。" },
    {TR_BUILDING_TAVERN_DESC_4, "醇厚的酒香和撲鼻的烤肉香味吸引了一大群顧客前來聚會、享樂和賭博。" },
    {TR_FIGURE_TYPE_BARKEEP, "酒館老板" },
    {TR_FIGURE_TYPE_BARKEEP_SUPPLIER, "酒館供應人" },
    {TR_OVERLAY_TAVERN, "酒    館" },
    {TR_TOOLTIP_OVERLAY_TAVERN_1, "該住宅未連接到酒館，或酒館的葡萄酒已用盡。" },
    {TR_TOOLTIP_OVERLAY_TAVERN_2, "酒館老板已有許久未路經此處，該住宅即將失去酒館的服務。" },
    {TR_TOOLTIP_OVERLAY_TAVERN_3, "帶著葡萄酒的酒館老板剛路經該住宅。" },
    {TR_TOOLTIP_OVERLAY_TAVERN_4, "帶著葡萄酒和肉類的酒館老板剛路經該住宅。" },
    {TR_TOOLTIP_OVERLAY_TAVERN_5, "帶著葡萄酒的酒館老板近期路經過該住宅。" },
    {TR_TOOLTIP_OVERLAY_TAVERN_6, "帶著葡萄酒和肉類的酒館老板近期路經過該住宅，讓歡樂的時光開始吧！" },
    {TR_BUILDING_WINDOW_HOUSE_SENTIMENT_1, "居民們討厭聽到你的名字。"},
    {TR_BUILDING_WINDOW_HOUSE_SENTIMENT_2, "居民們對你感到十分憤怒。" },
    {TR_BUILDING_WINDOW_HOUSE_SENTIMENT_3, "居民們對你表示憤怒。" },
    {TR_BUILDING_WINDOW_HOUSE_SENTIMENT_4, "居民們對你感到十分不滿。" },
    {TR_BUILDING_WINDOW_HOUSE_SENTIMENT_5, "居民們對你表示不滿。" },
    {TR_BUILDING_WINDOW_HOUSE_SENTIMENT_6, "居民們對你感到生氣。" },
    {TR_BUILDING_WINDOW_HOUSE_SENTIMENT_7, "居民們對你態度冷淡。" },
    {TR_BUILDING_WINDOW_HOUSE_SENTIMENT_8, "居民們對你感到滿意。" },
    {TR_BUILDING_WINDOW_HOUSE_SENTIMENT_9, "居民們對你感到比較滿意。" },
    {TR_BUILDING_WINDOW_HOUSE_SENTIMENT_10, "居民們對你感到十分滿意。" },
    {TR_BUILDING_WINDOW_HOUSE_SENTIMENT_11, "居民們對你表示熱愛。" },
    {TR_BUILDING_WINDOW_HOUSE_SENTIMENT_12, "居民們把你當作神一般崇拜。" },
    {TR_BUILDING_WINDOW_HOUSE_UPSET_HIGH_TAXES, "居民們對高稅負表示不滿。" },
    {TR_BUILDING_WINDOW_HOUSE_UPSET_LOW_WAGES, "居民們對低薪資表示不滿。" },
    {TR_BUILDING_WINDOW_HOUSE_UPSET_UNEMPLOYMENT, "居民們對缺少就業機會表示不滿。" },
    {TR_BUILDING_WINDOW_HOUSE_UPSET_SQUALOR, "居民們對質量欠佳的居住條件感到失望。" },
    {TR_BUILDING_WINDOW_HOUSE_SUGGEST_ENTERTAINMENT, "居民們希望得到更多娛樂。" },
    {TR_BUILDING_WINDOW_HOUSE_SUGGEST_FOOD, "居民們希望飲食更加多樣化。" },
    {TR_BUILDING_WINDOW_HOUSE_SUGGEST_DESIRABILITY, "居民們希望進一步提高住房滿意度。" },
    {TR_BUILDING_WINDOW_HOUSE_RECENT_EVENT_POSITIVE, "最近發生的事件令居民們振奮。" },
    {TR_BUILDING_WINDOW_HOUSE_RECENT_EVENT_NEGATIVE, "最近發生的事件使居民們失去信心。" },
    {TR_WINDOW_ADVISOR_ENTERTAINMENT_TAVERN_COVERAGE, "酒館" },
    {TR_WINDOW_ADVISOR_ENTERTAINMENT_GAMES_HEADER, "賽事" },
    {TR_WINDOW_ADVISOR_ENTERTAINMENT_GAMES_DESC, "贊助大競技場舉辦大型賽事，需消耗個人資金和城市資源。" },
    {TR_WINDOW_ADVISOR_ENTERTAINMENT_GAMES_BUTTON, "舉辦賽事" },
    {TR_WINDOW_ADVISOR_ENTERTAINMENT_GAMES_PREPARING, "大型賽事的準備工作正在如火如荼地進行中。" },
    {TR_SELECT_GAMES_HEADER, "賽事" },
    {TR_SELECT_GAMES_TEXT, "賽事將在大競技場內舉辦，需消耗個人資金和城市資源。" },
    {TR_BUILDING_ARENA, "競 技 場" },
    {TR_BUILDING_HORSE_STATUE, "騎手雕像" },
    {TR_BUILDING_DOLPHIN_FOUNTAIN, "海豚噴泉" },
    {TR_BUILDING_HEDGE_DARK, "翠綠樹籬" },
    {TR_BUILDING_HEDGE_LIGHT, "向日樹籬" },
    {TR_BUILDING_GARDEN_WALL, "花園圍墻" },
    {TR_BUILDING_LEGION_STATUE, "軍團雕像" },
    {TR_BUILDING_DECORATIVE_COLUMN, "大理石圓柱" },
    {TR_BUILDING_COLOSSEUM_PHASE_1, "（地窖）" },
    {TR_BUILDING_COLOSSEUM_PHASE_2, "（地基）" },
    {TR_BUILDING_COLOSSEUM_PHASE_3, "（拱廊）" },
    {TR_BUILDING_COLOSSEUM_PHASE_4, "（頂座）" },
    {TR_BUILDING_COLOSSEUM_PHASE_1_TEXT, "工人們正在挖掘地窖-地下通道和機械室，一切壯觀場面都始于此地。" },
    {TR_BUILDING_COLOSSEUM_PHASE_2_TEXT, "大競技場墻壁的形狀勾畫出層層梯位上羅馬平民和皇帝坐在一起的盛景。" },
    {TR_BUILDING_COLOSSEUM_PHASE_3_TEXT, "優雅美觀的拱廊高聳入云，把大競技場這個龐然建筑高高撐起。" },
    {TR_BUILDING_COLOSSEUM_PHASE_4_TEXT, "大競技場的頂座直入云霄，其墻壁上雕刻著閃閃發光的大理石雕像。" },
    {TR_BUILDING_HIPPODROME_PHASE_1, "（賽道）" },
    {TR_BUILDING_HIPPODROME_PHASE_2, "（地基）" },
    {TR_BUILDING_HIPPODROME_PHASE_3, "（看臺）" },
    {TR_BUILDING_HIPPODROME_PHASE_4, "（正門）" },
    {TR_BUILDING_HIPPODROME_PHASE_1_TEXT, "這片偌大的建造區正在被精心地勘察和清理，以開展一項規模前所未有的工程。" },
    {TR_BUILDING_HIPPODROME_PHASE_2_TEXT, "賽道周圍正在打造出入通道和支撐性建筑。" },
    {TR_BUILDING_HIPPODROME_PHASE_3_TEXT, "石匠們正在刻鑿可容下成千上萬名觀眾的石座。" },
    {TR_BUILDING_HIPPODROME_PHASE_4_TEXT, "除了賽道，工匠們正在打造一座配得上羅馬眾神和羅馬人民的紀念碑。" },
    {TR_BUILDING_MONUMENT_CONSTRUCTION_DESC, "建造奇觀需要倉庫提供建材、勞工營提供勞動力、建筑師協會提供師傅。" },
    {TR_BUILDING_MONUMENT_CONSTRUCTION_HALTED, "建造已暫停。" },
    {TR_WINDOW_ADVISOR_ENTERTAINMENT_ARENA_COVERAGE, "競技場" },
    {TR_BUILDING_INFO_TOURISM, "（旅行者）" },
    {TR_CITY_MESSAGE_TEXT_COLOSSEUM_COMPLETE, "大競技場建造完成！讓角斗大賽開始吧！" },
    {TR_CITY_MESSAGE_TEXT_HIPPODROME_COMPLETE, "賽馬場建造完成！市民觀眾們都等著你一聲令下讓比賽開始！" },
    {TR_CITY_MESSAGE_TITLE_COLOSSEUM_WORKING, "開幕賽" },
    {TR_CITY_MESSAGE_TITLE_HIPPODROME_WORKING, "比賽開始" },
    {TR_CITY_MESSAGE_TEXT_COLOSSEUM_WORKING, "大競技場的開幕賽已開始，在你的命令下，勇士們將用鮮血供奉這座奇觀！" },
    {TR_CITY_MESSAGE_TEXT_HIPPODROME_WORKING, "為了觀看戰車開幕賽，城內的人們一走而空，紛紛涌入城外的賽馬場。就在戰車大門打開的那一刻，震耳欲聾的歡呼聲響徹全場！" },
    {TR_WINDOW_GAMES_OPTION_1, "舉辦海軍演義" },
    {TR_WINDOW_GAMES_OPTION_2, "舉辦人獸廝殺" },
    {TR_WINDOW_GAMES_OPTION_3, "舉辦元旦慶祝賽" },
    {TR_WINDOW_GAMES_OPTION_4, "舉辦奧運會" },
    {TR_WINDOW_GAMES_OPTION_5, "舉辦羅馬運動會" },
    {TR_WINDOW_GAMES_OPTION_1_DESC, "大競技場將放水淹沒整個戰場，以再現一場大海戰。次年里己方軍隊移動速度和遠程戰斗力獲得提高。" },
    {TR_WINDOW_GAMES_OPTION_2_DESC, "大競技場將放出陰森恐怖的野獸，角斗士們要么獵殺野獸，要么被野獸當場處決，以取悅廣大羅馬民眾。次年里所有犯罪、暴動、起義活動將被鎮壓。角斗士將幫助城市免受下一次入侵。" },
    {TR_WINDOW_GAMES_OPTION_3_DESC, "大競技場將舉行的角斗賽事旨在新年的第一天里紀念羅馬眾神，祈求財富與商業繁榮。次年雙倍貿易限額。" },
    {TR_WINDOW_GAMES_OPTION_4_DESC, "賽馬場將舉辦奧運會，城市將招待大量游客，如果你的運動員在比賽中為城市贏得了榮譽，這將永久提高旅游業和支持度評比。" },
    {TR_WINDOW_GAMES_OPTION_5_DESC, "大競技場將舉辦羅馬運動會，舉國上下的選手來到你的城市參賽，如此盛景必定能提高皇帝和平民對你來年的支持度。" },
    {TR_WINDOW_GAMES_COST, "耗費：" },
    {TR_WINDOW_GAMES_PERSONAL_FUNDS, "（個人資金）" },
    {TR_WINDOW_GAMES_NOT_ENOUGH_FUNDS, "個人資金不足。" },
    {TR_WINDOW_GAMES_NOT_ENOUGH_RESOURCES, "資源儲備不足。" },
    {TR_WINDOW_GAMES_NO_VENUE, "舉辦場地未建造或場地無工人。" },
    {TR_BUILDING_SMALL_TEMPLE_PHASE_1, "（建設中）" },
    {TR_BUILDING_SMALL_TEMPLE_PHASE_1_TEXT, "石匠和工程師們正在用閃閃發光的大理石打造這座神圣的建筑。" },
    {TR_BUILDING_SMALL_TEMPLE_CONSTRUCTION_DESC, "建造該神廟需要倉庫提供建材、勞工營提供勞動力、建筑師協會提供師傅。" },
    {TR_CITY_MESSAGE_TEXT_NAVAL_GAMES_PLANNING, "貯水池中的水源正緩緩地注入大競技場內，木匠們開始組裝栩栩如生的微型戰船，專門為水上作戰而訓練的角斗士們也整裝待發，很快一場海戰即將打響！" },
    {TR_CITY_MESSAGE_TEXT_NAVAL_GAMES_STARTING, "大競技場變成了一片人工湖，角斗士們用生命重現昔日羅馬在海上的偉大勝利，隨著血液與海水的交融，觀眾們發出雷鳴般的歡呼。海軍演義已開始！" },
    {TR_CITY_MESSAGE_TEXT_NAVAL_GAMES_ENDING, "大競技場進入了一段漫長的清理過程，它的水需要被排干以待常用。在未來的幾年里你的人民將不斷訴說這段水上壯景。" },
    {TR_CITY_MESSAGE_TEXT_ANIMAL_GAMES_PLANNING, "奇奇怪怪的船貨開始出現在你的城市里，來自遙遠彼地的商人將兇殘可怖的野獸押送至大競技場地下的牢籠里。很快，一場慘烈壯觀的表演即將展現給在座的觀眾們。" },
    {TR_CITY_MESSAGE_TEXT_ANIMAL_GAMES_STARTING, "死刑犯和追尋終極榮耀的角斗士們將在這里以兇牙利爪的形式直面死亡。隨著一陣號角聲，人獸廝殺開始了！" },
    {TR_CITY_MESSAGE_TEXT_ANIMAL_GAMES_ENDING, "最后一只野獸已被殺死，它笨重地倒在大競技場的黃沙之中，野獸之血和斗士之血交織在了一起，這場人獸廝殺結束了。" },
    {TR_CITY_MESSAGE_TEXT_KALENDS_GAMES_PLANNING, "我們要以一場榮耀的角斗迎接新年、致敬眾神。作為回報，我們祈求財富、好運以及對商業的祝福。" },
    {TR_CITY_MESSAGE_TEXT_KALENDS_GAMES_STARTING, "元月初一的清晨，賽事開始了，祭司們用動物祭祀的同時，角斗士們披盔戴甲、浴血奮戰，為了尋求神靈的眷顧流盡最后一滴血。"},
    {TR_CITY_MESSAGE_TEXT_KALENDS_GAMES_ENDING, "占卜顯示我們偉大的賽事取悅了眾神，獻祭儀式也成功閉幕，我們預計今年將是財運亨通的一年！"},
    {TR_CITY_MESSAGE_TEXT_OLYMPIC_GAMES_PLANNING, "所有的目光都聚焦于我們的城市，奧林匹克運動會的舉辦將為城市帶來來自世界各地的游客、參賽選手以及羅馬政要人物。至少需要一年的時間才能做好一切準備工作。"},
    {TR_CITY_MESSAGE_TEXT_OLYMPIC_GAMES_STARTING, "這一天終于來了，奧運圣火已熊熊燃起，城市街道一走而空，賽馬場中人山人海，奧運賽事開始了！"},
    {TR_CITY_MESSAGE_TEXT_OLYMPIC_GAMES_ENDING, "圣火已熄滅，游客和市民們陸續離場，歸家途中大家雖感疲憊，卻依然沉浸在奧運的喜慶氛圍之中。永恒的榮耀屬于你，你成功舉辦了這場古老而神圣的慶典，它象征著凡人的力量與榮譽。" },
    {TR_CITY_MESSAGE_TEXT_IMPERIAL_GAMES_PLANNING, "隨著羅馬運動會的宣布，遠自帝國邊境的全國人民紛紛涌入你的城市，所有人都希望在接下來的比賽中取得優勝。至少需要一年的時間才能做好一切準備工作。" },
    {TR_CITY_MESSAGE_TEXT_IMPERIAL_GAMES_STARTING, "隨著儀仗隊莊嚴地走完，羅馬運動會開始了。愿朱庇特親自挑選賽跑、摔跤以及其他耐力和敏捷比賽的冠軍。大競技場擠得人滿為患，人們不僅盼著大賽開始，更望著按慣例分發的面包和橄欖油禮物。羅馬運動會開始了！" },
    {TR_CITY_MESSAGE_TEXT_IMPERIAL_GAMES_ENDING, "羅馬運動會的最后一天接近尾聲，每個市民吃飽喝足、意猶未盡，大家都嘗到了羅馬生活的美好與偉大。榮耀歸眾神，榮耀歸冠軍，榮耀歸父母官！" },
    {TR_CITY_MESSAGE_TITLE_GREAT_GAMES, "大型賽事"},
    {TR_WINDOW_ADVISOR_ENTERTAINMENT_GAMES_COOLDOWN, "距離可舉辦新賽事所剩月份：" },
    {TR_WINDOW_ADVISOR_ENTERTAINMENT_PREPARING_NG, "距離海軍演義開始所剩月份：" },
    {TR_WINDOW_ADVISOR_ENTERTAINMENT_UNDERWAY_NG, "海軍演義正在進行。在觀眾們的歡呼下，戰場上水花四濺，船板被踩得嘎吱作響，角斗士們發出的戰吼聲令人振奮不已。" },
    {TR_WINDOW_ADVISOR_ENTERTAINMENT_PREPARING_AG, "距離人獸廝殺開始所剩月份：" },
    {TR_WINDOW_ADVISOR_ENTERTAINMENT_UNDERWAY_AG, "人獸廝殺正在進行。就在野獸慢慢逼近它的人類獵物時，觀眾們肩并著肩，場內尖叫聲不絕于耳，有的人甚至喊破了嗓子。" },
    {TR_WINDOW_ADVISOR_ENTERTAINMENT_PREPARING_KG, "距離元旦慶祝賽開始所剩月份：" },
    {TR_WINDOW_ADVISOR_ENTERTAINMENT_UNDERWAY_KG, "元旦慶祝賽正在進行。動物祭品和熏香的氣味蓋過了競技場中刺鼻的鮮血和汗臭。" },
    {TR_WINDOW_ADVISOR_ENTERTAINMENT_PREPARING_OG, "距離奧運會開始所剩月份：" },
    {TR_WINDOW_ADVISOR_ENTERTAINMENT_UNDERWAY_OG, "奧運會正在進行。圣火日夜不熄，運動員們為不朽的榮耀而爭相競逐。" },
    {TR_WINDOW_ADVISOR_ENTERTAINMENT_PREPARING_IG, "距離羅馬運動會開始所剩月份：" },
    {TR_WINDOW_ADVISOR_ENTERTAINMENT_UNDERWAY_IG, "羅馬運動會正在進行。每位參賽者都用自己的鮮血、汗水和淚水來證明自己是最優秀的羅馬人。" },
    {TR_WINDOW_BUILDING_GAMES_REMAINING_DURATION, "大型賽事將再持續" }, // ...XX Days, 
    {TR_WINDOW_ADVISOR_ENTERTAINMENT_GAMES_COOLDOWN_TEXT, "近期已舉辦大型賽事。" },
    {TR_WINDOW_ADVISOR_ENTERTAINMENT_GAMES_UNDERWAY, "大型賽事正在進行。城市中充斥著來自各地的游客和狂歡活動。" },
    {TR_WINDOW_BUILDING_ARENA_CLOSED, "競技場已關閉。有鳥兒在空座上筑起了巢。" },
    {TR_WINDOW_BUILDING_ARENA_NO_SHOWS, "該競技場既沒有獅子也沒有角斗士，僅提供丁點娛樂。" },
    {TR_WINDOW_BUILDING_ARENA_BOTH_SHOWS, "該競技場舉辦角斗士和獵殺動物表演，以取悅當地民眾。" },
    {TR_WINDOW_BUILDING_ARENA_NEEDS_LIONS, "定期舉行的角斗士表演一直娛樂著大眾，要是有斗獅表演就更為壯觀了。" },
    {TR_WINDOW_BUILDING_ARENA_NEEDS_GLADIATORS, "競技場中饑餓的野獸四處徘徊尋找獵物，由于缺乏訓練有素的角斗士，表演持續時間短暫。" },
    {TR_BUILDING_MARS_TEMPLE_MODULE_DESC_NO_MESS, "一旦完工，該神廟會為你的軍糧站收集食物。" },
    {TR_OVERLAY_ARENA_COL, "競 技 場" },
    {TR_TOOLTIP_OVERLAY_ARENA_COL_1, "該住宅未連接到競技場或大競技場。" },
    {TR_TOOLTIP_OVERLAY_ARENA_COL_2, "該住宅可在競技場中看到一種表演。" },
    {TR_TOOLTIP_OVERLAY_ARENA_COL_3, "該住宅可在競技場中看到兩種表演。" },
    {TR_TOOLTIP_OVERLAY_ARENA_COL_4, "該住宅可在大競技場中看到一種表演。" },
    {TR_TOOLTIP_OVERLAY_ARENA_COL_5, "該住宅可在大競技場中看到兩種表演。" },
    {TR_TOOLTIP_OVERLAY_PROBLEMS_STRIKE, "罷工工人" },
    {TR_TOOLTIP_OVERLAY_PROBLEMS_NO_WATER_ACCESS, "未連接到水源" },
    {TR_TOOLTIP_OVERLAY_PROBLEMS_CARTPUSHER, "等待推車工" },
    {TR_TOOLTIP_OVERLAY_PROBLEMS_NO_RESOURCES, "無可用資源" },
    {TR_TOOLTIP_OVERLAY_PROBLEMS_MOTHBALLED, "建筑擱置待用" },
    {TR_WINDOW_BUILDING_TOURISM_DISABLED, "該類建筑過多-游客正在避開此地。" },
    {TR_WINDOW_BUILDING_TOURISM_ANNUAL, "今年來自游客的收入：" },
    {TR_WINDOW_BUILDING_WHARF_MOTHBALLED, "你的貿易顧問命令漁業停產。"},
    {TR_WINDOW_ADVISOR_FINANCE_INTEREST_TRIBUTE, "貢金/利息"},
    {TR_WINDOW_ADVISOR_TOURISM, "各式各樣"},
    {TR_WINDOW_FIGURE_TOURIST, "該游客已消費："},
    {TR_FIGURE_TYPE_TOURIST,  "游客"},
    {TR_SAVEGAME_LARGER_VERSION_TITLE, "存檔版本不兼容" },
    {TR_SAVEGAME_LARGER_VERSION_MESSAGE, "即將載入的存檔出自Augustus較新版本。\n請更新Augustus至最新版。" },
    {TR_BUILDING_COLONNADE, "柱   廊" },
    {TR_BUILDING_LARARIUM, "神   龕" },
    {TR_WINDOW_BUILDING_LARARIUM_DESC, "這座小型圣祠旨在供奉家庭守護神拉列斯-羅馬人的祖靈。在這里可以進行小型獻祭和祈禱。"},
    {TR_BUILDING_WATCHTOWER, "哨   塔"},
    {TR_BUILDING_WATCHTOWER_DESC, "駐扎在該塔上的弓箭手們時刻保持警惕，提防外來入侵者，同時樓下的街道由看守人巡邏。" },
    {TR_BUILDING_WATCHTOWER_DESC_NO_SOLDIERS, "該哨塔需要衛兵才能保衛我們的城市。"},
    {TR_BUILDING_NYMPHAEUM, "寧芙神廟"},
    {TR_BUILDING_NYMPHAEUM_DESC, "該大型神廟旨在供奉水仙女寧芙-泉水、河流、海洋里的小神靈。它能賦予這片土地-美麗和富饒，予以城市神圣的眷顧。" },
    {TR_BUILDING_SMALL_MAUSOLEUM, "小型陵墓"},
    {TR_BUILDING_SMALL_MAUSOLEUM_DESC, "該陵墓是城市里的貴族為已故先人打造的安息之地，也是許多活著的人羨慕的地方。" },
    {TR_BUILDING_LARGE_MAUSOLEUM, "大型陵墓"},
    {TR_BUILDING_LARGE_MAUSOLEUM_DESC, "這座華麗的陵墓有著拋光的大理石圍墻和雕塑，反映著貴族們的偉大功績，因而贏得了在此安息的特權。" },
    {TR_FIGURE_TYPE_WATCHMAN, "看守人" },
    {TR_TOOLTIP_BUTTON_ROADBLOCK_PERMISSION_MAINTENANCE, "切換是否允許巡警和工程師通過" },
    {TR_TOOLTIP_BUTTON_ROADBLOCK_PERMISSION_PRIEST, "切換是否允許祭司通過" },
    {TR_TOOLTIP_BUTTON_ROADBLOCK_PERMISSION_MARKET, "切換是否允許市場大媽通過" },
    {TR_TOOLTIP_BUTTON_ROADBLOCK_PERMISSION_ENTERTAINER, "切換是否允許娛樂藝人通過" },
    {TR_TOOLTIP_BUTTON_ROADBLOCK_PERMISSION_EDUCATION, "切換是否允許教育行人通過" },
    {TR_TOOLTIP_BUTTON_ROADBLOCK_PERMISSION_MEDICINE, "切換是否允許醫護人員通過" },
    {TR_TOOLTIP_BUTTON_ROADBLOCK_PERMISSION_TAX_COLLECTOR, "切換是否允許稅吏通過" },
    {TR_TOOLTIP_BUTTON_ROADBLOCK_PERMISSION_LABOR_SEEKER, "切換是否允許招工人通過" },
    {TR_TOOLTIP_BUTTON_ROADBLOCK_PERMISSION_MISSIONARY, "切換是否允許傳教士通過" },
    {TR_TOOLTIP_BUTTON_ROADBLOCK_PERMISSION_WATCHMAN, "切換是否允許看守人通過" },
    {TR_HOTKEY_DUPLICATE_TITLE, "熱鍵已占用"},
    {TR_HOTKEY_DUPLICATE_MESSAGE, "該鍵位已設定為以下功能："},
    {TR_BUILDING_GARDEN_PATH, "花園小徑" },
    {TR_BUTTON_GO_TO_SITE, "前往施工現場" },
    {TR_BUTTON_HALT_MONUMENT_CONSTRUCTION, "暫停建造" },
    {TR_BUTTON_RESUME_MONUMENT_CONSTRUCTION, "繼續建造" },
    {TR_RESOURCE_FISH, "魚肉" },
    {TR_WARNING_VARIANT_TOGGLE, "按下'R'轉向鍵可旋轉建筑方向"},
    {TR_BUILDING_SCHOOL_UPGRADE_DESC, "這座學校正在運轉，本地的發展讓市民們能夠擴建該建筑，為學童提供更多空間。"},
    {TR_BUILDING_THEATRE_UPGRADE_DESC, "這座戲院正在運轉，本地的發展讓市民們能夠擴建該建筑，為觀眾提供更多座位。" },
    {TR_BUILDING_LIBRARY_UPGRADE_DESC, "這座圖書館正在運轉，本地的發展讓市民們能夠擴建該建筑，為學者提供更多書卷和讀書角。" },
    {TR_WINDOW_BUILDING_INFO_WARNING_NO_MONUMENT_ROAD_ACCESS, "該奇觀的施工場地未連接至道路，無法接收建材，將道路連接至其中一塊中段瓷磚即可。"},
    {TR_WINDOW_BUILDING_WORKSHOP_STRIKING, "由于城市情緒低下，該作坊的工人已宣布罷工且拒絕工作！" },
    {TR_CITY_WARNING_SECESSION, "平民們拒絕上工！" },
    {TR_CITY_WARNING_WAREHOUSE_BREAKIN, "一座倉庫已遭洗劫！" },
    {TR_CITY_WARNING_GRANARY_BREAKIN, "一座谷倉已遭洗劫！" },
    {TR_CITY_WARNING_THEFT, "公共集會所或元老院的資金遭到偷竊！" },
    {TR_CITY_MESSAGE_TITLE_LOOTING, "城市正遭洗劫！"},
    {TR_CITY_MESSAGE_TEXT_LOOTING, "一座倉庫或谷倉被憤怒的市民們掠搶一空！城市情緒低下可能導致此類事件再次發生。"},
    {TR_SIDEBAR_EXTRA_INVASIONS, "入侵" },
    {TR_SIDEBAR_EXTRA_INVASION_UNDERWAY, "敵軍來犯！" },
    {TR_SIDEBAR_EXTRA_NO_INVASIONS, "未發現入侵" },
    {TR_SIDEBAR_EXTRA_INVASION_IMMINENT, "入侵迫在眉睫" },
    {TR_SIDEBAR_EXTRA_ENEMIES_CLOSING, "敵軍正在接近" },
    {TR_SIDEBAR_EXTRA_ENEMIES_DISTANT, "敵軍距離遙遠" },
    {TR_SIDEBAR_EXTRA_GODS, "神靈" },
    {TR_SIDEBAR_EXTRA_REQUESTS_VIEW_ALL, "查看所有" },
    {TR_SIDEBAR_EXTRA_REQUESTS_SMALL_FORCE, "小批軍隊"},
    {TR_SIDEBAR_EXTRA_REQUESTS_AVERAGE_FORCE, "適量軍隊"},
    {TR_SIDEBAR_EXTRA_REQUESTS_LARGE_FORCE, "大批軍隊"},
    {TR_SIDEBAR_EXTRA_REQUESTS_SEND, "發出請求"},
    {TR_SIDEBAR_EXTRA_REQUESTS_STOCK, "存儲"},
    {TR_SIDEBAR_EXTRA_REQUESTS_UNSTOCK, "停止存儲"},
    {TR_TOOLTIP_ADVISOR_HOUSING_GRAPH_BUTTON, "查看人口曲線圖" },
    {TR_TOOLTIP_ADVISOR_POPULATION_HOUSING_BUTTON, "拜訪你的住房顧問" },
    {TR_BUILDING_MONUMENT_CONSTRUCTION_ARCHITECT_NEEDED, "該階段建造這座奇觀所需的材料已備齊，工人們正在等待建筑師協會派出師傅前來指點進一步施工。" },
    {TR_RETURN_ALL_TO_FORT, "全軍回營" },
    {TR_TOOLTIP_ADVISOR_ENTERTAINMENT_GAMES_BUTTON, "賽事資訊" },
    {TR_WINDOW_BUILDING_GOVERNMENT_LARGE_STATUE_WATER_WARNING, "如果能打開這座雕像的噴泉，它看起來會更加宏偉，請為之供水。" },
    {TR_OPTION_MENU_APPLY, "應用" },
    {TR_OPTION_MENU_COST, "這將耗費" },
    {TR_FIGURE_TYPE_LOOTER, "劫匪" },
    {TR_FIGURE_TYPE_ROBBER, "強盜" },
    {TR_OVERLAY_SENTIMENT, "情    緒" },
    {TR_WINDOW_ADVISOR_ENTERTAINMENT_NO_COVERAGE, "沒有人" },
    {TR_WINDOW_ADVISOR_ENTERTAINMENT_FULL_COVERAGE, "所有人" },
    {TR_TOOLTIP_ADVISOR_TRADE_LAND_POLICY, "選擇一項陸地貿易政策"},
    {TR_TOOLTIP_ADVISOR_TRADE_LAND_POLICY_REQUIRED, "需要建造商隊驛站才能設立陸地貿易政策"},
    {TR_TOOLTIP_ADVISOR_TRADE_SEA_POLICY, "選擇一項海上貿易政策"},
    {TR_TOOLTIP_ADVISOR_TRADE_SEA_POLICY_REQUIRED, "需要建造燈塔才能設立海上貿易政策"},
    {TR_TOOLTIP_HOLD_GAME, "舉辦賽事"},
    {TR_TOOLTIP_NO_GAME, "不舉辦賽事"},
    {TR_WARNING_NEARBY_WOLF, "無法在狼群周邊建造" },
    {TR_MARKET_TRADING, "買賣中"},
    {TR_MARKET_NOT_TRADING, "未買賣"},
    {TR_TEMPLE_DISTRIBUTING, "分發中"},
    {TR_TEMPLE_NOT_DISTRIBUTING, "未分發"},
    {TR_ADVISOR_CHIEF_NO_IMMIGRATION_SQUALOR, "惡劣的居住條件和帳篷阻止了移民遷入"},
    {TR_FIGURE_TYPE_LIGHTHOUSE_SUPPLIER, "燈塔看守人"},
    {TR_WINDOW_GAMES_NO_WATER_ACCESS, "維納絲未接觸到貯水池。"},
};

void translation_traditional_chinese(const translation_string **strings, int *num_strings)
{
    *strings = all_strings;
    *num_strings = sizeof(all_strings) / sizeof(translation_string);
}
