# Kniflash 游戏项目技术文档

## 1. 文件树结构

```
.
├─figs/                 # 存放游戏资源文件（图片等）
├─build/               # Qt项目构建输出目录（可忽略）
├─*.cpp               # 源文件
├─*.h                 # 头文件
└─*.pro               # Qt项目配置文件
```

## 2. 类和数据结构

### 2.1 主要类

#### MySence（场景类）
- 继承自`QGraphicsScene`
- 主要成员变量：
  - `timer`: 游戏主循环定时器
  - `character_attack_timer`: 角色攻击定时器
  - `aimTimer`: 瞄准线定时器
  - `aimline`: 瞄准线对象
  - `cur_ai_num`: 当前AI数量
  - `kill_num`: 击杀数量
  - `cmt`: 角色移动定时器

#### Character（角色基类）
- 继承自`QGraphicsObject`
- 主要成员变量：
  - `health`: 生命值
  - `knife_num`: 飞刀数量
  - `speed`: 移动速度
  - `knife_r`: 近战攻击范围
  - `aim_range`: 瞄准范围
  - `dead`: 死亡状态
  - `high_speed`: 高速状态标志
  - `buff_lables`: 状态效果图标

#### Mob（敌人AI类）
- 继承自`Character`
- 主要成员变量：
  - `is_moving`: 移动状态标志
  - `mob_is_dead`: 死亡状态标志
- 新增功能：
  - 智能寻路系统
  - 随机移动行为
  - 道具追踪
  - 玩家追踪

#### Prop（道具类）
- 继承自`QGraphicsObject`
- 主要成员变量：
  - `m_prop_class`: 道具类型（飞刀/生命/靴子）
  - `picked`: 是否被拾取
- 道具类型：
  - `KNIFE`: 飞刀道具
  - `HEALTH`: 生命值道具
  - `BOOTS`: 速度提升道具

#### AimLine（瞄准线类）
- 用于实现瞄准系统
- 主要功能：
  - 动态瞄准线显示
  - 飞刀动画效果
  - 玩家/AI瞄准线区分

## 3. 算法

### MySence类
- `checkDistance()`: 检查物体间距离，处理碰撞检测
- `checkCharacterDistance()`: 检查角色间距离，处理角色交互
- `getAimedChar()`: 获取当前瞄准目标
- `resetAimLine()`: 重置瞄准线状态
- `character_move()`: AI移动控制
- `calculateMobDirection()`: 计算AI移动方向

### Character类
- `push_knife()`: 添加飞刀
- `pop_knife()`: 使用飞刀
- `add_health()`: 增加生命值
- `drop_health()`: 减少生命值
- `shoot()`: 发射飞刀攻击
- `be_hit()`: 处理被击中逻辑
- `picked_boots()`: 处理速度提升效果

### Mob类
- `handle_direction_input()`: 处理方向输入
- `handle_shoot()`: 处理射击行为
- `handle_mob_dead()`: 处理死亡状态

边界情况：
- 角色死亡时禁止移动和攻击
- 攻击有冷却时间
- 生命值为0时触发死亡
- AI移动有随机性和智能性
- 道具拾取有距离限制

## 4. 辅助函数

### 距离计算函数
- `areItemsClose()`: 检查两个物体是否足够接近
- `return_char_distance_squre()`: 计算两个角色之间的距离平方

### AI行为函数
- `calculateMobDirection()`: 计算AI移动方向
- `handle_direction_input()`: 处理AI移动输入

这些辅助函数主要用于：
- 碰撞检测
- 瞄准系统
- 道具拾取判定
- AI行为控制

## 5. 游戏特性

1. 多角色系统
   - 玩家角色
   - AI控制的敌人
   - 每个角色都有独立的生命值和攻击系统

2. 战斗系统
   - 飞刀投掷机制
   - 近战攻击
   - 生命值系统
   - 攻击冷却时间

3. 道具系统
   - 生命值恢复
   - 速度提升
   - 飞刀补充
   - 道具自动生成

4. 瞄准系统
   - 动态瞄准线
   - 目标锁定机制
   - 飞刀动画效果

5. AI系统
   - 智能寻路
   - 随机移动
   - 道具追踪
   - 玩家追踪

6. 游戏流程
   - 开始界面
   - 游戏主循环
   - 胜利/失败判定
   - 重新开始选项

这个项目是一个基于Qt框架开发的2D动作游戏，采用了面向对象的设计方法，实现了完整的游戏循环、角色控制系统和战斗机制。代码结构清晰，各个模块职责分明，便于维护和扩展。新增的AI系统和道具系统大大提升了游戏的可玩性和趣味性。
