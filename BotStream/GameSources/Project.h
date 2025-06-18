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
#include "Parts.h"//パーツ
//#include "Actor.h"


//Actorの中に入ってるやつ？
#include "AttackCollision.h"
#include "State.h"
#include "LandDetect.h"

//BillBoard
#include "BillBoard.h"

//State
#include "EnemyState.h"
#include "ZakoState.h"
#include "PlayerState.h"
#include "CameraState.h"

//Actor
#include "Player.h"
#include "Enemy.h"
#include "EnemyZako.h"

//Sprite
#include "Sprite.h"

//UI関係
#include "PlayerUI.h"
#include "EnemyUI.h"
#include "PauseUI.h"

//開発ステージ用
#include "WaveStageBase.h"
#include "TutorialStage.h"
#include "StageFirst.h"

#include "StageAbe.h"
#include "StageSanpei.h"
#include "StageSato.h"
#include "StageTuboi.h"
#include "WaveStage.h"
#include "TitleStage.h"
#include "StageSelect.h"
#include "GameClearScene.h"
#include "GameOverScene.h"

//Manager
#include "CameraManager.h"
#include "EffectManager.h"
#include "EnemyManager.h"
#include "StageCollisionController.h"
#include "PartsManager.h"
#include "SoundManager.h"


