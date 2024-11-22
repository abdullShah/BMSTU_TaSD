import os

def generate_tree_recursive(value, current_level, max_level, result):
    """
    Рекурсивно создаёт дерево и добавляет значения в список result.
    """
    if current_level > max_level:
        return

    result.append(value)  # Записываем текущий узел
    generate_tree_recursive(2 * value, current_level + 1, max_level, result)  # Левый потомок
    generate_tree_recursive(2 * value + 1, current_level + 1, max_level, result)  # Правый потомок

def generate_tree(levels):
    """
    Генерирует значения узлов дерева с заданным количеством уровней.
    """
    if levels < 1:
        return []

    result = []
    generate_tree_recursive(1, 1, levels, result)
    return result

def save_tree_to_file(levels, directory="trees"):
    """
    Создаёт файл с деревом заданного уровня и записывает узлы в формате PreOrder.
    """
    if levels < 1:
        print("Уровень дерева должен быть больше 0.")
        return

    # Убедимся, что директория существует
    os.makedirs(directory, exist_ok=True)

    # Создаём имя файла
    filename = os.path.join(directory, f"level_{levels}.txt")

    # Генерируем дерево и записываем его в файл
    tree_nodes = generate_tree(levels)
    with open(filename, "w") as file:
        for value in tree_nodes:
            file.write(f"{value}\n")

    print(f"Дерево с {levels} уровнями записано в файл: {filename}")

if __name__ == "__main__":
    import sys

    if len(sys.argv) < 2:
        print("Укажите уровень дерева как аргумент. Например:")
        print("python script.py 7")
        sys.exit(1)

    try:
        levels = int(sys.argv[1])
        save_tree_to_file(levels)
    except ValueError:
        print("Уровень дерева должен быть числом.")
