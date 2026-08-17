#ifndef THEMEMODULEREADER_H
#define THEMEMODULEREADER_H

#include "json_reader.h"
#include "dro/themes/theme_scene.h"

#include <datatypes.h>

class ThemeModuleReader : public JSONReader
{
public:
  ThemeModuleReader(QString t_moduleDirectory, QString t_moduleName);
  void ParseModule();
  void ParseModuleConfig();
  void ParseLayers();
  void ParseTabs();
  ThemeScene *ParseScene(QString t_scene);

  void ParseSubFont(QJsonObject t_fontObject, QString t_fontType, ThemeScene *t_scene);


  void SwitchModuleTarget(QString t_target);

  ThemeScene *getThemeScene(RPSceneType t_scene);

  bool getContainsSceneWidget(RPSceneType t_scene, QString t_name);

  //layers
  bool getContainsLayers();
  QVector<QStringList> getWidgetLayers();

  //Tabs
  QVector<ThemeTabInfo> getTabs();

  //Config
  bool getContainsBool(QString t_setting);
  bool getSettingBool(QString t_setting);

  //Config - Sounds
  bool getContainsSound(QString t_sfxName);
  QString getSoundFile(QString t_sfxName);

  QString getDirectoryPath();
private:
  //Config
  QHash<QString, bool> m_configBooleans = {};
  QHash<QString, QString> m_configSounds = {};

  //Scenes
  ThemeScene *m_CourtroomScene = nullptr;
  ThemeScene *m_LobbyScene = nullptr;
  ThemeScene *m_ReplaysScene = nullptr;
  ThemeScene *m_ViewportScene = nullptr;

  //Other
  QVector<ThemeTabInfo> m_themeTabs = {};
  QVector<QStringList> m_WidgetLayers = {};

  QString m_moduleDirectory = "";
};

#endif // THEMEMODULEREADER_H
