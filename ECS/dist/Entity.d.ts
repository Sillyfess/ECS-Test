export declare class Entity {
    private static nextId;
    readonly id: number;
    private componentMask;
    private alive;
    constructor();
    setComponentMask(mask: bigint): void;
    getComponentMask(): bigint;
    addComponentBit(componentTypeId: number): void;
    removeComponentBit(componentTypeId: number): void;
    hasComponentBit(componentTypeId: number): boolean;
    destroy(): void;
    isAlive(): boolean;
    reset(): void;
}
//# sourceMappingURL=Entity.d.ts.map