# Media Download
When enabled, this feature presents a dialog that lets the user select to download audio, video files or play them.

## Enable Condition
This feature is enabled when `swe_feature_allow_media_downloads` configuration parameter is set to `true`

## Configuration
The following configuration parameters are available. These parameters can be customized by overriding them in the following file:
*browser_root*/swe/channels/*channel_name*/values/strings.xml

For more information about using Channels and strings.xml, please refer to [Customizing using Channels](channels.md)

| Definition | Default Value | Description |
|---|---|---|
| `<bool name="swe_feature_allow_media_downloads"></bool>` | `false` | Set to `true` to make the browser show a dialog box to let user select between downloading or playing audio and video files |
| `<string name="swe_media_msg"></string>` | "Please select how to handle media file" | String to use as body of dialog |
