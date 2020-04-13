# Сервер игры в "Поле чудес"
Здесь представлена асинхронная реализация сервера с использованием epoll. Все события, связанные с подключениями, возможностями для записи или чтения обрабатываются через epoll.  
Пример использования можно увидеть в main.cpp - нужно передать номер порта в конструктор сервера и запустить метод ServeForever.
Слова для игры берутся из файла, имя которого указывается в конструкторе класса WordPool.  
Когда попытки угадывания кончаются, или пользователь угадывает слово, соединение обрывается (в будущем хотелось бы поменять такое поведение).
Также не доделана остановка сервера (возможно, стоило использовать сигналы); исходники не распиханы по директориям.
