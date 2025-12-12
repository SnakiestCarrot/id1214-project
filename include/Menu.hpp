class Menu {
public:
    Menu();
    ~Menu();

    void show();
    void hide();
    bool isVisible() const;

private:
    bool visible;
    
};