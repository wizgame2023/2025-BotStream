/*!
@file TutorialStage.h
@brief Derived WaveStage
*/

#include "stdafx.h"

namespace basecross {

    class TutorialStage : public WaveStageBase {
    protected:
        //ビューの作成
        virtual void CreateViewLight();

        virtual void CreateFloor() override;
        virtual void CreateWall() override;
        virtual void CreateCeiling() override;

        //各オブジェクトへの参照用
        // シーン
        weak_ptr<Scene> m_scene;
        // プレイヤー
        weak_ptr<Player> m_player;
        // 敵管理オブジェクト
        weak_ptr<EnemyManager> m_enemyMgr;
        // ボス
        weak_ptr<EnemyBase> m_boss;

        shared_ptr<BossGaugeUI> m_bossGauge;

        //Waveが次に移ったときに呼び出される
        virtual void WaveInitialize() override;

    public:
        //構築と破棄
        TutorialStage() : WaveStageBase() {}
        virtual ~TutorialStage() {}
        //初期化
        virtual void OnCreate()override;
        virtual void OnUpdate()override;
    };

}