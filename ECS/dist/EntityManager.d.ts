import { Entity } from './Entity';
export declare class EntityManager {
    private entities;
    private entityPool;
    private readonly poolSize;
    constructor();
    private initializePool;
    createEntity(): Entity;
    destroyEntity(entityId: number): void;
    getEntity(entityId: number): Entity | undefined;
    getAllEntities(): Entity[];
    getActiveEntities(): Entity[];
    getEntitiesWithComponentMask(mask: bigint): Entity[];
    clear(): void;
    getEntityCount(): number;
}
//# sourceMappingURL=EntityManager.d.ts.map