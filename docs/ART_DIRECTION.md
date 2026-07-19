# Direção de Arte — Project Oracle

Referências: Infinity Nikki, Heartopia, Neverness to Everness. O que esses jogos têm em comum tecnicamente: iluminação física com céu dinâmico, vegetação estilizada densa, paleta saturada e quente, bloom generoso, personagens anime sobre mundo semi-realista.

## O que o código já entrega (v0.4.0)

O `AOracleEnvironment` cria automaticamente o mesmo rig de iluminação usado em produção AAA: céu atmosférico físico (SkyAtmosphere), nuvens volumétricas, névoa exponencial com godrays (volumetric fog + Lumen), skylight em tempo real, e color grading vibrante (saturação +14%, bloom 0.7, vinheta suave). O sol nasce laranja às 6h, fica branco-quente ao meio-dia, põe-se laranja às 18h e a noite é azulada com névoa — tudo sincronizado com o relógio do jogo.

Isso é a **fundação de luz**. A beleza final = esta luz + assets estilizados.

## Passo a passo para o salto visual (na sua máquina)

### 1. Instalar packs do Fab (loja de assets da Epic, dentro do editor)

No editor: **Window → Fab** (ou fab.com logada com sua conta Epic). Busque com filtro **Free** primeiro — a Epic libera packs pagos gratuitamente todo mês, e há muito conteúdo permanente gratuito. Termos de busca que funcionam: "stylized nature", "anime environment", "stylized fantasy".

Packs alinhados com nossa direção (verifique preço/licença na página):

- [Anime Nature Pack](https://www.fab.com/listings/d1ee0166-c909-48dc-8fd3-373511560bee) — grama, flores, árvores e ruínas pintadas à mão, estilo filme de anime. Exatamente nossa vibe.
- [Stylized Nature Vol. 1](https://www.fab.com/listings/842f8137-94b0-4a0c-9a11-e918a0d1158d) — bundle grande de ambiente estilizado com ferramentas de landscape.
- [Stylized Nature Pack](https://www.fab.com/listings/b066de06-73b8-4fbe-b30c-468f5bcf7575) — alternativa compacta.
- Na [busca por tag "anime"](https://www.fab.com/search?tags=anime) há personagens e cenários no estilo.

Clique em **Add to My Library → Add to Project → Oracle**.

### 2. Aplicar (sem tocar em código!)

- **Cenário**: arraste árvores/pedras/grama do pack para o mapa; use o Foliage Mode (tecla 3) para pintar grama em massa.
- **Itens do jogo**: crie os Data Assets reais (clique direito → Miscellaneous → Data Asset → `OracleItemDefinition`) e aponte `WorldMesh` para as malhas do pack (uma cadeira de verdade no lugar do cubo). Depois desligue `bSpawnDemoContent` no GameMode.
- **Personagem**: siga o `GUIA_PERSONAGEM_VISUAL.md`; para visual anime, busque "anime character" ou "stylized character" no Fab e aplique no lugar do Quinn (mesmo processo de Mesh + retarget de animações).

### 3. Ajustes finos que mudam tudo (5 minutos cada)

- **Landscape**: crie um terreno (Modes → Landscape) com material do pack no lugar do chão cinza.
- **Água**: plugin **Water** da engine (Edit → Plugins → Water) dá lagos/rios prontos.
- **Grading por bioma**: os valores do PostProcess vivem no `AOracleEnvironment` (categoria Oracle) — experimente saturação/bloom no editor em tempo real.

## Regra de ouro

Compre/baixe UM pack de natureza e UM de vila primeiro. Coerência de estilo > quantidade: misturar 10 packs de estilos diferentes é o erro nº 1 de projetos indie.

Fontes: [Fab — Anime Nature Pack](https://www.fab.com/listings/d1ee0166-c909-48dc-8fd3-373511560bee), [Fab — Stylized Nature Vol. 1](https://www.fab.com/listings/842f8137-94b0-4a0c-9a11-e918a0d1158d), [Fab — Stylized Nature Pack](https://www.fab.com/listings/b066de06-73b8-4fbe-b30c-468f5bcf7575), [Fab — tag anime](https://www.fab.com/search?tags=anime)
