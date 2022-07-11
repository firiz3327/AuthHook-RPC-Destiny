## MapleStory Global v176.1

RPC
- NGS Removed
- MSCRC Patched
- Nexon Logging Removed
- Multi Client Enabled
- Discord Rich Presence Support

Destiny (JP)
- Allow other language to type (https://forum.ragezone.com/f427/release-allow-language-type-chat-1206111/)

Launched via the following...

    MapleStory.exe GameLaunching 8.31.99.141 8484

- https://github.com/TEAM-SPIRIT-Productions/AuthHook176_RPC
- https://github.com/KururuLABO/DestinyClient


### やり方

これ自体のビルドはTeam Spritを参照 [github](https://github.com/TEAM-SPIRIT-Productions/AuthHook176_RPC)

v176のダウンロード
[setup](http://download2.nexon.net/Game/MapleStory/FullVersion/176/MSSetupv176.exe) | [z01](http://download2.nexon.net/Game/MapleStory/FullVersion/176/MSSetupv176.z01) | [z02](http://download2.nexon.net/Game/MapleStory/FullVersion/176/MSSetupv176.z02) | [z03](http://download2.nexon.net/Game/MapleStory/FullVersion/176/MSSetupv176.z03) | [z04](http://download2.nexon.net/Game/MapleStory/FullVersion/176/MSSetupv176.z04) | [z05](http://download2.nexon.net/Game/MapleStory/FullVersion/176/MSSetupv176.z05) | [z06](http://download2.nexon.net/Game/MapleStory/FullVersion/176/MSSetupv176.z06)

v176のサーバー
[SwordieMS](https://bitbucket.org/swordiemen/swordie/src/master/)

1. Cloneしてきたらsqlフォルダの中身全部mysqlで実行、最初InitTables_charactersをするといいかも
2. [WZDumper](https://github.com/Xterminatorz/WZ-Dumper/releases/tag/v1.9.4.1)でサーバー用wzダンプする。クライアントのフォルダを指定してスレッド多くしてダンプすれば割と早い。サーバー直下に配置(swordie/wz/Base.wz | Character.wz って感じ)
3. Intellij IDEAとかでプロジェクト開いてStart.java実行 最初はdat生成で起動長い

### なうなやんぐ

日本語入力はコピーで行う。ブラウザとかからコピーした文字はなんか貼り付けられんのでVisual Studioとかで一度書いて試すといいかも
まあ文字化けするけど