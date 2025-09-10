export declare abstract class Component {
    private static typeIdCounter;
    private static typeIds;
    static getTypeId<T extends Component>(componentClass: new (...args: any[]) => T): number;
    abstract clone(): Component;
}
export interface ComponentPool<T extends Component> {
    get(index: number): T | undefined;
    set(index: number, component: T): void;
    remove(index: number): void;
    clear(): void;
}
//# sourceMappingURL=Component.d.ts.map