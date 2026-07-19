# HANDOFF — Project Oracle
**Data:** 19/07/2026 · **Versão:** 0.6 · **Dona:** Karen (karen-arwen)

Documento de passagem: tudo que existe, como funciona, o que falta e como continuar — legível por humanos e por qualquer assistente de IA que assumir o desenvolvimento.

---

## 1. O que é o projeto

**Project Oracle** — jogo Cozy Open World AAA na Unreal Engine **5.6.1** (C++ + Blueprints). Inspirações: Heartopia, Infinity Nikki, Neverness to Everness, Fantasy Life, Stardew Valley, Animal Crossing, BOTW, Dreamlight Valley. Direção de arte: anime/semi-realista, vibrante, iluminação cinematográfica. A visão completa está no "Master Prompt" da Karen (público 15–35, moda, fazenda, pets, coleções, NPCs com rotina, estações, multiplayer futuro).

- **Pasta local:** `C:\Users\arwen\OneDrive\Documentos\GitHub\joguin`
- **Repositório:** `github.com/karen-arwen/project-oracle` (**privado** — obrigatório: contém referência a assets Fab; os packs em si estão fora do git via .gitignore, licença Epic proíbe redistribuir)
- **Abrir o projeto:** duplo clique em `Oracle.uproject` (NUNCA pelo launcher "Inicializar" — abre elevado e impede controle remoto da IA). Recompilar módulos: responder **Sim**.
- **Compilar mudanças C++ com o editor aberto:** Ctrl+Alt+F11 (Live Coding) — funciona e foi usado várias vezes.

## 2. Estado: o que está PRONTO e testado em máquina

Loop cozy completo, jogável em qualquer mapa (os sistemas se montam sozinhos via GameMode):

| Sistema | Estado | Onde (código) |
|---|---|---|
| Movimento (walk/run/sprint, pulo variável, coyote time, jump buffer) | ✅ testado | `Character/` |
| Câmera 3ª pessoa (zoom scroll, lag, FOV kick no sprint, dip no pouso) | ✅ | `Camera/` |
| Personagem visual (mannequin Quinn + ABP_Unarmed do pacote Third Person, via C++) | ✅ | `OracleCharacter` construtor |
| Enhanced Input (Data Asset + fallback runtime — joga sem nenhum asset) | ✅ | `Input/` |
| Interação [E] (interface + busca por proximidade + prompt no HUD) | ✅ | `Interaction/` |
| Inventário (pilhas, Data Assets de item) | ✅ | `Inventory/` |
| Coleta com respawn (cogumelos gigantes, troncos) | ✅ | `World/OracleGatherable` |
| Fazenda (plantar→regar→dias passam→colher; abóbora cresce visualmente) | ✅ | `Farming/` |
| Tempo (relógio, dia de 20min, eventos OnDay/OnHourChanged) | ✅ | `Core/OracleTimeSubsystem` |
| Céu/luz dinâmicos (SkyAtmosphere, nuvens volumétricas, fog, sol dourado, **lua à noite**, grading anime) | ✅ | `World/OracleEnvironment` |
| Decoração [B] (ghost com grid snap 50cm, girar [R], posicionar, guardar) | ✅ | `Building/` |
| Save/Load [F5]/[F9] (jogador, tempo, inventário, canteiros, móveis) | ✅ | `Core/OracleSave*` |
| HUD provisório (relógio, mochila, prompts — Canvas, será CommonUI) | ✅ | `UI/OracleHUD` |
| Mundo demo autogerado (itens/coletáveis/fazendinha spawnam no Play) | ✅ | `Demo/OracleDemoWorld` |

**Controles:** WASD mover · mouse câmera · scroll zoom · Espaço pular · Shift sprint · Ctrl andar · E interagir · B decorar · clique posicionar · R girar · F5/F9 save/load.

## 3. Conteúdo/assets no projeto

- `Content/Characters` + `Content/ThirdPerson` — pacote Third Person Epic (Quinn + animações). **No git.**
- `Content/StylizedIsland` — **Stylized Village** (Hivemind, resgatado grátis). Mapa vitrine: `StylizedIsland/Scenes/Village_Main` — o jogo RODA nele (GameMode spawna nossos sistemas). **Fora do git** (re-baixar: editor → botão Fab → Minha biblioteca → Add to project).
- `Content/PackOfTreeEnts` — criaturas-árvore (SAN 3D Art). **Fora do git.**
- `Content/Fantastic_Village_Pack` — **FANTASTIC Village Pack** (Tidal Flask, 4.8★, grátis) — estilo colorido Infinity Nikki. Baixado, **ainda não usado**. **Fora do git.**
- `Content/Oracle/Maps/L_Sandbox` — nosso mapa (chão plano). É onde a vila própria do Oracle será montada.
- Itens demo usam meshes do StylizedIsland via soft path com fallback para formas da engine (projeto compila e joga sem os packs).

