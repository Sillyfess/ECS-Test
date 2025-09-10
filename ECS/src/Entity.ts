export class Entity {
    private static nextId = 0;
    public readonly id: number;
    private componentMask: bigint = 0n;
    private alive: boolean = true;

    constructor() {
        this.id = Entity.nextId++;
    }

    public setComponentMask(mask: bigint): void {
        this.componentMask = mask;
    }

    public getComponentMask(): bigint {
        return this.componentMask;
    }

    public addComponentBit(componentTypeId: number): void {
        this.componentMask |= (1n << BigInt(componentTypeId));
    }

    public removeComponentBit(componentTypeId: number): void {
        this.componentMask &= ~(1n << BigInt(componentTypeId));
    }

    public hasComponentBit(componentTypeId: number): boolean {
        return (this.componentMask & (1n << BigInt(componentTypeId))) !== 0n;
    }

    public destroy(): void {
        this.alive = false;
        this.componentMask = 0n;
    }

    public isAlive(): boolean {
        return this.alive;
    }

    public reset(): void {
        this.componentMask = 0n;
        this.alive = true;
    }
}