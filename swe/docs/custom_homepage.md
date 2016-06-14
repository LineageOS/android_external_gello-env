# Custom Homepage
This feature enables partners to set a custom home page for the browser

## Enable Condition
The feature is enabled when the following parameter value is not empty:

* `default_homepage_url`

## Configuration
The following configuration parameters are available. These parameters can be customized by overriding them in the following file:
*browser_root*/swe/channels/*channel_name*/values/strings.xml

For more information about using Channels and strings.xml, please refer to [Customizing using Channels](channels.md)

| Definition | Default Value | Description |
|---|---|---|
| `<string name="default_homepage_url" translatable="false">""</string>` | (Empty) | Default homepage URL |
