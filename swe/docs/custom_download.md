# Custom Download
## Select Download Path
When enabled, the user can select a "Download To" folder. If a Download Descriptor is available, the "Download To" folder is automatically selected based on the download file's MIME type.

### Enable Condition
The feature is enabled when the following parameter values are not empty:

* `swe_downloadpath_activity_intent`
* `swe_downloadpath_activity_result_selection`

## Use-case: Direct Downloads
When user downloads from a hyperlink pointing directly to any file, a prompt is displayed with message specified by `swe_download_infobar_text`. The message contains the name of the download file and name of the Download To folder. The download to folder name is hyperlinked. On clicking the hyperlink, the browser starts an activity with intent specified by `swe_downloadpath_activity_intent`.

The activity is expected to return with the full path of the Download To folder using the Intent class' extra string. The key name of the extra string that contains the download folder's full path is specified by `swe_downloadpath_activity_result_selection`.

### Use-case: OMA OTA Downloads
Open Mobile Alliance's Over The Air Download specification allows for using a Download Descriptor to download a file. The hyperlink used to download a file in this case does not directly point to the file.

The flow to handle such downloads is similar to the Direct Downloads use-case. In OMA OTA Download case, however, the Snapdragon Browser automatically defaults to an appropriate Download To folder based on the type of file.

#### Default Download To Folders
| MIME Type | Folder |
|---|---|
| audio | Environment.DIRECTORY_MUSIC |
| movies | Environment.DIRECTORY_MOVIES |
| image | Environment.DIRECTORY_PICTURES |
| (default) | Environment.DIRECTORY_DOWNLOADS |

### Configuration
The following configuration parameters are available. These parameters can be customized by overriding them in the following file:
*browser_root*/swe/channels/*channel_name*/values/strings.xml

For more information about using Channels and strings.xml, please refer to [Customizing using Channels](channels.md)

| Definition | Default Value | Description |
|---|---|---|
| `<string name="swe_download_infobar_text"></string>` | Do you want to download ^1 to ^2? | Prompt shown to user before downloading a file. ^1 is substituted with download file name. ^2 is replaced with the folder name and is hyperlinked. |
| `<string name="swe_downloadpath_activity_intent" translatable="false"></string>` | (Empty) | Intent string used to start an activity when user taps on download to folder hyperlink |
| `<string name="swe_downloadpath_activity_result_selection" translatable="false"></string>` | (Empty) | The key name used retrieve full path of download to folder using Intent.getExtraString() |

### Note
Intent string looks like "android.intent.action.SEARCH"

## Fail Download on Low Storage Space
This feature checks for available storage space and fails the download if not enough space is available for the Download Content.

### Enable Condition
This feature is always enabled

### Configuration
The following configuration parameters are available. These parameters can be customized by overriding them in the following file:
*browser_root*/swe/channels/*channel_name*/values/strings.xml

For more information about using Channels and strings.xml, please refer to [Customizing using Channels](channels.md)

| Definition | Default Value | Description |
|---|---|---|
| `<string name="swe_downloadpath_not_enough_space"></string>` | Not enough space to download | Prompt shown to the user when not enough space is available for download content. |
