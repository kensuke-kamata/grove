# Notes from the Game Programmer Book

## Derived data and consistency

> ただ、計算で求まる情報を保存するやり方には、矛盾が発生しうるという欠点がある。検索して得られるプレイヤーの位置と、変数で保存されたプレイヤーの位置が違ってしまう可能性がゼロではないということだ。心配事が増えるくらいなら毎回探した方がいい、という考え方はあってもいい。速度が問題にならない限りは、生成できる情報は必要なときに生成するようにした方が無難だろう。

### Takeaway

When information can be derived from an authoritative source, storing a second copy creates a consistency risk. Prefer deriving it when needed unless the performance cost is significant.

### Keywords

- derived data
- source of truth
- consistency
- premature optimization