## 4. Arquitetura (regras que TODOS devem seguir)

- **Character é coordenador fino** — lógica vive em componentes (`Interaction`, `Inventory`, `Placement`, `CameraController`) e no `OracleCharacterMovementComponent` (gaits; enums prontos para escalada/planador/montaria via MOVE_Custom).
- **Conteúdo = Data Assets** (`UOracleItemDefinition`, `UOracleCropDefinition`). Centenas de itens/roupas/cultivos virão de assets, não de código.
- **Tudo que depende de tempo escuta o `UOracleTimeSubsystem`** (nunca polling) — NPCs, lojas, festivais plugam aí.
- **Null-safe sempre**: sem asset → fallback, nunca crash.
- Save versionado (`UOracleSaveGame::CurrentVersion`), itens resolvidos por nome.
- Detalhes completos: `docs/ARCHITECTURE.md` (inclui tabela "onde cada sistema futuro vive").

## 5. Problemas conhecidos / avisos

- **VRAM estourou** com 2 editores + PIE no Village_Main ("Video memory exhausted"). Solução: manter UM editor aberto; fechar Chrome/apps ao jogar no mapa da vila.
- Editor às vezes abre **duas instâncias** (duplo clique repetido) — verificar barra de tarefas.
- Launcher Epic e apps elevados **bloqueiam o controle da IA** (UIPI); editor aberto via .uproject funciona.
- "HLODs desatualizados" no Village_Main — cosmético, ignorar (mapa vitrine de terceiros).
- Mensagem "compiler 14.44 not preferred" no build — só warning, ignorar.
- Save do conteúdo demo não migra quando itens virarem Data Assets reais (por design).
- `bSpawnDemoContent=true` no GameMode — desligar quando o conteúdo real existir.

## 6. Git/GitHub

- 6+ commits na branch `main`; publicar mudanças = GitHub Desktop → Summary → Commit → Push origin.
- `.gitignore` exclui: Binaries/Intermediate/Saved/DerivedDataCache, packs Fab (StylizedIsland, PackOfTreeEnts — **adicionar Fantastic_Village_Pack se ainda não estiver!**) e seus __ExternalActors__/__ExternalObjects__.
- Git LFS inicializado no GitHub Desktop (histórico antigo tem uassets raw <100MB — ok).

## 7. Próximos passos (ordem recomendada)

1. **Adicionar `Content/Fantastic_Village_Pack/` ao .gitignore** (1 linha, igual aos outros packs).
2. **Sessão de tuning**: jogar e ajustar game feel (velocidades, pulo, câmera — tudo é UPROPERTY).
3. **Vila do Oracle no L_Sandbox**: arrastar casas/props do Fantastic_Village_Pack e StylizedIsland; virar o mapa principal.
4. **Áudio v1**: passos, coleta, ambiente (vento/pássaros) — ganchos null-safe JÁ existem (`FootstepSound`, `GatherSound`, `LandSound` etc.); falta atribuir assets (buscar packs de áudio grátis no Fab).
5. **Coleções/compêndio** (pilar do design) — novo módulo `Collections/`, Data Assets.
6. **UI bonita** (Common UI) substituindo o HUD Canvas.
7. Depois: NPCs com rotina (escutando TimeSubsystem), pesca, clima/estações (expandir OracleEnvironment), moda/editor de personagem.
8. Semanalmente: resgatar grátis da Fab (aba Fab → "Grátis por tempo limitado").

## 8. Documentos do projeto (pasta `docs/`)

`SETUP.md` (instalar/compilar do zero) · `ARCHITECTURE.md` · `GUIA_TESTE_COMPLETO.md` (loop de 10min) · `GUIA_PERSONAGEM_VISUAL.md` (ABP customizado futuro) · `ART_DIRECTION.md` (packs recomendados + como aplicar) · `ROADMAP.md` (módulos ✅ e futuros) · `CHANGELOG.md` (histórico 0.1→0.6) · este `HANDOFF.md`.

**Regra de ouro do estúdio:** um módulo por vez, completamente funcional, e sempre perguntar — *"isso está divertido?"*
