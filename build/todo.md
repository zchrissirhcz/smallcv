改进 Point 类的设计，增加 `operator Point_<Tp2>` 重载函数。
这个函数用来避免 line()/circle() 函数要实现两份。

这需要先实现 `saturate_cast` 函数/类。
