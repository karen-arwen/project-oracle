# Guia — Personagem visual (Módulo 3, parte de editor)

O código C++ já faz todo o cálculo. Este guia é o trabalho de editor: dar corpo, animação e som ao personagem. Faça na ordem, depois de compilar o projeto. Tempo estimado: 1–2 horas na primeira vez.

## 1. Importar o conteúdo Third Person da Epic (gratuito)

1. No editor: **Content Browser → Add (+) → Add Feature or Content Pack**.
2. Aba **Blueprint → Third Person → Add to Project**.
3. Isso traz o boneco **Quinn/Manny** (`SKM_Quinn`), esqueleto e um conjunto completo de animações (idle, walk, run, jump) em `Content/Characters/`.

## 2. Criar o Blueprint do personagem

1. No Content Browser, navegue até **C++ Classes → Oracle → Character → OracleCharacter**.
2. Clique direito → **Create Blueprint class based on OracleCharacter**.
3. Salve como `Content/Oracle/Characters/BP_OracleCharacter`.
4. Abra o BP, selecione o componente **Mesh**:
   - *Skeletal Mesh Asset*: `SKM_Quinn`
   - *Location*: (0, 0, **-90**)  |  *Rotation*: (0, 0, **-90**)
   - *Anim Class*: deixe vazio por enquanto (passo 3).
5. **Project Settings → Maps & Modes → Selected GameMode → Default Pawn Class** = `BP_OracleCharacter`.
   - Se não puder editar ali, crie `BP_OracleGameMode` (filho de `OracleGameMode`), defina o Pawn nele e aponte o *Default GameMode* do projeto para ele.

Aperte Play: o boneco aparece e desliza em T-pose. Esperado — falta o Animation Blueprint.

## 3. Criar o Animation Blueprint (ligado ao C++)

1. Clique direito no Content Browser → **Animation → Animation Blueprint**.
2. Em *Parent Class* escolha **OracleAnimInstance** (nossa classe C++ — é isso que liga o grafo ao código). Skeleton: o do `SKM_Quinn` (`SK_Mannequin`).
3. Salve como `Content/Oracle/Characters/ABP_Oracle`.

### 3a. Blend Space de locomoção

1. Clique direito → **Animation → Legacy → Blend Space 1D**, skeleton do Quinn. Nome: `BS_Locomotion`.
2. *Horizontal Axis*: nome `Speed`, mínimo **0**, máximo **750**.
3. Arraste as animações para a linha: `MM_Idle` em 0, `MM_Walk_Fwd` em **200**, `MM_Run_Fwd` em **500** e de novo `MM_Run_Fwd` em **750** (com Play Rate maior via *Rate Scale* ≈ 1.2 se quiser).
   - Os valores 200/500/750 correspondem às velocidades Walk/Run/Sprint do código.

### 3b. State Machine

No **AnimGraph** do `ABP_Oracle`:

1. Crie uma **State Machine** ("Locomotion") ligada ao *Output Pose*.
2. Estados:
   - **Ground**: toca `BS_Locomotion`, com o pino *Speed* ligado à variável **GroundSpeed** (herdada do C++ — aparece na lista de variáveis).
   - **Fall**: toca `MM_Fall_Loop`.
   - **Land**: toca `MM_Land` (ou `MM_Jump` na descida, teste o que fica melhor).
3. Transições:
   - Ground → Fall: condição **bIsFalling**.
   - Fall → Land: **NOT bIsFalling**.
   - Land → Ground: *Automatic Rule Based on Sequence Player* (marca a checkbox na transição) — sai quando a animação de pouso termina.
4. Compile e salve. No `BP_OracleCharacter`, componente Mesh → *Anim Class* = `ABP_Oracle`.

Aperte Play: andar, correr, sprint, pular e cair devem estar animados e suaves.

## 4. Passos com som (Anim Notify → C++)

1. Abra a animação `MM_Walk_Fwd`. Na timeline, nos frames em que cada pé toca o chão, clique direito na faixa Notifies → **Add Notify → New Notify** → nome `Footstep`.
2. Repita em `MM_Run_Fwd`.
3. No **Event Graph** do `ABP_Oracle`, aparece o evento **AnimNotify_Footstep** → ligue-o a **Try Get Pawn Owner → Cast to OracleCharacter → PlayFootstep** (função C++ já pronta, null-safe).
4. No `BP_OracleCharacter`, categoria **Oracle|Feedback**, atribua o som *Footstep Sound* (qualquer som curto serve para testar; o pacote **Starter Content** e packs gratuitos do Fab têm opções).

## 5. Feedback de pouso (já funciona; só atribuir assets)

O shake de câmera no pouso **já funciona sem nenhum asset** (classe C++ `OracleLandingShake`). Para completar:

- *Land Sound*: um "thump" curto.
- *Land Dust VFX*: um Niagara simples — clique direito → **Niagara System → template "Fountain"** e reduza para um puff de poeira, ou use um pack gratuito do Fab. Salve em `Content/Oracle/VFX/NS_LandDust`.

Ambos são atribuídos no `BP_OracleCharacter` em **Oracle|Feedback**.

## 6. Checklist de game feel para a sessão de teste

Rode e avalie honestamente ("isso está divertido?"):

- [ ] Parar/iniciar movimento tem peso, sem parecer patinação?
- [ ] O giro do personagem acompanha a câmera de forma agradável?
- [ ] O pulo tem peso (subida rápida, descida com gravidade)?
- [ ] Segurar Espaço pula visivelmente mais alto que um toque?
- [ ] Pular no limite de uma borda funciona (coyote time)?
- [ ] Apertar pulo logo antes de pousar emenda outro pulo (buffer)?
- [ ] O sprint dá sensação de velocidade (FOV)?
- [ ] O pouso "assenta" a câmera de forma gostosa, não enjoativa?

Anote qualquer valor que pareceu errado (ex.: "sprint lento demais", "shake forte demais") — todos são propriedades editáveis no BP/C++ e ajustamos juntos no passe de tuning.
