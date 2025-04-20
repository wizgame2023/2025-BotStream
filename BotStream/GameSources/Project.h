/*!
@file Project.h
@brief コンテンツ用のヘッダをまとめる
*/

#pragma once


#include "ProjectShader.h"
#include "ProjectBehavior.h"
#include "Scene.h"
#include "GameStage.h"
#include "Character.h"
//#include "Player.h"


#include "MyGameObject.h"//全ての親クラス

#include "ObjectMove.h" //動くオブジェクト
#include "ObjectNotMove.h"//動かないオブジェクト
#include "Ground.h";//地面
//#include "Actor.h"


//Actorの中に入ってるやつ？
#include "AttackCollision.h"
#include "State.h"
#include "LandDetect.h"

//BillBoard
#include "BillBoard.h"

//State
#include "EnemyState.h"
#include "PlayerState.h"

//Actor
#include "Player.h"
#include "Enemy.h"

//Sprite
#include "Sprite.h"

//UI関係
#include "PlayerUI.h"

//開発ステージ用
#include "StageAbe.h"
#include "StageSanpei.h"
#include "StageSato.h"
#include "StageTuboi.h"
#include "WaveStage.h"

//Manager
#include "CameraManager.h"
#include "EffectManager.h"
#include "EnemyManager.h"


