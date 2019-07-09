#include <string>
#include <vector>

#include "cocos2d.h"
#include "sqlite3.h"

using namespace cocos2d;

typedef struct {
  std::string name;
  int grades{0};
} Player;

/**
@brief   Ranking scene
*/
class Ranking : public Scene {
public:
  static sqlite3 *db;
  std::vector<Player> players;

  /**
  @brief    Create scene with `static create()`
  @return   pointer to a cocos2d::Scene object
  */
  static Scene *createScene() { return Ranking::create(); }

  /**
  @brief    Implementation of `static create()` function,
            used in createScene()
  @return   pointer to an object of current class
  */
  CREATE_FUNC(Ranking);

  /*
  @brief    Open database connection
  @param    std::string, database file name
  @param    sqlite3 *, a pointer to db connection
  @return   a pointer to db connection
  */
  static sqlite3 *openDataBase(const std::string &, sqlite3 *);

  /*  */
  static void insertData(const std::string &name, const std::string &grades,
                         sqlite3 *db);
  void selectData(std::vector<Player> &players);

  /**
  @brief   Initialize current cocos2d::Scene object
  @return   status of creation:
            * true -> success
            * false -> failure
  */
  virtual bool init();
};