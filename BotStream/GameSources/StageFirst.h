/*!
@file StageFirst.h
@brief Derived WaveStage
*/

#include "stdafx.h"
#include "WaveStageBase.h"

namespace basecross {

    class StageFirst : public WaveStageBase {
    protected:
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

    public:
        //構築と破棄
        StageFirst() : WaveStageBase() {}
        virtual ~StageFirst() {}
        //初期化
        virtual void OnCreate()override;
    };

}