#define STATE_NO_GHOST 0
#define STATE_GHOST_START 1
#define STATE_GHOST_DONE 2
#define STATE_CHANGE_BOOT_FAIL 3

#define COMMAND_CHECK _T("-check")
#define COMMAND_START _T("-start")
#define COMMAND_RETRY _T("-retry")

#define TMS_FOLDER_NAME _T("TMS")
#define TMS_REMOTE_RESTORE_NAME _T("TMSRemoteRestore")
#define BCDEDIT_TOOL_NAME _T("bcdedit.exe")
#define HARDDISK_VOLUME_STRING _T("HarddiskVolume")

#define GHOST_EVENT_STRING _T("ghost-event")
#define GHOST_STATUS_STRING _T("ghost-status")
#define GHOST_STATUS_SUCCEED 1
#define GHOST_STATUS_FAIL 0
