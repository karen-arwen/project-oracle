# Guia — Testando o loop completo (v0.3.0)

Depois de compilar (ver `SETUP.md`) e criar o mapa Basic, aperte **Play**. O mundo demo se monta sozinho: esferas rosadas são arbustos de fruta, cilindros marrons são troncos, e a grade de blocos achatados é a fazendinha. O HUD mostra relógio, mochila e dicas.

## O loop de 10 minutos

1. **Colete** — aproxime-se de um arbusto rosa e aperte **E** ("Coletar Fruta Estrela"). Ele some e reaparece em ~45s. Colete madeira nos troncos.
2. **Plante** — vá à fazendinha. Você começa com 4 sementes: **E** num canteiro vazio planta.
3. **Regue** — **E** de novo rega (o prompt muda). Sem regar, não cresce.
4. **Espere** — o dia dura 20 minutos reais (ajustável). O sol se move sozinho. À meia-noite, cultivos regados crescem; em 2 dias, colheita.
5. **Colha** — quando o broto ficar vermelho e grande, **E** dá 3 frutas.
6. **Decore** — aperte **B**: um fantasma do Banquinho segue seu olhar com snap de grade (verde = pode). **Clique** posiciona, **R** gira 45°, **B** sai. **E** num móvel posicionado o guarda de volta.
7. **Salve** — **F5** salva tudo (posição, hora, mochila, canteiros, móveis). Mude as coisas e aperte **F9**: o mundo volta.

## Ajustes rápidos (para sentir na prática)

Quer acelerar o teste da fazenda? No editor, sem tocar em código: **Window → World Settings** não — o tempo é subsystem. Mais fácil: em `Source/Oracle/Core/OracleTimeSubsystem.h`, mude `DayLengthRealMinutes = 20.f` para `2.f` e recompile — cada dia dura 2 minutos.

## O que este demo prova (e o que não prova)

Prova: todos os sistemas conversam — interação, inventário, tempo, fazenda, decoração e save funcionam juntos, na arquitetura definitiva. Trocar as formas geométricas por assets bonitos NÃO muda uma linha desses sistemas: itens/cultivos reais serão Data Assets criados no editor (clique direito → Miscellaneous → Data Asset → OracleItemDefinition), e o demo se desliga com `bSpawnDemoContent = false` no GameMode.

Não prova: beleza. O passe visual (Módulo 12) é onde entram assets do Fab (ex.: packs gratuitos de natureza estilizada), materiais, iluminação e pós-processo. Faremos juntos quando o loop estiver gostoso nas suas mãos.

## Problemas conhecidos / limitações da v0.3.0

- O tint colorido dos placeholders depende do parâmetro "Color" do material básico da engine; se tudo aparecer cinza, é cosmético — o gameplay funciona igual.
- Save v1 usa slot único e resolve itens por nome; quando os Data Assets reais substituírem o demo, saves antigos do demo não migram (por design — é conteúdo descartável).
- O fantasma da decoração fica sólido (sem transparência) até termos um material próprio de ghost.
