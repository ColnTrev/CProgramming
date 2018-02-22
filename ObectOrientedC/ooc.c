

#include <stdio.h>
#include "List.h"

void revprint(Node* head){
    if(!head)return;
    revprint(head->next);
    printf("%d ", head->data);
}

void rp_wrapper(List* lst){
    revprint(lst->head);
}

void ppp(List* lst){
    puts("This is C magic. How does it work?");
}

int main(void){
    List test  = __init__();
    List test2 = __init__();
    List test3 = __init__();
    
    test.constructor(&test);
    test2.constructor(&test2);
    test3.constructor(&test3);
    
    for (int i = 0; i < 20; i++) {
        test.add(&test, i);
    }
    for (int i = 1; i <= 20; i *= 2) {
        test2.add(&test2, i);
    }
    printf("List 1: ");
    test.print(&test);
    test.print = rp_wrapper;
    printf("List 1 reversed: ");
    test.print(&test);
    putchar('\n');
    
    printf("List 2: ");
    test2.print(&test2);
    puts("Changing print function in List 2.");
    test2.print = ppp;
    test2.print(&test2);
    printf("Size function List 1: ");
    test.size(&test);
    printf("Size function List 2: ");
    test2.size(&test2);
    
    printf("Test 1 == Test2: %d. Test1 != Test2: %d.\n",
           test.equality(&test, &test2), test2.inequality(&test2, &test));
    
    printf("Test with empty list Test1 == Test3: %d.\n",
           test.equality(&test, &test3));
    
    __destroy__(&test);
    __destroy__(&test2);
    __destroy__(&test3);
    return 0;
}
