export abstract class Component {
    private static typeIdCounter = 0;
    private static typeIds = new Map<Function, number>();

    public static getTypeId<T extends Component>(componentClass: new (...args: any[]) => T): number {
        if (!this.typeIds.has(componentClass)) {
            this.typeIds.set(componentClass, this.typeIdCounter++);
        }
        return this.typeIds.get(componentClass)!;
    }

    public abstract clone(): Component;
}

export interface ComponentPool<T extends Component> {
    get(index: number): T | undefined;
    set(index: number, component: T): void;
    remove(index: number): void;
    clear(): void;
}