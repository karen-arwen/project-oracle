# Roadmap — Project Oracle

Regra: um módulo por vez, cada um 100% funcional antes do próximo. Critério de pronto incluído em cada módulo.

## Fase 1 — Prototype (provar que é divertido)

| # | Módulo | Conteúdo | Pronto quando |
|---|---|---|---|
| 1 | ✅ Fundação | Projeto, git, arquitetura, docs | Compila e roda |
| 2 | ✅ Locomoção + Câmera | Walk/run/sprint, pulo, zoom, 8 direções | Jogável no sandbox |
| 3 | 🔨 Personagem visual | Mesh, Animation Blueprint, blend de locomoção; game feel de pulo/câmera (código ✅, editor pendente — ver GUIA_PERSONAGEM_VISUAL.md) | Animações refletem as marchas E o checklist de game feel passa |
| 4 | ✅ Interação | Interface Interactable + componente, prompt na tela | Pegar um item do chão |
| 5 | ✅ Inventário | Itens como Data Assets, HUD provisório | Coletar, ver itens |
| 6 | ✅ Coleta no mundo | Recursos colhíveis com respawn | Loop coletar→guardar |
| 7 | ✅ Fazenda v1 | Plantar, regar, crescer (dias), colher | Ciclo completo de 1 cultivo |
| 8 | ✅ Tempo v1 | Ciclo dia/noite + relógio + sol (Subsystem) | Cultivo cresce com o tempo |
| 9 | ✅ Save v1 | F5/F9: jogador, tempo, inventário, fazenda, móveis | Fechar o jogo e continuar |
| 10 | ✅ Decoração v1 | Fantasma com grid snap, girar, posicionar/guardar | Decorar com móveis |
| 11 | Sessão de tuning | TESTAR TUDO em mãos, ajustar valores, corrigir bugs | Checklist de game feel passa |
| 12 | ✅ Passe visual v1 | Packs Fab integrados, luz cozy, sol/lua dinâmicos | Vila jogável e bonita |
| 13 | ✅ Coleções v1 | Compêndio (descobertas, totais), toasts de coleta, save v2 | Descobrir item novo dá alegria |
| 14 | Vila do Oracle | Montar nossa vila no L_Sandbox com o FANTASTIC pack | Mapa próprio digno de screenshot |
| 15 | Áudio v1 | Passos, coleta, ambiente — ganchos prontos, faltam assets | Mundo deixa de ser mudo |

**Meta do Prototype:** 20 minutos de loop cozy — explorar, coletar, plantar, decorar — que dê vontade de continuar. Tudo com assets placeholder/marketplace.

## Fase 2 — Vertical Slice (provar a qualidade)

Um recorte pequeno do mundo com qualidade final: 1 vila + 1 bioma, arte definitiva, 2–3 NPCs com rotina e diálogo, moda v1 (trocar roupas), pesca v1, clima e estações v1, música e UI com identidade. É o material de divulgação/pitch.

## Fase 3 — Alpha

Mundo expandido (World Partition), escalada/planador/natação, NPCs com relacionamento, culinária, pets, quests, coleções, eventos sazonais v1.

## Fase 4 — Beta → Release → Live Service

Conteúdo completo, otimização (consoles/Steam Deck), localização, achievements, polish. Depois: festivais sazonais, expansões, multiplayer co-op (decisão de arquitetura de rede será revisitada ao final da Alpha — o CMC já é network-ready).

## Princípios de escopo

Cozy primeiro: profundidade em poucos sistemas antes de largura em muitos. Nada de multiplayer, masmorras ou veículos antes do loop central estar delicioso. Cada fase corta features sem dó — a lista da visão é o horizonte, não o contrato da v1.
