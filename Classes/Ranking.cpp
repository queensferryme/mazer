#include <iostream>

#include "Config.h"
#include "Music.h"
#include "Ranking.h"
#include "StartMenu.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;

sqlite3 *Ranking::db = nullptr;

/* call back function for sqlite3 */
static int callback(void *data, int argc, char **argv, char **azColName) {
  Player temp;
  temp.name = argv[0];
  temp.grades = atoi(argv[1]);
  ((vector<Player> *)data)->push_back(temp);
  return 0;
}

/* initialize current cocos2d::Scene object */
bool Ranking::init() {
  if (!Scene::init())
    return false;
  // create default data
  db = openDataBase(Config::dbFilePath, db);
  selectData(players);
  sqlite3_close(db);
  // create back label
  auto backItem = MenuItemImage::create(
      "img/BackNormal.png", "img/BackClicked.png", [](Ref *pSpender) {
        playSoundEffect("audio/click.wav");
        Director::getInstance()->popScene();
      });
  backItem->setPosition(Vec2(60, 660));
  auto menu = Menu::create(backItem, NULL);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 1);
  // create ranking list
  char rank[10], gradesTemp[10];
  for (int i{0}; i < players.size() && i < 10; i++) {
    sprintf(rank, "%d", i + 1);
    sprintf(gradesTemp, "%d", players[i].grades);
    auto rankLabel = Label::createWithTTF(rank, "fonts/MarkerFelt.ttf", 20);
    auto nameLabel = Label::createWithTTF(players[i].name.c_str(),
                                          "fonts/MarkerFelt.ttf", 20);
    auto gradesLabel =
        Label::createWithTTF(gradesTemp, "fonts/MarkerFelt.ttf", 20);
    rankLabel->setPosition(Vec2(200, 600 - i * 50));
    nameLabel->setPosition(Vec2(300, 600 - i * 50));
    gradesLabel->setPosition(Vec2(500, 600 - i * 50));
    this->addChild(rankLabel);
    this->addChild(nameLabel);
    this->addChild(gradesLabel);
  }
  return true;
}

/* insert data into database */
void Ranking::insertData(const string &name, const string &grades,
                         sqlite3 *db) {
  char *zErrMsg = 0;
  string command = R"(INSERT INTO PLAYER (NAME, GRADES) VALUES (')" + name +
                   R"(',)" + grades + R"( );)";
  sqlite3_exec(db, command.c_str(), callback, 0, &zErrMsg);
}

/* open database connection */
sqlite3 *Ranking::openDataBase(const std::string &fileName, sqlite3 *db) {
  char *zErrMsg = 0;
  sqlite3_open(fileName.c_str(), &db);
  string command =
      R"(CREATE TABLE IF NOT EXISTS PLAYER(NAME TEXT NOT NULL, GRADES INT NOT NULL);)";
  sqlite3_exec(db, command.c_str(), callback, 0, &zErrMsg);
  return db;
}

/* select top 10 records from database */
void Ranking::selectData(std::vector<Player> &players) {
  char *zErrMsg = 0;
  std::string command = R"(SELECT * FROM PLAYER ORDER BY GRADES DESC;)";
  sqlite3_exec(db, command.c_str(), callback, (void *)&players, &zErrMsg);
}
